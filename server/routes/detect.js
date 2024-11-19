const express = require('express');
const router = express.Router();

const outputDeviceId = 'cf414906-b98b-4c42-bcac-c64987501bd9';

module.exports = (broadcastMessage) => {
  router.post('/', (req, res) => {
    const { pose, accuracy, deviceId } = req.body; // Expect JSON {pose: int {0,6}, accuracy: int {0,100}, deviceId: str}

    console.log(`Sending pose: ${pose}, with accuracy: ${accuracy} for device with id: ${deviceId}`);

    if (deviceId === outputDeviceId && (pose >= 0 && pose < 7) && (accuracy >= 0 && accuracy <= 100)) {
      // Broadcast the message to WebSocket clients
      const message = {
        pose,
        accuracy,
        deviceId,
        timestamp: new Date().toISOString(),
      };
      broadcastMessage(message);

      // Respond to the HTTP client
      res.status(200).send({ message: `Sent pose: ${pose}, with accuracy: ${accuracy} for device with id: ${deviceId}` });
    } else {
      res.status(400).send({ error: 'Invalid pose id, accuracy, or device id' });
    }
  });

  return router;
};
