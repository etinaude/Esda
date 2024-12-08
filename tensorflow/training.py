import tensorflow as tf
import numpy as np
import pandas as pd
from model import normalizer, optimizer, early_stopping, reduce_lr
from colorama import Fore, Style

OUTPUT_CLASSES = 4


class Training:
    training_data = None
    val_data = None
    x_train = None
    y_train = None
    x_val = None
    y_val = None
    model = None

    def __init__(self):
        self.read_data()

    def read_data(self):
        data = pd.read_json('./data/train.json')

        self.training_data = data.sample(frac=0.8, random_state=201)
        self.val_data = data.drop(self.training_data.index)

        # remove invalid labels
        self.training_data = self.training_data[self.training_data['label'] >= 0]
        self.val_data = self.val_data[self.val_data['label'] >= 0]

        # remove duplicates
        self.training_data = self.training_data.drop_duplicates(
            subset='flexVals')
        self.val_data = self.val_data.drop_duplicates(subset='flexVals')

        self.x_train = np.array(self.training_data['flexVals'].tolist())
        self.y_train = np.array(self.training_data['label'].tolist())

        self.x_val = np.array(self.val_data['flexVals'].tolist())
        self.y_val = np.array(self.val_data['label'].tolist())

        normalizer.adapt(self.x_train)

        print(f'\n\n{Fore.BLUE}~~~READ COMPLETE~~~{Style.RESET_ALL}')

    def train_model(self):
        print('\n\n{Fore.BLUE}~~~TRAINING~~~{Style.RESET_ALL}')

        # Model
        self.model = tf.keras.Sequential([
            normalizer,
            tf.keras.layers.Dense(64, activation='relu', input_shape=(5,)),
            tf.keras.layers.Dense(16, activation='relu'),
            tf.keras.layers.Dense(OUTPUT_CLASSES, activation='softmax')
        ])
        self.model.compile(optimizer=optimizer,
                           loss='sparse_categorical_crossentropy', metrics=['accuracy'])

        self.model.fit(self.x_train, self.y_train, validation_data=(
            self.x_val, self.y_val), epochs=200, batch_size=32, callbacks=[early_stopping])

        print('\n\n{Fore.BLUE}~~~TRAINING COMPLETE~~~{Style.RESET_ALL}')

    def print_model(self):
        self.model.summary()
        self.model.evaluate(self.x_val, self.y_val)

        y_pred = self.model.predict(self.x_val)
        y_pred = np.argmax(y_pred, axis=1)
        confusion_matrix = tf.math.confusion_matrix(self.y_val, y_pred)

        print(f'\n\n{Fore.BLUE}~~~EVAL~~~{Style.RESET_ALL}')
        print('\nConfusion matrix:')
        print(confusion_matrix)

        print('\n\n')
        # accuracy by class
        for i in range(OUTPUT_CLASSES):
            accuracy = np.mean(
                self.y_val[self.y_val == i] == y_pred[self.y_val == i]) * 100
            color = Fore.YELLOW if accuracy < 90 else Fore.GREEN
            color = Fore.RED if accuracy < 80 else color

            print(f'Accuracy for class {i}: {color}{
                  accuracy:.2f}%{Style.RESET_ALL}')
        print(f'\n\n{Fore.BLUE}~~~~~~~~~~~~{Style.RESET_ALL}')

    def export_model(self):
        print(f'\n\n{Fore.BLUE}~~~EXPORT~~~{Style.RESET_ALL}')

        converter = tf.lite.TFLiteConverter.from_keras_model(self.model)
        tflite_model = converter.convert()

        with open('data/model.tflite', 'wb') as f:
            f.write(tflite_model)

        print("Model exported")
        formatted_data = "alignas(16) const unsigned char model[] = {\n"

        with open("data/model.tflite", "rb") as file:
            binary_data = file.read()

        file.close()

        hex_data = binary_data.hex()

        for i in range(0, len(hex_data), 2):
            formatted_data += f"0x{hex_data[i:i+2]}, "

        # remove last comma
        formatted_data = formatted_data[:-2]
        formatted_data += "\n};"

        with open("data/model.h", "w") as file:
            file.write(formatted_data)

        file.close()

        print(f'\n\n{Fore.BLUE}~~~EXPORT COMPLETE~~~{Style.RESET_ALL}')


data = Training()
data.train_model()
data.export_model()
data.print_model()
