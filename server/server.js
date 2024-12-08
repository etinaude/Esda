const express = require('express');
const bodyParser = require('body-parser');
const WebSocket = require('ws');
const statusRoutes = require('./routes/status');
const detectRoutes = require('./routes/detect');

const cancelRoutes = require('./routes/cancel');
const trainRoutes = require('./routes/train');

const app = express();
app.use(bodyParser.json({ type: 'application/json', limit: '10mb', strict: true }));

const PORT = 3000;

// Create WebSocket server
const wss = new WebSocket.Server({ noServer: true });

// List of connected WebSocket clients
let clients = [];

// Handle WebSocket connections
wss.on('connection', (ws) => {
  console.log('New WebSocket client connected');
  clients.push(ws);

  // Remove the client when disconnected
  ws.on('close', (code, reason) => {
    console.log(`WebSocket client disconnected. Code: ${code}, Reason: ${reason}`);
    // Only remove if the client is actually disconnected
    if (ws.readyState === WebSocket.CLOSED) {
      clients = clients.filter((client) => client !== ws);
    }
  });

  ws.on('error', (error) => {
    console.error('WebSocket error:', error);
  });

  ws.on('message', (message) => {
    const data = JSON.parse(message);

    flexVals = JSON.parse(data.flexVals);
    data.flexVals = flexVals;

    console.log('Received message:', data);

    // Broadcast the message to all clients
    broadcastMessage(data);
  });
});

// Broadcast a message to all connected clients
function broadcastMessage(message) {
  clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify(message));
    }
  });
}

// Attach the WebSocket server to the Express server
const server = app.listen(PORT, () => {
  console.log(`Server is listening on port ${PORT}`);
});

server.on('upgrade', (request, socket, head) => {
  wss.handleUpgrade(request, socket, head, (ws) => {
    wss.emit('connection', ws, request);
  });
});

// Pass WebSocket broadcast function to routes
app.use('/status', statusRoutes(broadcastMessage));
app.use('/detect', detectRoutes(broadcastMessage));
app.use('/cancel', cancelRoutes(broadcastMessage));
app.use('/train', trainRoutes);

// Default route to check server
app.get('/', (req, res) => {
  res.send('Server is running!');
});
