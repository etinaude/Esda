#include <Arduino.h>
#include <ArduTFLite.h>
#include "model.h"

#define INPUT_SIZE 5
#define OUTPUT_SIZE 4
#define BUFFER_SIZE 5

// mapping
String labels[OUTPUT_SIZE] = {"Normal", "Back", "Fist", "Finger"};

constexpr int kTensorArenaSize = 2000;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

class TensorModel {
  public:
    bool success = false;

    int indexBuffer[BUFFER_SIZE];
    float confidenceBuffer[BUFFER_SIZE];
    int bufferIndex = 0;

    int rollingMode = 0;
    float rollingConfidence = 0;
    String pose = "";

    TensorModel(){
      if (!modelInit(model, tensor_arena, kTensorArenaSize)){
        Serial.println("Model initialization failed!");
        while(true);
      }
      Serial.println("Model initialization done.");
    }

    void add(int index, float confidence){
      if (!success) return;

      indexBuffer[bufferIndex] = index;
      confidenceBuffer[bufferIndex] = confidence;

      bufferIndex++;
      if(bufferIndex >= BUFFER_SIZE){
        bufferIndex = 0;
      }
    }

    // gets rolling mode and rolling ave confidence and most likely pose
    void getRollingStats(){
      int counts[OUTPUT_SIZE] = {0, 0, 0, 0};
      float confidenceSum = 0;

      for (int i = 0; i < BUFFER_SIZE; i++) {
        counts[indexBuffer[i]]++;
        confidenceSum += confidenceBuffer[i];
      }

      rollingMode = 0;
      for (int i = 0; i < OUTPUT_SIZE; i++) {
        if (counts[i] > counts[rollingMode]) {
          rollingMode = i;
        }
      }


      rollingConfidence = confidenceSum / BUFFER_SIZE;
      pose = labels[rollingMode];
    }

    void mlPredict(int vals[]) {
      success = false;

      for (int i = 0; i < INPUT_SIZE; i++) {
        modelSetInput(vals[i], i);
      }

      if (!modelRunInference() || isnan(modelGetOutput(0))) {
        Serial.println("Run Inference Failed!");
        return;
      }

      success = true;
      int maxIndex = 0;

      for (int i = 0; i < OUTPUT_SIZE; i++) {
        if (modelGetOutput(i) > modelGetOutput(maxIndex)) {
          maxIndex = i;
        }
      }
      add(maxIndex, modelGetOutput(maxIndex));
      getRollingStats();
    }

    void displayResults(){
      if (!success) {
        Serial.println("Failed to get results");
        return;
      }

      Serial.print("Pose: ");
      Serial.print(pose);
      Serial.print("\tConfidence: ");
      Serial.print(rollingConfidence);
      Serial.println("\n");
    }
};


TensorModel tensorModel;