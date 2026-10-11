const express = require('express');
const cors = require('cors');
const swaggerUi = require('swagger-ui-express');
const swaggerJsdoc = require('swagger-jsdoc');

const RSA = require('./rsa');
const AES = require('./aes');

const app = express();
const PORT = process.env.PORT || 3000;


app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));


function secureLogger(req, res, next) {
    console.log(
        `[secure] ${req.method} ${req.originalUrl}`,
        Object.keys(req.body || {}).length ? JSON.stringify(req.body) : ''
    );
    next();
}


app.use('/secure', secureLogger, require('./secure/secure.routes'));
console.log('[secure] routes mounted');


let employees = [];
const genId = () => {
    if (employees.length === 0) return 1;
    const used = new Set(employees.map(e => e.id));
    let i = 1; while (used.has(i)) i++;
    return i;
};

const rsaKeysStore = [];
let nextRsaKeyId = 1;

app.get('/', (req, res) => res.send('Use /employees, /api-docs, and crypto endpoints'));

app.get('/employees', (req, res) => res.json(employees));

app.get('/employees/:id', (req, res) => {
    const emp = employees.find(e => e.id === +req.params.id);
    if (!emp) return res.sendStatus(404);
    res.json(emp);
});

app.post('/employees', (req, res) => {
    const { firstName, lastName, age } = req.body || {};
    const ageNum = Number(age);
    if (!firstName || !lastName || Number.isNaN(ageNum)) return res.sendStatus(400);
    const dup = employees.find(e => e.firstName === firstName && e.lastName === lastName && e.age === ageNum);
    if (dup) return res.sendStatus(400);
    const emp = { id: genId(), firstName, lastName, age: ageNum };
    employees.push(emp);
    res.json(emp);
});

app.put('/employees/:id', (req, res) => {
    const idx = employees.findIndex(e => e.id === +req.params.id);
    if (idx === -1) return res.sendStatus(404);
    const { firstName, lastName, age } = req.body || {};
    if (!firstName || !lastName || age === undefined) return res.sendStatus(400);
    employees[idx] = { ...employees[idx], firstName, lastName, age: +age };
    res.json(employees[idx]);
});

app.delete('/employees/:id', (req, res) => {
    const before = employees.length;
    employees = employees.filter(e => e.id !== +req.params.id);
    if (employees.length === before) return res.sendStatus(404);
    res.sendStatus(200);
});

app.post('/api/crypto-keys/generate/rsa-keys', (req, res) => {
    try {
        const { publicKey, privateKey } = RSA.generateCryptoKeys();
        const id = nextRsaKeyId++;
        rsaKeysStore.push({ id, publicKey, privateKey });
        res.status(201).json({ id });
    } catch {
        res.status(400).json({ message: 'RSA key generation failed' });
    }
});

app.get('/api/crypto-keys/rsa-public-key/:id', (req, res) => {
    const id = Number(req.params.id);
    const pair = rsaKeysStore.find(k => k.id === id);
    if (!pair) return res.status(404).json({ message: 'Key not found' });
    const publicKeyBase64 = Buffer.from(pair.publicKey).toString('base64');
    res.json({ publicKeyBase64 });
});

app.post('/api/rsa/encrypt', (req, res) => {
    try {
        const { publicKey, plainText } = req.body || {};
        if (!publicKey || typeof plainText !== 'string')
            return res.status(400).json({ message: 'Body: { publicKey, plainText }' });
        res.json({ cipherTextBase64: RSA.encrypt(publicKey, plainText) });
    } catch {
        res.status(400).json({ message: 'RSA encrypt failed' });
    }
});

app.get('/api/crypto-keys/rsa-private-key/:id', (req, res) => {
    try {
        const id = Number(String(req.params.id).trim());
        if (!Number.isFinite(id)) return res.status(400).json({ message: 'Bad id' });

        const pair = rsaKeysStore.find(k => k.id === id);
        if (!pair) return res.status(404).json({ message: 'Key not found' });

        res.json({ privateKey: pair.privateKey });
    } catch (e) {
        console.error('get private failed:', e);
        res.status(500).json({ message: 'Internal error' });
    }
});

app.post('/api/rsa/decrypt', (req, res) => {
    try {
        const { privateKey, cipherTextBase64 } = req.body || {};
        if (!privateKey || !cipherTextBase64)
            return res.status(400).json({ message: 'Body: { privateKey, cipherTextBase64 }' });
        res.json({ plainText: RSA.decrypt(privateKey, cipherTextBase64) });
    } catch {
        res.status(400).json({ message: 'RSA decrypt failed' });
    }
});

app.post('/api/aes/generate', (req, res) => {
    try { res.status(201).json({ key: AES.generateSecretKey() }); }
    catch { res.status(400).json({ message: 'AES key generation failed' }); }
});

app.post('/api/aes/encrypt', (req, res) => {
    try {
        const { key, plainText } = req.body || {};
        if (!key || typeof plainText !== 'string')
            return res.status(400).json({ message: 'Body: { key:{key,iv}, plainText }' });
        res.json({ cipherTextBase64: AES.encrypt(key, plainText) });
    } catch {
        res.status(400).json({ message: 'AES encrypt failed' });
    }
});

