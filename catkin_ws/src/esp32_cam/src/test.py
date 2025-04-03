import json
def QRresultJson(raw_text):
    result_dict = {}
    for line in raw_text.split("\n"):
        if ": " in line:
            key, value = line.split(": ", 1)
            result_dict[key.strip()] = value.strip()
    return result_dict

print(json.dumps(QRresultJson("Name: abc\nTime: 123\nType: abc\nCode: 123")))