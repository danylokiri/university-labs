const TTL_MINUTES = parseInt(process.env.SESSION_TTL_MINUTES || '1', 10);
const SESS_TTL_MS = TTL_MINUTES * 60 * 1000;

const sessions = new Map();
const nonces = new Map();
const NONCE_LIMIT = 200;


function buildRecord(data) {
    const now = Date.now();
    return {
        key: data.key,
        iv: data.iv,
        createdAt: new Date(now).toISOString(),
        expiredAt: new Date(now + SESS_TTL_MS).toISOString(),
        ts: now,
        expired: false,
    };
}

function set(sessionId, data) {
    sessions.set(sessionId, buildRecord(data));
}

function validate(sessionId) {
    const rec = sessions.get(sessionId);
    if (!rec) return { status: 'missing', reason: 'Unknown session' };

    const now = Date.now();
    const expMs = Date.parse(rec.expiredAt);
    if (now >= expMs) {
        rec.expired = true;
        sessions.set(sessionId, rec);
        return { status: 'expired', session: rec, reason: 'Session expired' };
    }

    rec.ts = now;
    sessions.set(sessionId, rec);
    return { status: 'valid', session: rec };
}

function material(rec) {
    return { key: rec.key, iv: rec.iv };
}

function list() {
    const out = [];
    for (const [id, rec] of sessions) {
        out.push({
            id,
            state: rec.expired ? 'invalid' : 'valid',
            createdAt: rec.createdAt,
            expiredAt: rec.expiredAt,
            lastActivityAt: new Date(rec.ts).toISOString(),
        });
    }
    return out.sort((a, b) => a.id.localeCompare(b.id));
}

setInterval(() => {
    const now = Date.now();
    for (const [id, rec] of sessions) {
        const expCutoff = Date.parse(rec.expiredAt) + (10 * 60 * 1000);
        if (now > expCutoff) sessions.delete(id);
    }
}, 10 * 60 * 1000).unref();

function useNonce(sessionId, nonce) {
    if (!nonce || typeof nonce !== 'string') return false;
    const bag = nonces.get(sessionId) || new Set();
    if (bag.has(nonce)) return false;
    bag.add(nonce);
    if (bag.size > NONCE_LIMIT) {
        const it = bag.values();
        bag.delete(it.next().value);
    }
    nonces.set(sessionId, bag);
    return true;
}
module.exports = { set, validate, material, list, SESS_TTL_MS, TTL_MINUTES, useNonce };

