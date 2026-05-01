const http = require('http');
const fs = require('fs');
const path = require('path');
const WebSocket = require('ws');

const PORT = process.env.PORT || 8080;

// ---- HTTP server (serves controller page) ----
const server = http.createServer((req, res) => {
  console.log("Request:", req.url);
  
  // Add CORS headers
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

  if (req.url === '/') {
    fs.readFile('./controller.html', (err, data) => {
      if (err) {
        res.writeHead(500);
        return res.end("Error loading controller");
      }
      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });

  } else if (req.url === '/chest') {
    fs.readFile('./player.html', (err, data) => {
      if (err) {
        console.error(err);
        res.writeHead(500);
        return res.end("Error loading player");
      }
      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });

  } else if (req.url.startsWith('/audio/')) {
    const filePath = path.join(__dirname, req.url);
    fs.readFile(filePath, (err, data) => {
      if (err) {
        res.writeHead(404);
        return res.end("Audio not found");
      }
      res.writeHead(200, { 'Content-Type': 'audio/mpeg' });
      res.end(data);
    });

  } else if (req.url === '/favicon.ico') {
    res.writeHead(200, { 'Content-Type': 'image/x-icon' });
    res.end('');
  } else {
    res.writeHead(404);
    res.end("Not found");
  }
});

// ---- WebSocket server ----
const wss = new WebSocket.Server({ server });

wss.on('connection', (ws) => {
  console.log("Client connected");

  ws.on('message', (msg) => {
    // broadcast to all clients (player phones)
    wss.clients.forEach(client => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(msg.toString());
      }
    });
  });

  ws.on('close', () => console.log("Client disconnected"));
});

server.listen(PORT, () => {
  console.log(`Server running: http://localhost:${PORT}`);
});