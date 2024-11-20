import tensorflow as tf
import numpy as np
import pandas as pd

# Data format
# [
#   {
#     "label": 1,
#     "flexVals": [0, 0, 0, 0, 0]
#   },...
# ]


data = pd.read_json('./data/test.json')
data = data.sample(frac=1).reset_index(drop=True)

train_size = int(0.8 * len(data))
train_data = data[:train_size]
val_data = data[train_size:]

x_train = np.array(train_data['flexVals'].tolist())
y_train = np.array(train_data['label'].tolist())

x_val = np.array(val_data['flexVals'].tolist())
y_val = np.array(val_data['label'].tolist())


model = tf.keras.Sequential([
    tf.keras.layers.Dense(16, activation='relu', input_shape=(5,)),  # Input layer
    tf.keras.layers.Dense(8, activation='relu'),                     # Hidden layer
    tf.keras.layers.Dense(1, activation='sigmoid')                   # Output layer (adjust activation for your problem)
])
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])  # Adjust loss based on the problem type

history = model.fit(x_train, y_train, validation_data=(x_val, y_val), epochs=50, batch_size=32)


converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

# Save the model
with open('model.tflite', 'wb') as f:
    f.write(tflite_model)

converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_quantized_model = converter.convert()

# Save the quantized model
with open('model_quantized.tflite', 'wb') as f:
    f.write(tflite_quantized_model)