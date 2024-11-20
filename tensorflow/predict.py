import numpy as np
import tensorflow as tf

# Load the TFLite model
interpreter = tf.lite.Interpreter(model_path="model.tflite")
interpreter.allocate_tensors()

dummyData = [[6244, 0, 0, 0, 0],
             [8191, 0, 0, 0, 0]
             ]

input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

for i in dummyData:
    input_data = np.array([i], dtype=np.float32)
    interpreter.set_tensor(interpreter.get_input_details()[0]['index'], input_data)

    interpreter.invoke()

output_data = interpreter.get_tensor(output_details[0]['index'])
print("Output:", output_data[0][0])