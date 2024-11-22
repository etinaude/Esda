#include <Arduino.h>
#include <ArduTFLite.h>
#include "model.h"

#define INPUT_SIZE 5
#define OUTPUT_SIZE 2


constexpr int kTensorArenaSize = 2000;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

struct TensorModel {
  float results[OUTPUT_SIZE];
  int maxIndex;
  float maxVal;
  bool success = false;
};

void initML(){
  if (!modelInit(model, tensor_arena, kTensorArenaSize)){
    Serial.println("Model initialization failed!");
    while(true);
  }
  Serial.println("Model initialization done.");
}

TensorModel mlPredict(int vals[]) {
  TensorModel tensorModel;

  for (int i = 0; i < 5; i++) {
    modelSetInput(vals[i], i);
  }

  if (!modelRunInference()) {
    Serial.println("RunInference Failed!");
    return tensorModel;
  }

  tensorModel.success = true;
  float max = tensorModel.results[0];
  int maxIndex = 0;

  for (int i = 0; i < OUTPUT_SIZE; i++) {
    tensorModel.results[i] = modelGetOutput(i);

    if (tensorModel.results[i] > max) {
      tensorModel.maxVal = tensorModel.results[i];
      tensorModel.maxIndex = i;
    }
  }

  return tensorModel;
}

void displayResults(TensorModel tensorModel){
  if (!tensorModel.success) {
    Serial.println("Failed to get results");
    return;
  }

  Serial.print("{ ");
  for (int i = 0; i < OUTPUT_SIZE; i++) {
    Serial.print(tensorModel.results[i]);
    Serial.print(" ");
  }
  Serial.print("}");

  Serial.println();
  Serial.print("Max Index: ");
  Serial.println(tensorModel.maxIndex);
  Serial.print("Max Value: ");
  Serial.println(tensorModel.maxVal);
  Serial.println("\n");
}