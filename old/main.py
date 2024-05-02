import os
import gc
import machine
import network
import urequests


def connect_to_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    if not wlan.isconnected():
        print(f"Connecting to WiFi ({ssid})...")
        wlan.active(True)
        print("--------------")
        wlan.connect(ssid, password)
        retries = 0
        while not wlan.isconnected():
            retries += 1
            print(f"Attempt: {retries}")
            if retries > 3:  
                print("Failed to connect to WiFi.")
                return False
        print("Connected to WiFi:", ssid)
        return True
    else:
        print("Already connected to WiFi.")
        return True

def get_storage_info():
    statvfs = os.statvfs('/')
    total_storage = statvfs[0] * statvfs[2] / (1024 * 1024) 
    free_storage = statvfs[0] * statvfs[3] / (1024 * 1024)  
    return total_storage, free_storage

def get_ram_info():
    ram_size = gc.mem_free() / (1024 * 1024)
    return ram_size

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

def init():
    total_storage, free_storage = get_storage_info()
    ram_size = get_ram_info()

    print("Device Storage:")
    print("Total Storage: {:.2f} MB".format(total_storage))
    print("Free Storage: {:.2f} MB".format(free_storage))

    print("\nRAM:")
    print("RAM Size: {:.2f} MB".format(ram_size))
    print("")



    # Connect to Wi-Fi
    ssid = 'Safe Level of Radiation'
    password = 'Idontwanttosettheworldonfire'
    # ssid = 'mid - iphone'
    # password = 'mid12345'
    if not connect_to_wifi(ssid, password):
        return

    # # Fetch data from JSON file
    # url = 'http://pieceowater.github.io/resume/src/data.json'
    # data = fetch_data(url)
    # if data:
    #     print("\nData from JSON file:")
    #     print(data)

init()