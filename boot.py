# boot.py -- run on boot-up
import network


WIFI_NAME = "Safe Level of Radiation"
WIFI_PASS = "Idontwanttosettheworldonfire"

WLAN_INSTANCE = network.WLAN(network.STA_IF)

def connect_to_wifi(ssid, password):
    print(f"WiFi active ({WLAN_INSTANCE.active()}) and is con ({WLAN_INSTANCE.isconnected()})")
    try:
        WLAN_INSTANCE.connect()
        if not WLAN_INSTANCE.isconnected():
            print(f"WiFi status ({WLAN_INSTANCE.status})")
            print(f"Connecting to WiFi ({ssid})...")
            WLAN_INSTANCE.active(True)
            print(f"connecting to {ssid}, {password}")
            WLAN_INSTANCE.connect(ssid, password)
            print(f"mac: {WLAN_INSTANCE.config('mac')}")

            retries = 0
            while not WLAN_INSTANCE.isconnected():
                retries += 1
                print(f"Attempt: {retries}, {WLAN_INSTANCE.ifconfig()}")
                if retries > 3:  
                    print("Failed to connect to WiFi.")
                    return False
            print("Connected to WiFi:", ssid)
            return True
        else:
            print("Already connected to WiFi.")
            return True
    except Exception as e:
        WLAN_INSTANCE.disconnect()
        print(e)
        return False


def get_storage_info():
    statvfs = os.statvfs('/')
    total_storage = statvfs[0] * statvfs[2] / (1024 * 1024) 
    free_storage = statvfs[0] * statvfs[3] / (1024 * 1024)  
    return total_storage, free_storage

def get_ram_info():
    ram_size = gc.mem_free() / (1024 * 1024)
    return ram_size



def init():
    total_storage, free_storage = get_storage_info()
    ram_size = get_ram_info()

    print(">>>>>>>>>>>>>>>>>>>>>>")
    print("RAM Size: {:.2f} MB\n".format(ram_size))
    print("Total Storage: {:.2f} MB".format(total_storage))
    print("Free Storage: {:.2f} MB".format(free_storage))
    print(">>>>>>>>>>>>>>>>>>>>>>")


    #ssid = 'OnePlus vIt'
    #password = 'goodgames'
    # ssid = 'mid - iphone'
    # password = 'mid12345'
    ssid = WIFI_NAME
    password = WIFI_PASS

    if not connect_to_wifi(ssid, password):
        return

    print("BOOTED UP!")


init()