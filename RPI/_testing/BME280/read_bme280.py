# Raspberrry Pi: Read BME280 values

# Connect:
# VIN -> 5V
# GND -> GND
# SCK -> SCL
# SDI -> SDA

import time
import json

import board
import busio
import adafruit_bme280

# Create library object using our Bus I2C port
i2c = busio.I2C(board.SCL, board.SDA)

def setup_bme():
    global bmeReady
    global bme280
    try:
	    #default address = 0x77 for adafruit and 0x76 for china version
        bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c, address=0x76)
        # change this to match the location's pressure (hPa) at sea level
        bme280.sea_level_pressure = 1013.25
        bmeReady = True
    except:
        print('BME280 not initialized')

def read_bme():
    bmeJson_data = ''
    if bmeReady:
        bmeJson['temperature'] = float('%0.1f' % bme280.temperature)
        bmeJson['humidity'] = float('%0.1f' % bme280.humidity)
        bmeJson['pressure'] = float('%0.1f' % bme280.pressure)
        bmeJson['altitude'] = float('%0.1f' % bme280.altitude)
        bmeJson_data = json.dumps(bmeJson)
    else:
        print('Not reading BME280 data')
    return bmeJson_data

bmeReady = False

bmeJson = {}
setup_bme()

while True:
    bmeJson_data = read_bme()
    print(bmeJson_data)
    time.sleep(5)
