const express = require('express');
const mysql = require('mysql');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
app.use(bodyParser.json());
app.use(cors());

const connection = mysql.createConnection({
    host: '172.25.161.138',
    user: 'papu',
    password: '',
    database: 'client_db'
});

connection.connect((err) => {
    if (err) {
        console.error('Error connecting: ' + err.stack);
        return;
    }
    console.log('Connected as id ' + connection.threadId);
});

// Endpoint para enviar solicitud de acceso
app.post('/access-request', (req, res) => {
    const { user_id, channel_id } = req.body;

    const query = 'INSERT INTO channel_users (user_id, channel_id, status) VALUES (?, ?, 2)'; // 2 indica 'Solicitud'
    connection.query(query, [user_id, channel_id], (error, results) => {
        if (error) {
            console.error('Error inserting access request:', error);
            res.status(500).send(error);
        } else {
            res.status(201).json({ id: results.insertId, user_id, channel_id, status: 'pending' });
        }
    });
});

// Endpoint para aceptar solicitud de acceso
app.post('/accept-request', (req, res) => {
    const { user_id, channel_id } = req.body;

    const query = 'UPDATE channel_users SET status = 1 WHERE user_id = ? AND channel_id = ?'; // 1 indica 'Usuario'
    connection.query(query, [user_id, channel_id], (error, results) => {
        if (error) {
            console.error('Error updating access request:', error);
            res.status(500).send(error);
        } else {
            res.status(200).json({ user_id, channel_id, status: 'accepted' });
        }
    });
});

// Endpoint para obtener los canales
app.get('/channels', (req, res) => {
    const query = 'SELECT * FROM channels';
    connection.query(query, (error, results) => {
        if (error) {
            console.error('Error fetching channels:', error);
            res.status(500).send(error);
        } else {
            res.status(200).json(results);
        }
    });
});

// Endpoint para crear un nuevo canal
app.post('/channels', (req, res) => {
    const { name, administrator_id } = req.body;

    // Log the received data for debugging
    console.log('Received request to create channel:', { name, administrator_id });

    const query = 'INSERT INTO channels (name, administrator_id) VALUES (?, ?)';
    connection.query(query, [name, administrator_id], (error, results) => {
        if (error) {
            console.error('Error creating channel:', error);
            res.status(500).send({
                message: 'Error creating channel',
                error: error.message // Send the error message in the response for debugging
            });
        } else {
            res.status(201).json({ id: results.insertId, name, administrator_id });
        }
    });
});

// Endpoint para obtener mensajes de un canal
app.get('/channels/:id/messages', (req, res) => {
    const channelId = req.params.id;
    const query = 'SELECT * FROM messages WHERE channel_id = ?';
    connection.query(query, [channelId], (error, results) => {
        if (error) {
            console.error('Error fetching messages:', error);
            res.status(500).send(error);
        } else {
            res.status(200).json(results);
        }
    });
});

// Endpoint para enviar un mensaje
app.post('/messages', (req, res) => {
    const { msg, user_id, channel_id } = req.body;

    const query = 'INSERT INTO messages (msg, user_id, channel_id) VALUES (?, ?, ?)';
    connection.query(query, [msg, user_id, channel_id], (error, results) => {
        if (error) {
            console.error('Error sending message:', error);
            res.status(500).send(error);
        } else {
            res.status(201).json({ id: results.insertId, msg, user_id, channel_id });
        }
    });
});

const PORT = process.env.PORT || 3002;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
