const express = require('express');
const router = express.Router();

const outputDeviceId = 'cf414906-b98b-4c42-bcac-c64987501bd9';

module.exports = (broadcastMessage) => {
  router.post('/', (req, res) => {
    const { status, deviceId } = req.body; // Expect JSON {status: 'ON'|'OFF'|'LowBattery', deviceId: str}

    console.log(`Setting status: ${status} for device with id: ${deviceId}`);

    if (deviceId === outputDeviceId && (status === 'ON' || status === 'OFF' || status === 'LowBattery')) {
      // Broadcast the message to WebSocket clients
      const message = {
        status,
        deviceId,
        timestamp: new Date().toISOString(),
      };
      broadcastMessage(message);

      // Respond to the HTTP client
      res.status(200).send({ message: `Set status: ${status} for device with id: ${deviceId}` });
    } else {
      res.status(400).send({ error: 'Invalid status or device id' });
    }
  });

  return router;
};
