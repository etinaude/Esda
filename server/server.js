const express = require('express');
const bodyParser = require('body-parser');
const WebSocket = require('ws');
const statusRoutes = require('./routes/status');
const detectRoutes = require('./routes/detect');
const cancelRoutes = require('./routes/cancel');

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
  ws.on('close', () => {
    console.log('WebSocket client disconnected');
    clients = clients.filter((client) => client !== ws);
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

// Default route to check server
app.get('/', (req, res) => {
  res.send('Server is running!');
});
