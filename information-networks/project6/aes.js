const crypto = require('crypto');

const b64 = (buf) => Buffer.from(buf).toString('base64');
const fromB64 = (s) => Buffer.from(s, 'base64');

function generateSecretKey() {
    const key = crypto.randomBytes(32);
    const iv  = crypto.randomBytes(16);
    return { key: b64(key), iv: b64(iv) };
}

function encrypt(aesKey, plainText) {
    if (!aesKey || typeof aesKey.key !== 'string' || typeof aesKey.iv !== 'string')
        throw new Error('Invalid key object');
    const cipher = crypto.createCipheriv('aes-256-cbc', fromB64(aesKey.key), fromB64(aesKey.iv));
    const enc = Buffer.concat([cipher.update(plainText, 'utf8'), cipher.final()]);
    return b64(enc);
}

function decrypt(aesKey, cipherTextBase64) {
    if (!aesKey || typeof aesKey.key !== 'string' || typeof aesKey.iv !== 'string')
        throw new Error('Invalid key object');
    const decipher = crypto.createDecipheriv('aes-256-cbc', fromB64(aesKey.key), fromB64(aesKey.iv));
    const dec = Buffer.concat([decipher.update(fromB64(cipherTextBase64)), decipher.final()]);
    return dec.toString('utf8');
}

module.exports = { generateSecretKey, encrypt, decrypt };
