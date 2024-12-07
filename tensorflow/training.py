import tensorflow as tf
import numpy as np
import pandas as pd
from model import normalizer, optimizer, early_stopping, reduce_lr
from colorama import Fore, Style


OUTPUT_CLASSES = 4

data = pd.read_json('./data/train.json')

train = data.sample(frac=0.8, random_state=200)
val = data.drop(train.index)

train = train[train['label'] >= 0]


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
        tf.keras.layers.Dense(32, activation='relu'),
        tf.keras.layers.Dense(OUTPUT_CLASSES, activation='softmax')
    ])
    model.compile(optimizer=optimizer,
                  loss='sparse_categorical_crossentropy', metrics=['accuracy'])

    model.fit(x_train, y_train, validation_data=(
        x_val, y_val), epochs=200, batch_size=32, callbacks=[early_stopping, reduce_lr])

    return model


def print_model(model):
    model.summary()
    model.evaluate(x_val, y_val)

    y_pred = model.predict(x_val)
    y_pred = np.argmax(y_pred, axis=1)
    confusion_matrix = tf.math.confusion_matrix(y_val, y_pred)

    print('\n\n~~~EVAL~~~')
    print('\nConfusion matrix:')
    print(confusion_matrix)

    print('\n\n')
    # accuracy by class
    for i in range(OUTPUT_CLASSES):
        accuracy = np.mean(y_val[y_val == i] == y_pred[y_val == i]) * 100
        color = Fore.YELLOW if accuracy < 90 else Fore.GREEN
        color = Fore.RED if accuracy < 80 else color

        print(f'Accuracy for class {i}: {color}{
              accuracy:.2f}%{Style.RESET_ALL}')


def export(model):
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    tflite_model = converter.convert()
    with open('model.tflite', 'wb') as f:
        f.write(tflite_model)

    # converter.optimizations = [tf.lite.Optimize.DEFAULT]
    # tflite_quantized_model = converter.convert()

    # with open('model_quantized.tflite', 'wb') as f:
    #     f.write(tflite_quantized_model)


model = train_model()
export(model)
print_model(model)
