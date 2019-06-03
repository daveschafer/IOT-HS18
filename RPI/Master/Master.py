# ***************************************************
# * By Group 5 @ HSLU I.BSCI.HS18.IOT               *
# ****************************************************

########################
# Informations         #
########################

# Raspberry Pi: Read BME280 values
# Raspberry Pi: Read BME280 values
# Raspberry Pi - Dragino HAT: Read GPS
# Raspberry Pi - Dragino HAT: Acces LORA Network

# Connect BME280:
# VIN -> 5V
# GND -> GND
# SCK -> SCL
# SDI -> SDA

########################
# Imports              #
########################

import os

#For CO2 Sensor
from co2Sensor import Co2Sensor

#For BME and Sensors
from bme280Wrapper import BME280Wrapper
import board
import busio

#For GPS
from gpsWrapper import GPSWrapper

##For Adafruit.IO
from Adafruit_IO import Client, Feed, Data, RequestError
import datetime
import time


########################
# Global Variables     #
########################

# If set True, it will send the messages through LORA
useLORA = True

# If set True, it will directly send the data to Adafruit.IO
useAIO = False

#time between lora sendings
loraTimer = 3

## Adafruit.IO Settings
ADAFRUIT_IO_KEY = 'fb1e2ed9d91848ea9f0f43d85e960fb0'
ADAFRUIT_IO_USERNAME = 'hslu_iot_hs18'
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Create library object using our Bus I2C port
i2c = busio.I2C(board.SCL, board.SDA)


########################
# Function Definitions #
########################

def _call_lora(message):
    '''Send a message over LoRa. Calls the specified c program.'''
    # call the c app
    #print('call_lora')
    os.system('/home/pi/LORA/dragino_lora_app sender '+ message) #path to LORA App here please, domo aligatoh

def _send_message(feedKey, msgType, msg):
    '''Send a message to either LoRa or AIO.'''
    print('sending ' + msgType +':' + str(msg))
    ## 1. Directly send to Adafruit.IO
    if(useAIO):
        aio.send_data(feedKey, msg)
    ## 2. Send to Lora
    if(useLORA):
        _call_lora("'" + msgType + ":" + str(msg) + "'") #message need to be encapsulated " ' message ' "
    #time.sleep(3) # Adafruit: 3s timeout. 
    time.sleep(loraTimer) # Adafruit: 3s timeout |Lora: 5s timeout


def _read_send_temp():
    bmeTemp = bme.read_bme_temp()
    if not bmeTemp == float('inf'):
        # sending temp
        _send_message(tempFeed.key, 'temperature', bmeTemp)
    else:
        print('Invalid Temperature returned')
    return bmeTemp

def _read_send_humi():
    bmeHumi = bme.read_bme_humi()
    if not bmeHumi == float('inf'):
        # sending humidity
        _send_message(humiFeed.key, 'humidity', bmeHumi)
    else:
        print('Invalid Humidity returned')
    return bmeHumi

def _read_send_press():
    bmePress = bme.read_bme_press()
    if not bmePress == float('inf'):
        # sending pressure
        _send_message(pressFeed.key, 'pressure', bmePress)
    else:
        print('Invalid Pressure returned')
    return bmePress

def _read_send_co2():
    co2Level = co2.read_co2()
    if co2Level > 0:
        _send_message(co2Feed.key, 'co2', co2Level)
    else:
        print('Invalid Co2 Level returned: ' + str(co2Level))


