const express = require('express');
const fs = require('fs');
const path = require('path');
const router = express.Router();

const trainingLogPath = path.join(__dirname, 'trainingLog.json');

const LABEL = 3

router.post('/', (req, res) => {
  const { deviceId, flexVals } = req.body;

  console.log(`Received training values: ${flexVals}`);

  let parsedFlexVals;
  try {
    parsedFlexVals = typeof flexVals === 'string' ? JSON.parse(flexVals) : flexVals;
  } catch (error) {
    console.error('Error parsing flexVals:', error.message);
    return res.status(400).send({ error: 'Invalid format for flexVals. It should be a valid JSON array.' });
  }

  if (
    Array.isArray(parsedFlexVals) &&
    parsedFlexVals.length === 5 &&
    parsedFlexVals.every((val) => Number.isInteger(val))
  ) {
    appendToTrainingLog({
      flexVals: parsedFlexVals,
      label: LABEL,
      timestamp: new Date().toISOString(),
    });

    res.status(200).send({ message: `Logged training data: ${parsedFlexVals}` });
  } else {
    res.status(400).send({ error: 'Invalid training data. Ensure flexVals is an array of 5 integers' });
  }
});

function appendToTrainingLog(data) {
  try {
    let trainingLog = [];

    if (fs.existsSync(trainingLogPath)) {
      const fileContent = fs.readFileSync(trainingLogPath, 'utf-8');
      trainingLog = JSON.parse(fileContent);
    }

    trainingLog.push(data);

    fs.writeFileSync(trainingLogPath, JSON.stringify(trainingLog, null, 2), 'utf-8');
    console.log('Training data logged successfully.');

  } catch (error) {
    console.error('Error logging training data:', error.message);
  }
}

module.exports = router;
