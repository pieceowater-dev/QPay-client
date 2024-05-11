# QPay-client

> Use Pymakr VSCode Extention to upload project to device and run

### create python virtual env
```bash
python3 -m venv venv
```

### activate python virtual env
```bash
source venv/bin/activate
```

### pip
```bash
pip install esptool

pip freeze > requirements.txt

pip install -r requirements.txt
```



### command to get list of connected devices
```bash
ls /dev/cu.*
```

### command to erase device
```bash
esptool.py --chip esp32 --port /dev/cu.usbserial-0001 erase_flash
```

### command to install Firmware
```bash
esptool.py --chip esp32 --port /dev/cu.usbserial-0001 --baud 460800 write_flash -z 0x1000 ESP32_GENERIC-OTA-20240222-v1.22.2.bin
```


