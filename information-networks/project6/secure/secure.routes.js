const express = require('express');
const RSA = require('../rsa');   // используем твои модули
const AES = require('../aes');
const sessionStore = require('./session.store');

const router = express.Router();


const { publicKey, privateKey } = RSA.generateCryptoKeys();
const rsaState = { rsaId: 'rsa-1', publicKey, privateKey };
const crypto = require('crypto');
const { useNonce } = require('./session.store');

function b64(data) {
    return Buffer.isBuffer(data)
        ? data.toString('base64')
        : Buffer.from(String(data)).toString('base64');
}

function hmacKeyFrom(keyB64, ivB64) {
    return crypto.createHash('sha256').update(`${keyB64}:${ivB64}`).digest(); // Buffer
}

function makeHmac(hKeyBuf, parts) {
    const h = crypto.createHmac('sha256', hKeyBuf);
    for (const p of parts) h.update(typeof p === 'string' ? p : Buffer.from(p));
    return h.digest('base64');
}

const MAX_DRIFT_MS = parseInt(process.env.SIGN_DRIFT_MS || '60000', 10); // 60 секунд допустимого сдвига

function verifySignedRequest(req, res, next) {
    try {
        const { sessionId, cipherTextBase64, ts, nonce, hmac } = req.body || {};
        if (!sessionId || !cipherTextBase64 || !hmac || !ts || !nonce) {
            return res.status(400).json({ error: 'Bad signed payload' });
        }

        const check = sessionStore.validate(sessionId);
        if (check.status === 'missing') {
            return res.status(400).json({ error: 'Unknown session' });
        }
        if (check.status === 'expired') {
            return res.status(440).json({ error: 'Session expired', expiredAt: check.session.expiredAt });
        }

        const drift = Math.abs(Date.now() - Number(ts));
        if (!Number.isFinite(drift) || drift > MAX_DRIFT_MS) {
            return res.status(400).json({ error: 'Stale or invalid ts' });
        }

        if (!useNonce(sessionId, nonce)) {
            return res.status(409).json({ error: 'Replay detected' });
        }

        const { key, iv } = sessionStore.material(check.session);
        const hKey = hmacKeyFrom(key, iv);
        const expected = makeHmac(hKey, [String(ts), nonce, cipherTextBase64, sessionId]);
        if (expected !== hmac) {
            return res.status(400).json({ error: 'Bad signature' });
        }

        req.__sess = { sessionId, keyB64: key, ivB64: iv, hKey };
        next();
    } catch {
        return res.status(400).json({ error: 'Verify failed' });
    }
}

router.get('/get-public-key', (req, res) => {
    res.json({ rsaId: rsaState.rsaId, publicKey: rsaState.publicKey });
});


router.post('/session/init', (req, res) => {
    try {
        const rsaId = req.header('x-rsa-id');
        if (rsaId !== rsaState.rsaId) return res.status(400).json({ error: 'Bad rsaId' });
        const { payloadBase64 } = req.body || {};
        if (!payloadBase64) return res.status(400).json({ error: 'No payloadBase64' });

        const decryptedJson = RSA.decrypt(rsaState.privateKey, payloadBase64);
        const { sessionId, aesKey, iv } = JSON.parse(decryptedJson) || {};
        if (!sessionId || !aesKey || !iv) return res.status(400).json({ error: 'Bad payload' });

        sessionStore.set(sessionId, { key: aesKey, iv });
         const check = sessionStore.validate
           ? sessionStore.validate(sessionId)
               : null;
         res.json({
               ok: true,
               expiredAt: check?.session?.expiredAt // если в сторе есть expiredAt
         });
    } catch (e) { res.status(400).json({ error: e.message || 'Decrypt error' }); }
});


