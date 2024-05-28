import React, { useEffect, useState } from 'react';

const App = () => {
    const [message, setMessage] = useState('');
    const [response, setResponse] = useState('');
    const [ws, setWs] = useState(null);

    useEffect(() => {
        const socket = new WebSocket('ws://localhost:3001');

        socket.onopen = () => {
            console.log('Connected to Node.js server');
            setWs(socket);
        };

        socket.onmessage = (event) => {
            console.log('Received:', event.data);
            setResponse(event.data);
        };

        socket.onerror = (error) => {
            console.error('WebSocket error:', error);
        };

        socket.onclose = () => {
            console.log('WebSocket connection closed');
        };

        return () => {
            socket.close();
        };
    }, []);

    const sendMessage = () => {
        if (ws && ws.readyState === WebSocket.OPEN) {
            ws.send(message);
            setMessage(''); // Clear the input field after sending
        } else {
            console.log('WebSocket connection not open');
        }
    };

    return (
        <div>
            <h1>React WebSocket Client</h1>
            <p>Message from C server: {response}</p>
            <input
                type="text"
                value={message}
                onChange={(e) => setMessage(e.target.value)}
            />
            <button onClick={sendMessage}>
                Send Message
            </button>
        </div>
    );
};

export default App;