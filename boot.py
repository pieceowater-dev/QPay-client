# boot.py -- run on boot-up
# import webrepl_setup

WIFI_NAME = 'Pieceowater'
WIFI_PASS = 'Idontwanttosettheworldonfire'


def wifi_connect():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    # print(wlan.scan())
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect(WIFI_NAME, WIFI_PASS)
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())

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
    

    wifi_connect()

    print("BOOTED UP!")


init()