router.post('/message/send', (req, res) => {
    try {
        const sessionId = req.header('x-session-id');
        const { cipherTextBase64, ts, nonce, hmac } = req.body || {};

        console.log('[MSG] /message/send',
            'sessionId=', sessionId,
            'ts=', ts,
            'nonce=', nonce
        );

        if (!sessionId) return res.status(400).json({ error: 'No x-session-id' });
        if (!cipherTextBase64) return res.status(400).json({ error: 'No cipherTextBase64' });
        if (!ts || !nonce || !hmac) return res.status(400).json({ error: 'Bad signed payload' });

        // 1) Проверка сессии
        const check = sessionStore.validate(sessionId);
        if (check.status === 'missing') {
            console.log('[ERROR] unknown session', sessionId);
            return res.status(400).json({ error: 'Unknown session' });
        }
        if (check.status === 'expired') {
            console.log('[EXPIRED] session', sessionId, 'expiredAt=', check.session.expiredAt);
            return res.status(440).json({ error: 'Session expired', expiredAt: check.session.expiredAt });
        }
        const sess = sessionStore.material(check.session); // {key, iv}

        // 2) Проверка времени
        const drift = Math.abs(Date.now() - Number(ts));
        if (!Number.isFinite(drift) || drift > MAX_DRIFT_MS) {
            console.log('[ERROR] stale ts for', sessionId, 'drift=', drift);
            return res.status(400).json({ error: 'Stale/invalid ts' });
        }

        // 3) Anti-replay
        if (typeof useNonce === 'function' && !useNonce(sessionId, String(nonce))) {
            console.log('[REPLAY DETECTED]', sessionId, 'nonce=', nonce);
            return res.status(409).json({ error: 'Replay detected' });
        }

        // 4) Проверка HMAC
        const hKey = crypto.createHash('sha256').update(`${sess.key}:${sess.iv}`).digest();
        const expectedHmac = crypto
            .createHmac('sha256', hKey)
            .update(String(ts))
            .update(String(nonce))
            .update(String(cipherTextBase64))
            .update(String(sessionId))
            .digest('base64');

        if (expectedHmac !== hmac) {
            console.log('[ERROR] bad signature for', sessionId);
            return res.status(400).json({ error: 'Bad signature' });
        }

        // 5) Расшифровка входа
        const plain = AES.decrypt(sess, cipherTextBase64);

        // 6) Шифруем ответ
        const replyPlain = JSON.stringify({
            echoed: plain,
            receivedAt: new Date().toISOString(),
            sessionId
        });
        const outCipherB64 = AES.encrypt(sess, replyPlain);

        // 7) Подписываем ответ
        const tsOut = Date.now();
        const nonceOut = crypto.randomBytes(8).toString('hex');
        const hmacOut = crypto
            .createHmac('sha256', hKey)
            .update(String(tsOut))
            .update(String(nonceOut))
            .update(String(outCipherB64))
            .update(String(sessionId))
            .digest('base64');

        console.log('[OK] /message/send for', sessionId);

        return res.json({
            sessionId,
            cipherTextBase64: outCipherB64,
            ts: tsOut,
            nonce: nonceOut,
            hmac: hmacOut
        });
    } catch (e) {
        console.log('[ERROR] /message/send exception', e.message);
        return res.status(400).json({ error: e.message || 'AES error' });
    }
});


router.post('/create-session', (req, res) => {
    try {
        const { sessionId, cipherTextBase64 } = req.body || {};
        if (!sessionId || !cipherTextBase64) {
            return res.status(400).json({ error: 'sessionId and cipherTextBase64 required' });
        }

        const plain = RSA.decrypt(rsaState.privateKey, cipherTextBase64); // => '{"key":"...","iv":"..."}'
        let obj;
        try { obj = JSON.parse(plain); } catch { return res.status(400).json({ error: 'Invalid JSON inside RSA payload' }); }
        if (!obj || typeof obj.key !== 'string' || typeof obj.iv !== 'string') {
            return res.status(400).json({ error: 'Invalid AES material' });
        }

        sessionStore.set(sessionId, { key: obj.key, iv: obj.iv });

        const validation = sessionStore.validate(sessionId);
        return res.json({
            sessionId,
            status: validation.status,
            expiredAt: validation.session?.expiredAt,
        });
    } catch (e) {
        return res.status(400).json({ error: e.message || 'Session create failed' });
    }
});
module.exports = router;
