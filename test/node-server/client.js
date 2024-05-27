// node_server.js
const WebSocket = require('ws');
const net = require('net');

const wss = new WebSocket.Server({ port: 3001 });

const cServerHost = '127.0.0.1';
const cServerPort = 5000;

wss.on('connection', (ws) => {
  console.log('Client connected to Node.js server');

  const cServer = net.createConnection({ host: cServerHost, port: cServerPort });

  cServer.on('connect', () => {
    console.log('Connected to C server');
  });

  cServer.on('data', (data) => {
    console.log('Received data from C server:', data.toString());
    ws.send(data.toString());
  });

  ws.on('message', (message) => {
    console.log('Received message from client:', message);
    cServer.write(message);
  });

  ws.on('close', () => {
    console.log('Client disconnected from Node.js server');
    cServer.end();
  });
});