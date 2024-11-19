const express = require('express');
const router = express.Router();
const twilio = require('twilio');

// Twilio credentials
const accountSid = 'ACe4f323d7b009cd55d0c2c3b1ba80abfd';
const authToken = '7a052a963bca51dc5fb4727ab4c4f109';
const client = twilio(accountSid, authToken);

// Twilio notification endpoint
router.post('/', (req, res) => {
    console.log('Request body:', req.body); // Debug
  
    const { phone, message } = req.body; // Extract phone and message
  
    if (!phone || !message) {
      return res.status(400).send({ error: 'Phone and message are required.' });
    }
  
    client.messages
      .create({
        body: message,
        from: '+1 575 219 4205',
        to: phone,
      })
      .then((msg) => {
        console.log('Message sent:', msg.sid);
        res.status(200).send({ message: 'Notification sent', sid: msg.sid });
      })
      .catch((err) => {
        console.error('Error sending notification:', err);
        res.status(500).send({ error: 'Failed to send notification' });
      });
});

module.exports = router; // Export the router
  