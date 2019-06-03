# ***************************************************
# * By Group 5 @ HSLU I.BSCI.HS18.IOT               *
# ****************************************************

import adafruit_gps
import serial
import board
import time

class GPSWrapper:
    def __init__(self):
        self.gps = None
        self.initialized = False

    def setup_gps(self, serialDevice):
        '''Setup GPS Sensor with a given serial device. Set GPS module to only collect GPS location, altitude and fix.'''
        #Setting GPS Interface
        try:
            uart = serial.Serial(serialDevice, baudrate=9600, timeout=3000)
        except Exception as e:
            print('GPSWrapper: Error while opening Serial Device: ' + str(e))
            return False

        try:
            self.gps = adafruit_gps.GPS(uart, debug=False)
            #Configure GPS module to send "Recommended Minimum Specific GNSS Sentence" and "GPS Fix Data", see: https://cdn.sparkfun.com/assets/parts/1/2/2/8/0/PMTK_Packet_User_Manual.pdf
            self.gps.send_command(b'PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0')
            #Configure how often the GPS module updates its values. Has to be lower than the UART timeout.
            self.gps.send_command(b'PMTK220,1000')
        except Exception as e:
            print('GPSWrapper: Failed to initialize GPS, Exception: ' + str(e))
            return False

        self.initialized = True
        return True

    def check_gps(self):
        '''Check if a GPS fix is available.'''
        # Update GPS Data
        self.gps.update()
        time.sleep(1)
        if not self.gps.has_fix:
            # Try again if we don't have a fix yet.
            print('GPSWrapper: [NoGPS] Waiting for fix (3 tries)...')
            time.sleep(1)
            if not self.gps.has_fix:
                print('GPSWrapper: [NoGPS] Waiting for fix (2 tries)...')
                time.sleep(1)
                if not self.gps.has_fix:
                    print('GPSWrapper: [NoGPS] Waiting for fix (1 tries)...')
                    return False
                else:
                    return True
            else:
                return True
            #return False
        else:
            return True

    def read_gps_latitude(self):
        '''Retrieve latitude.'''
        return self.gps.latitude

    def read_gps_longitude(self):
        '''Retrieve longitude.'''
        return self.gps.longitude

    def read_gps_altitude(self):
        '''Retrieve altitude.'''
        return self.gps.altitude_m