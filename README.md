# QPay-client


python3 -m venv myenv
source myenv/bin/activate   # Activate the virtual environment on macOS/Linux


pip install esptool

pip freeze > requirements.txt


ls /dev/cu.*



esptool.py --chip esp32 --port /dev/cu.usbserial-0001 erase_flash


esptool.py --chip esp32 --port /dev/cu.usbserial-0001 --baud 460800 write_flash -z 0x1000 ESP32_GENERIC-20240222-v1.22.2.bin