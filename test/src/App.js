// TcpClientComponent.js
import React, { useState } from 'react';

const TcpClientComponent = () => {
  const [serverResponse, setServerResponse] = useState('');
  const ws = new WebSocket('ws://localhost:3001');

  ws.onopen = () => {
    console.log('Connected to Node.js server');
  };

  ws.onmessage = (event) => {
    console.log('Received data from Node.js server:', event.data);
    setServerResponse(event.data);
  };

  ws.onclose = () => {
    console.log('Connection closed');
  };

  const sendMessage = (message) => {
    console.log('Sending message to Node.js server:', message);
    ws.send(message);
  };

  return (
    <div>
      <button onClick={() => sendMessage('Hello from React client')}>Send Message</button>
      <p>Server Response: {serverResponse}</p>
    </div>
  );
};

export default TcpClientComponent;