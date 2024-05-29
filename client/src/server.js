const WebSocket = require('ws');
const net = require('net');

const wss = new WebSocket.Server({ port: 3001 });

const connectToCServer = (ws) => {
    const cServerHost = '127.0.0.1'; 
    const cServerPort = 5000;
    let cServerSocket = new net.Socket();

    const attemptConnection = () => {
        cServerSocket.connect(cServerPort, cServerHost, () => {
            console.log('Connected to C server');
        });
    };

    attemptConnection();

    cServerSocket.on('connect', () => {
        ws.on('message', (message) => {
            console.log('Received from client:', message);
            cServerSocket.write(message);
        });
    });

    cServerSocket.on('data', (data) => {
        const response = data.toString();
        console.log('Received from C server:', response);
        ws.send(response);
    });

    cServerSocket.on('error', (err) => {
        console.error('C server socket error:', err);
        cServerSocket.destroy();
    });

    cServerSocket.on('close', () => {
        console.log('C server socket closed, attempting to reconnect...');
        setTimeout(attemptConnection, 5000); 
    });

    ws.on('close', () => {
        console.log('WebSocket client disconnected');
        cServerSocket.end();
    });

    ws.on('error', (err) => {
        console.error('WebSocket error:', err);
        cServerSocket.end();
    });
};

wss.on('connection', (ws) => {
    console.log('WebSocket client connected');
    connectToCServer(ws);
});

console.log('Node.js WebSocket server listening on port 3001');