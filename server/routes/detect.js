const express = require('express');
const router = express.Router();
const twilio = require('twilio');

// Twilio credentials
const accountSid = 'ACe4f323d7b009cd55d0c2c3b1ba80abfd';
const authToken = '7a052a963bca51dc5fb4727ab4c4f109';
const client = twilio(accountSid, authToken);

const outputDeviceId = 'cf414906-b98b-4c42-bcac-c64987501bd9';
const notificationPhone = '+1234567890'; // Replace with the actual recipient phone number
const twilioPhone = '+1 575 219 4205'; // Replace with your Twilio phone number

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

      // Send SMS via Twilio
      client.messages
        .create({
          body: `Check in on XXXX, they may be having a seizure. Pose: ${pose}, Accuracy: ${accuracy}.`,
          from: twilioPhone,
          to: notificationPhone,
        })
        .then((msg) => {
          console.log('SMS sent:', msg.sid);
        })
        .catch((err) => {
          console.error('Error sending SMS:', err);
        });

      // Respond to the HTTP client
      res.status(200).send({
        message: `Sent pose: ${pose}, with accuracy: ${accuracy} for device with id: ${deviceId}. SMS notification sent.`,
      });
    } else {
      res.status(400).send({ error: 'Invalid pose id, accuracy, or device id' });
    }
  });

  return router;
};
