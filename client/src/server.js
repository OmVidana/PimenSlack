const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const path = require('path');
const cors = require('cors'); // Add this line

const app = express();
const port = 3001;

app.use(cors()); // Add this line to enable CORS
app.use(bodyParser.json());

// Endpoint to get groups data
app.get('/groups', (req, res) => {
  fs.readFile(path.join(__dirname, 'groups.json'), 'utf8', (err, data) => {
    if (err) {
      res.status(500).send('Error reading groups data');
    } else {
      res.send(data);
    }
  });
});

// Endpoint to save new group
app.post('/groups', (req, res) => {
  const newGroup = req.body;
  fs.readFile(path.join(__dirname, 'groups.json'), 'utf8', (err, data) => {
    if (err) {
      res.status(500).send('Error reading groups data');
    } else {
      const groups = JSON.parse(data);
      groups.push(newGroup);
      fs.writeFile(path.join(__dirname, 'groups.json'), JSON.stringify(groups, null, 2), (err) => {
        if (err) {
          res.status(500).send('Error writing groups data');
        } else {
          res.status(200).send('Group added successfully');
        }
      });
    }
  });
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});
