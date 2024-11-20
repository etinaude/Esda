const express = require('express');
const fs = require('fs'); // For file operations
const path = require('path');
const router = express.Router();

const trainingLogPath = path.join(__dirname, 'trainingLog.json'); // File path for training log

router.post('/', (req, res) => {
  const { deviceId, flexVals, /*label*/ } = req.body; // Expect JSON {deviceId: Str, flexVals: [int,int,int,int,int], label: int}

  console.log(`Received training values: ${flexVals}`);

    let parsedFlexVals;
    try {
    parsedFlexVals = typeof flexVals === 'string' ? JSON.parse(flexVals) : flexVals;
    } catch (error) {
    console.error('Error parsing flexVals:', error.message);
    return res.status(400).send({ error: 'Invalid format for flexVals. It should be a valid JSON array.' });
    }

    // Validate parsedFlexVals and label
    if (
        Array.isArray(parsedFlexVals) &&
        parsedFlexVals.length === 5 &&
        parsedFlexVals.every((val) => Number.isInteger(val))
    ) {
        // Append the data to trainingLog.json
        appendToTrainingLog({
        flexVals: parsedFlexVals,
        label: 0, //label,
        timestamp: new Date().toISOString(),
        });
    
        // Respond to the HTTP client
        res.status(200).send({ message: `Logged training data: ${parsedFlexVals}` });
    } else {
        res.status(400).send({ error: 'Invalid training data. Ensure flexVals is an array of 5 integers and label is between 0 and 6.' });
    }
});

// Function to append data to the training log
function appendToTrainingLog(data) {
  try {
    let trainingLog = [];

    // Check if the file exists
    if (fs.existsSync(trainingLogPath)) {
      // Read existing data from the file
      const fileContent = fs.readFileSync(trainingLogPath, 'utf-8');
      trainingLog = JSON.parse(fileContent);
    }

    // Add the new data
    trainingLog.push(data);

    // Write updated data back to the file
    fs.writeFileSync(trainingLogPath, JSON.stringify(trainingLog, null, 2), 'utf-8');
    console.log('Training data logged successfully.');
  } catch (error) {
    console.error('Error logging training data:', error.message);
  }
}

module.exports = router;
