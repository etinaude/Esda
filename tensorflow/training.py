import tensorflow as tf
import numpy as np
import pandas as pd
from model import normalizer, optimizer, early_stopping, reduce_lr

data = pd.read_json('./data/train.json')

train = data.sample(frac=0.8, random_state=200)
val = data.drop(train.index)

x_train = np.array(train['flexVals'].tolist())
y_train = np.array(train['label'].tolist())

x_val = np.array(val['flexVals'].tolist())
y_val = np.array(val['label'].tolist())

normalizer.adapt(x_train)


def train_model():
    # Model
    model = tf.keras.Sequential([
        normalizer,
        tf.keras.layers.Dense(64, activation='relu', input_shape=(5,)),
        tf.keras.layers.Dense(32, activation='relu'),
        tf.keras.layers.Dense(2, activation='softmax')
    ])
    model.compile(optimizer=optimizer,
                  loss='sparse_categorical_crossentropy', metrics=['accuracy'])

    model.fit(x_train, y_train, validation_data=(
        x_val, y_val), epochs=100, batch_size=32, callbacks=[early_stopping, reduce_lr])

    return model


def export(model):
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    tflite_model = converter.convert()
    with open('model.tflite', 'wb') as f:
        f.write(tflite_model)

    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_quantized_model = converter.convert()

    with open('model_quantized.tflite', 'wb') as f:
        f.write(tflite_quantized_model)


model = train_model()
export(model)
