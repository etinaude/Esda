f = open("data/hex_model.txt", "r")
formatted_data = ""

while True:
    data = f.read(2)
    if (data == ''):
        break
    formatted_data += f"0x{data}, "

print(formatted_data)
f.close()

# remove last comma
formatted_data = formatted_data[:-2]

# output
f = open("data/hex_model_ard.txt", "w")
f.write(formatted_data)
f.close()
