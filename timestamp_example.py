#!/usr/bin/python
import os.path
import time

timestamp_filename = os.path.expanduser("~/.GDAQRec_timestamp");
while True:
    time.sleep(0.5) # every half second

    if os.path.exists(timestamp_filename):
        with open(timestamp_filename, "rt") as timestamp_file:
            timestamp = timestamp_file.read() 
            print(timestamp)
    else:
        print("not recording")


