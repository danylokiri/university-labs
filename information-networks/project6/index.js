const readline = require('readline');
const crypto = require('crypto');

const BASE = 'http://127.0.0.1:3000/secure';

const b64 = (buf) => Buffer.from(buf).toString('base64');
const fromB64 = (s) => Buffer.from(s, 'base64');

function aesEncrypt(aes, text) {
    const cipher = crypto.createCipheriv('aes-256-cbc', fromB64(aes.key), fromB64(aes.iv));
    const enc = Buffer.concat([cipher.update(text, 'utf8'), cipher.final()]);
    return b64(enc);
}
function aesDecrypt(aes, cipherTextBase64) {
    const decipher = crypto.createDecipheriv('aes-256-cbc', fromB64(aes.key), fromB64(aes.iv));
    const dec = Buffer.concat([decipher.update(fromB64(cipherTextBase64)), decipher.final()]);
    return dec.toString('utf8');
}
function rsaEncrypt(pubPem, text) {
    const out = crypto.publicEncrypt(
        { key: pubPem, padding: crypto.constants.RSA_PKCS1_OAEP_PADDING, oaepHash: 'sha256' },
        Buffer.from(text, 'utf8')
    );
    return b64(out);
}

function makeHmacKey(aes) {
    return crypto.createHash('sha256')
        .update(`${aes.key}:${aes.iv}`)
        .digest(); // Buffer
}

async function httpGetJson(url) {
    const r = await fetch(url);
    if (!r.ok) throw new Error(`GET ${url} -> ${r.status}`);
    return r.json();
}
async function httpPostJson(url, body, headers = {}) {
    const r = await fetch(url, {
        method: 'POST',
        headers: { 'content-type': 'application/json', ...headers },
        body: JSON.stringify(body),
    });
    if (!r.ok) {
        const t = await r.text().catch(() => '');
        throw new Error(`POST ${url} -> ${r.status} ${t}`);
    }
    return r.json();
}

async function waitForServer(url, attempts = 40, ms = 250) {
    for (let i = 1; i <= attempts; i++) {
        try { const r = await fetch(url); if (r.ok) return; } catch {}
        await new Promise(res => setTimeout(res, ms));
    }
    throw new Error('Server is not reachable');
}

async function initSession() {
    const { rsaId, publicKey } = await httpGetJson(`${BASE}/get-public-key`);
    const aes = { key: b64(crypto.randomBytes(32)), iv: b64(crypto.randomBytes(16)) };
    const sessionId = crypto.randomBytes(8).toString('hex');

    const payload = JSON.stringify({ sessionId, aesKey: aes.key, iv: aes.iv });
    const payloadBase64 = rsaEncrypt(publicKey, payload);

    const resp = await httpPostJson(
        `${BASE}/session/init`,
        { payloadBase64 },
        { 'x-rsa-id': rsaId }
    );

    return { aes, sessionId, expiredAt: resp.expiredAt };
}

async function sendMessage(ctx, text) {
    const cipherTextBase64 = aesEncrypt(ctx.aes, text);

    const ts = Date.now();
    const nonce = crypto.randomBytes(8).toString('hex');
    const hKey = makeHmacKey(ctx.aes);

    let hmac = crypto.createHmac('sha256', hKey)
        .update(String(ts))
        .update(String(nonce))
        .update(String(cipherTextBase64))
        .update(String(ctx.sessionId))
        .digest('base64');

   // hmac = hmac.slice(0, -1) + (hmac.slice(-1) === 'A' ? 'B' : 'A'); //zlamanii test

    const body = {
        cipherTextBase64,
        ts,
        nonce,
        hmac
    };

    const data = await httpPostJson(
        `${BASE}/message/send`,
        body,
        { 'x-session-id': ctx.sessionId }
    );

    const expectedHmac = crypto.createHmac('sha256', hKey)
        .update(String(data.ts))
        .update(String(data.nonce))
        .update(String(data.cipherTextBase64))
        .update(String(ctx.sessionId))
        .digest('base64');

    if (data.hmac !== expectedHmac) {
        console.error('Bad HMAC in server reply!');
    }

    const decrypted = aesDecrypt(ctx.aes, data.cipherTextBase64);
    return JSON.parse(decrypted);
}

async function main() {
    try {
        await waitForServer(`${BASE}/get-public-key`);
        const ctx = await initSession();
        console.log(`Connected | sessionId=${ctx.sessionId} | expiresAt=${ctx.expiredAt || 'n/a'}`);

        const rl = readline.createInterface({ input: process.stdin, output: process.stdout });
        const ask = () => rl.question('> ', async (msg) => {
            if (!msg.trim()) return ask();
            try {
                const ans = await sendMessage(ctx, msg.trim());
                console.log('Reply:', ans);
            } catch (e) {
                console.error('Error:', e.message);
            }
            ask();
        });
        ask();
    } catch (e) {
        console.error('starting error:', e.message);
        process.exit(1);
    }
}
main();
