# import os
# import gc
# import machine
# import network

import urequests
# from socketIO_client import SocketIO, LoggingNamespace



def fetch_data(url):
    try:
        response = urequests.get(url)
        if response.status_code == 200:
            data = response.json()
            response.close()
            return data
        else:
            print("Failed to fetch data. Status code:", response.status_code)
            response.close()
            return None
    except Exception as e:
        print("Error fetching data:", e)
        return None

# def on_bbb_response(*args):
        # print('on_bbb_response', args)

# def socketIo():
#     socketIO = SocketIO(
#         'localhost', 8000,
#         params={"deviceId": "customClientId"},
#         headers={'Authorization': 'Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MSwiaWF0IjoxNzE0NzMzMjgyLCJleHAiOjE3MTQ3MzY4ODJ9.FwTsvSoxNFyJkGTza1Sgdx5mhetgqjWmF9Ts9C_6C0E'}
#     )
#     socketIO.emit('subscribe', {"deviceId": "customClientId"}, on_bbb_response)
#     socketIO.wait_for_callbacks(seconds=1)

def init():
    url = 'https://pieceowater.github.io/resume/src/data.json'
    data = fetch_data(url)
    if data:
        print("\nData from JSON file:")
        print(data)

init()