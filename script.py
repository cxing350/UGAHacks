import serial
import time
from pymongo import MongoClient
from datetime import datetime, timezone

client = MongoClient('mongodb+srv://cxing:JZQtteO8fdHi916r@cluster0-akbsl.gcp.mongodb.net/test?retryWrites=true&w=majority')

ser = serial.Serial('COM6', 9800, timeout=1)
time.sleep(5)
ser.write(b'A')

db = client.UGAHacks
panelData = db.panelData
atmData = db.atmData

while True:
    x = ser.readline() #"avg angle"
    output = x.split()
    
    if len(output) == 5:
        data = {"brightness":float(output[0]), "angle":float(output[1]), "time":datetime.now(timezone.utc)}
        print(data)
        panelData.insert_one(data)
        atmData.insert_one({"pressure":float(output[2]), "temperature":float(output[3]), "altitude":float(output[4])})
        

ser.close()
exit()