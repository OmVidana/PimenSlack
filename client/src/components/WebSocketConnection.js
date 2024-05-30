import React, { useEffect, useState, useContext, createContext, useCallback } from 'react';

const WebSocketContext = createContext(null);

export const WebSocketProvider = ({ children }) => {
    const [ws, setWs] = useState(null);
    const [listeners, setListeners] = useState([]);
    const [encryptionKeys, setEncryptionKeys] = useState(null); // Variable para almacenar las claves de cifrado

    useEffect(() => {
        const socket = new WebSocket('ws://localhost:3001');

        socket.onopen = () => {
            console.log('Connected to C server');
            setWs(socket);
            // Enviar el mensaje 'getKeys' al abrir la conexión
            socket.send(JSON.stringify({ action: 'get_keys', data: {} }));
        };

        socket.onmessage = (event) => {
            console.log('Received:', event.data);
            const message = JSON.parse(event.data);
            if (message.action === 'get_keys') {
                setEncryptionKeys({ n: message.data.n, e: message.data.e });
            }
            listeners.forEach(listener => listener(message));
        };

        socket.onerror = (error) => {
            console.error('WebSocket error:', error);
        };

        socket.onclose = () => {
            console.log('WebSocket connection closed');
        };

        return () => {
            if (socket.readyState === WebSocket.OPEN || socket.readyState === WebSocket.CONNECTING) {
                socket.close();
            }
        };
    }, [listeners]);

    const sendMessage = useCallback((data) => {
        if (ws && ws.readyState === WebSocket.OPEN) {
            ws.send(JSON.stringify(data));
        } else {
            console.log('WebSocket connection not open');
        }
    }, [ws]);

    const subscribe = useCallback((listener) => {
        setListeners(prev => [...prev, listener]);
    }, []);

    const unsubscribe = useCallback((listener) => {
        setListeners(prev => prev.filter(l => l !== listener));
    }, []);




    return (
        <WebSocketContext.Provider value={{ sendMessage, subscribe, unsubscribe, encryptionKeys }}>
            {children}
        </WebSocketContext.Provider>
    );
};

export const useWebSocket = () => useContext(WebSocketContext);