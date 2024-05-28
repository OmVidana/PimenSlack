import React, { useEffect, useState } from 'react';

const App = () => {
    const [message, setMessage] = useState('');
    const [response, setResponse] = useState('');

    useEffect(() => {
        const ws = new WebSocket('ws://localhost:3001');

        ws.onopen = () => {
            console.log('Connected to Node.js server');
            ws.send('Hello from React client');
        };

        ws.onmessage = (event) => {
            console.log('Received:', event.data);
            setResponse(event.data);
        };

        ws.onerror = (error) => {
            console.error('WebSocket error:', error);
        };

        ws.onclose = () => {
            console.log('WebSocket connection closed');
        };

        return () => {
            ws.close();
        };
    }, []);

    return (
        <div>
            <h1>React WebSocket Client</h1>
            <p>Message from C server: {response}</p>
            <input
                type="text"
                value={message}
                onChange={(e) => setMessage(e.target.value)}
            />
            <button
                onClick={() => {
                    const ws = new WebSocket('ws://localhost:3001');
                    ws.onopen = () => {
                        ws.send(message);
                    };
                }}
            >
                Send Message
            </button>
        </div>
    );
};

export default App;