if __name__ == "__main__":
    #######################
    # Starting Program    #
    #######################
    print('*****************************************************')
    print('*****************************************************')
    print('***     Raspberry Pi: LEM-KIT v2                  ***')
    print('***       GPS: ok                                 ***')
    print('***       BME: ok                                 ***')
    print('***       CO2: ok                                 ***')
    print('*****************************************************')
    print('*****************************************************')
    print('=' * 40)  # Print a separator line.


    #Assigning Feeds from Adafruit.IO, only if useAIO=true
    try:
        tempFeed = aio.feeds('temperature-rpi') #wichtig: auch wenn man _ im adafruit IO macht, es kennt nur - (Bindestriche)
        humiFeed = aio.feeds('humidity-rpi')
        pressFeed = aio.feeds('pressure-rpi')
        co2Feed = aio.feeds('co2-rpi')
        locationFeed = aio.feeds('gps-rpi') #gps feed
    except RequestError: # Doesn't exist, create a new feed
        print('Oh oh, something went wrong with assigning Adafruit.IO Feeds :(')

    print('***********************************')
    print('***     Initializing Sensors    ***')
    print('***********************************')

    #Starting BME Sensor
    print('Initializing BME280 Sensor')
    bme = BME280Wrapper()
    if (bme.setup_bme(i2c, i2cAddress=0x76, seaLevelPressure=1013.25)):
        print('BME280 Sensor Initialization successful.')

    #Starting GPS Sensors
    print('Initializing GPS Sensor')
    gps = GPSWrapper()
    if (gps.setup_gps('/dev/ttyS0')):
        print('GPS Sensor Initialization successful.')

    #GPS Variables
    value = 0   # Value wird benötigt von Adafruit.IO, kann aber immer 0 sein
    latitude = 40.726190
    longitude = -74.005334
    altitude = 6 # elevation above sea level (meters)
    _call_lora("'gps:" + str(value) + ", " + str(latitude) + ", " + str(longitude) + ", " + str(altitude) + "'") #message need to be encapsulated " ' message ' "
    time.sleep(1)

    #Setup Arduino Communication / Co2 Sensor
    print('Initializing Co2 Sensor')
    co2 = Co2Sensor()
    #if (co2.setup_arduino('/dev/ttyUSB0')): ## ttyUSB0 -> raspi roman
    if (co2.setup_arduino('/dev/ttyACM0')): ## ttyACM0 -> raspi dave
        print('Co2 Sensor Initialization successful.')


    print('***********************************')
    print('***      Start Reading Data     ***')
    print('***********************************')
    ##Its important to sleep 3 seconds between the measurements, because we only have 30 free data pushed to Adafruit.IO per minute. 5 Seconds timeout if lora is used
    while True:
        try:
            if (bme.initialized):
                _read_send_temp()
                _read_send_humi()
                _read_send_press()
            else:
                print('BME280 not initialized!')

            if (co2.initialized):
                _read_send_co2()
            else:
                print('Co2 not initialized!')

            #Check if sending GPS is possible:
            if (gps.initialized):
                #Check if GPS has a fix:
                if (gps.check_gps()):
                    #value, lat, long, alt
                    latitude = gps.read_gps_latitude()
                    longitude = gps.read_gps_longitude()
                    altitude = gps.read_gps_altitude()
                    print('\nSending GPS Infos to location feed...\n')
                    print('\tLat: ', latitude)
                    print('\tLon: ', longitude)
                    print('\tAlt: ', altitude)
                    #Sending Data to Adafruit:
                    ## 1. Directly send to Adafruit.IO
                    if(useAIO):
                        aio.send_location_data(locationFeed.key, value, latitude, longitude, altitude)
                    ## 2. Send to Lora
                    if(useLORA):
                        _call_lora("'gps:" + str(value) + "," + str(latitude) + "," + str(longitude) + "," + str(altitude) + "'") #message need to be encapsulated " ' message ' "
                    #time.sleep(3) # Adafruit: 3s timeout.
                    time.sleep(loraTimer) # Adafruit: 3s timeout |Lora: 5s timeout

                else:
                    print('No GPS Signal... sending no Data')
            else:
                print('GPS not initialized!')

            print('=' * 40)  # Print a separator line.
        except Exception as e:
            print('Something went wrong...')
            print('=' * 40)
            print('Error: ' + str(e))
            print('=' * 40)
        time.sleep(1)