app.post('/api/aes/decrypt', (req, res) => {
    try {
        const { key, cipherTextBase64 } = req.body || {};
        if (!key || !cipherTextBase64)
            return res.status(400).json({ message: 'Body: { key:{key,iv}, cipherTextBase64 }' });
        res.json({ plainText: AES.decrypt(key, cipherTextBase64) });
    } catch {
        res.status(400).json({ message: 'AES decrypt failed' });
    }
});
const sessionStore = require('./secure/session.store');

app.get('/api/sessions', (req, res) => {
    res.json(sessionStore.list());
});


const swaggerDefinition = {
    openapi: '3.0.3',
    info: { title: 'Employees Crypto API', version: '1.0.0' },
    servers: [{ url: `http://localhost:${PORT}` }],
    components: {
        schemas: {
            Employee: {
                type: 'object',
                properties: {
                    id: { type: 'integer' },
                    firstName: { type: 'string' },
                    lastName: { type: 'string' },
                    age: { type: 'integer' }
                }
            },
            AesKey: {
                type: 'object',
                properties: {
                    key: { type: 'string', description: 'base64(32 bytes)' },
                    iv: { type: 'string', description: 'base64(16 bytes)' }
                },
                required: ['key', 'iv']
            }
        },
        parameters: {
            IdParam: {
                name: 'id',
                in: 'path',
                required: true,
                schema: { type: 'integer' }
            }
        }
    },
    paths: {
        '/employees': {
            get: { summary: 'List employees', responses: { 200: { description: 'OK' } } },
            post: { summary: 'Create employee', responses: { 200: { description: 'OK' }, 400: { description: 'Bad' } } }
        },
        '/employees/{id}': {
            get: {
                summary: 'Get employee',
                parameters: [{ $ref: '#/components/parameters/IdParam' }],
                responses: { 200: {}, 404: {} }
            },
            put: {
                summary: 'Update employee',
                parameters: [{ $ref: '#/components/parameters/IdParam' }],
                responses: { 200: {}, 404: {} }
            },
            delete: {
                summary: 'Delete employee',
                parameters: [{ $ref: '#/components/parameters/IdParam' }],
                responses: { 200: {}, 404: {} }
            }
        },
        '/api/crypto-keys/generate/rsa-keys': {
            post: {
                summary: 'Generate RSA key pair (store & return id)',
                responses: {
                    201: { description: 'Created (returns {id})' },
                    400: { description: 'Error' }
                }
            }
        },
        '/api/crypto-keys/rsa-public-key/{id}': {
            get: {
                summary: 'Get RSA public key by id (base64)',
                parameters: [{ $ref: '#/components/parameters/IdParam' }],
                responses: {
                    200: { description: 'OK (returns {publicKeyBase64})' },
                    404: { description: 'Not found' }
                }
            }
        },
        '/api/rsa/encrypt': {
            post: {
                summary: 'RSA encrypt (OAEP-SHA256, Base64)',
                requestBody: {
                    required: true,
                    content: {
                        'application/json': {
                            schema: {
                                type: 'object',
                                properties: {
                                    publicKey: { type: 'string' },
                                    plainText: { type: 'string' }
                                },
                                required: ['publicKey', 'plainText']
                            }
                        }
                    }
                },
                responses: { 200: { description: 'OK' }, 400: { description: 'Error' } }
            }
        },
        '/api/rsa/decrypt': {
            post: {
                summary: 'RSA decrypt (OAEP-SHA256, Base64)',
                requestBody: {
                    required: true,
                    content: {
                        'application/json': {
                            schema: {
                                type: 'object',
                                properties: {
                                    privateKey: { type: 'string' },
                                    cipherTextBase64: { type: 'string' }
                                },
                                required: ['privateKey', 'cipherTextBase64']
                            }
                        }
                    }
                },
                responses: { 200: { description: 'OK' }, 400: { description: 'Error' } }
            }
        },
        '/api/aes/generate': {
            post: {
                summary: 'Generate AES key/iv (Base64)',
                responses: { 201: { description: 'Created' }, 400: { description: 'Error' } }
            }
        },
        '/api/aes/encrypt': {
            post: {
                summary: 'AES-256-CBC encrypt (Base64)',
                requestBody: {
                    required: true,
                    content: {
                        'application/json': {
                            schema: {
                                type: 'object',
                                properties: {
                                    key: { $ref: '#/components/schemas/AesKey' },
                                    plainText: { type: 'string' }
                                },
                                required: ['key', 'plainText']
                            }
                        }
                    }
                },
                responses: { 200: { description: 'OK' }, 400: { description: 'Error' } }
            }
        },
        '/api/aes/decrypt': {
            post: {
                summary: 'AES-256-CBC decrypt (Base64)',
                requestBody: {
                    required: true,
                    content: {
                        'application/json': {
                            schema: {
                                type: 'object',
                                properties: {
                                    key: { $ref: '#/components/schemas/AesKey' },
                                    cipherTextBase64: { type: 'string' }
                                },
                                required: ['key', 'cipherTextBase64']
                            }
                        }
                    }
                },
                responses: { 200: { description: 'OK' }, 400: { description: 'Error' } }
            }
        }
    }
};

const swaggerSpec = swaggerJsdoc({ definition: swaggerDefinition, apis: [] });
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerSpec));

app.listen(PORT, () => console.log(`http://localhost:${PORT} | Swagger: /api-docs`));
