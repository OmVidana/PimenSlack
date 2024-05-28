const net = require('net');

// C server Docker container IP and port
const cServerHost = '127.0.0.1'; // Assuming Docker port is exposed to localhost
const cServerPort = 5000;

// Node.js server port to listen for incoming client connections
const nodeServerPort = 3001;

const server = net.createServer((clientSocket) => {
    console.log('Client connected');

    // Connect to C server
    const cServerSocket = new net.Socket();
    cServerSocket.connect(cServerPort, cServerHost, () => {
        console.log('Connected to C server');
    });

    // Forward data from client to C server
    clientSocket.on('data', (data) => {
        console.log('Data from client:', data.toString());
        cServerSocket.write(data);
    });

    // Forward data from C server to client
    cServerSocket.on('data', (data) => {
        console.log('Data from C server:', data.toString());
        clientSocket.write(data);
    });

    // Handle client disconnection
    clientSocket.on('end', () => {
        console.log('Client disconnected');
        cServerSocket.end();
    });

    // Handle C server disconnection
    cServerSocket.on('end', () => {
        console.log('C server disconnected');
        clientSocket.end();
    });

    // Error handling
    clientSocket.on('error', (err) => {
        console.error('Client socket error:', err);
    });

    cServerSocket.on('error', (err) => {
        console.error('C server socket error:', err);
    });
});

server.listen(nodeServerPort, () => {
    console.log(`Node.js server listening on port ${nodeServerPort}`);
});