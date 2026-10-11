const crypto = require('crypto');

const b64 = (buf) => Buffer.from(buf).toString('base64');
const fromB64 = (s) => Buffer.from(s, 'base64');

function generateCryptoKeys() {
    const { publicKey, privateKey } = crypto.generateKeyPairSync('rsa', {
        modulusLength: 2048,
        publicKeyEncoding:  { type: 'spki',  format: 'pem' },
        privateKeyEncoding: { type: 'pkcs8', format: 'pem' }
    });
    return { publicKey, privateKey };
}

function encrypt(publicKeyPEM, plainText) {
    if (typeof publicKeyPEM !== 'string' || typeof plainText !== 'string')
        throw new Error('Invalid arguments');
    const out = crypto.publicEncrypt(
        { key: publicKeyPEM, padding: crypto.constants.RSA_PKCS1_OAEP_PADDING, oaepHash: 'sha256' },
        Buffer.from(plainText, 'utf8')
    );
    return b64(out);
}

function decrypt(privateKeyPEM, cipherTextBase64) {
    if (typeof privateKeyPEM !== 'string' || typeof cipherTextBase64 !== 'string')
        throw new Error('Invalid arguments');
    const out = crypto.privateDecrypt(
        { key: privateKeyPEM, padding: crypto.constants.RSA_PKCS1_OAEP_PADDING, oaepHash: 'sha256' },
        fromB64(cipherTextBase64)
    );
    return out.toString('utf8');
}

module.exports = { generateCryptoKeys, encrypt, decrypt };
