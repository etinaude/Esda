const express = require('express');
const router = express.Router();

const outputDeviceId = 'cf414906-b98b-4c42-bcac-c64987501bd9';

module.exports = (broadcastMessage) => {
  router.post('/', (req, res) => {
    const { deviceId } = req.body; // Expect JSON {deviceId: str}

    console.log(`Sending alert cancel request to device with id: ${deviceId}`);

    if (deviceId === outputDeviceId) {
      // Broadcast the message to WebSocket clients
      const message = {
        event: 'cancel',
        deviceId,
        timestamp: new Date().toISOString(),
      };
      broadcastMessage(message);

      // Respond to the HTTP client
      res.status(200).send({ message: `${deviceId} cancel command sent.` });
    } else {
      res.status(400).send({ error: 'Invalid device id' });
    }
  });

  return router;
};
