import numpy as np
import tensorflow as tf
import pandas as pd
from colorama import Fore, Style

interpreter = tf.lite.Interpreter(model_path="model.tflite")
interpreter.allocate_tensors()

dummyData = pd.read_json('./data/test.json')


def display(datum, output_data):
    max_index = np.argmax(output_data[0])
    percentage = round(output_data[0][max_index]*100, 2)

    match_color = Fore.RED if max_index != datum['label'] else Fore.GREEN
    acc_color = Fore.RED if percentage < 60 else Fore.GREEN
    reset = Style.RESET_ALL
    correct = max_index == datum['label']

    print(f"\n\nInput: {datum['flexVals']}")
    print(f"Output: {max_index} => {acc_color}{percentage}%{reset} Label: {
          datum['label']}, Match:{match_color} {correct} {reset}")
    print(f"Full Array: {output_data[0]}")


def predict(datum):
    input_data = np.array([datum['flexVals']], dtype=np.float32)
    interpreter.set_tensor(interpreter.get_input_details()[0][
                           'index'], input_data)
    interpreter.invoke()

    return interpreter.get_tensor(
        interpreter.get_output_details()[0]['index'])


for index, datum in dummyData.iterrows():
    output_data = predict(datum)

    display(datum, output_data)
