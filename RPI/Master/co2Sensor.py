# ***************************************************
# * By Group 5 @ HSLU I.BSCI.HS18.IOT               *
# ****************************************************

import serial
import time

class Co2Sensor:
    def __init__(self):
        self.arduino_serial = None
        self.initialized = False

    def setup_arduino(self, serialPort):
        '''Setup Communication to Arduino connected as a serial device.'''
        # Device may change if there is another serial device connected!
        # Use ls /dev/tty* to check new USB Serial devices.
        try:
            self.arduino_serial = serial.Serial(serialPort, 9600, timeout = 3)
        except Exception as e:
            print('Co2Sensor: Serial Port not available, Exception: ' + str(e))
            return False
        
        if self.arduino_serial.is_open:
            time.sleep(5)
            self.initialized = True
            return True
        else:
            try:
                self.arduino_serial.open()
                # Arduino is being resetted, wait until available
                time.sleep(5)
                self.initialized = True
                return True
            except Exception as e:
                print('Co2Sensor: Arduino not initialized, Exception: ' + str(e))

        return False

    def read_co2(self):
        '''Retrieve Air Quality Level (1, 2, 3). Returns 0 if an illegal value is received by the Arduino.'''
        # Tell Arduino to start reading
        # Encode as Bytes first
        try:
            self.arduino_serial.write('read_co2'.encode('utf-8'))
            response = self.arduino_serial.readline()
        except Exception as e:
            print('Co2Sensor: Cannot write / read to Arduino Serial, Exception: ' + str(e))
            self.initialized = False
        
        # Remove '\r\n' and decode Bytes back to String
        response = response.rstrip().decode('utf-8')
        if self._is_int(response):
            return int(response)
        else:
            print('Co2Sensor: Not reading Co2 data')
            return 0

    def _is_int(self, s):
        '''Internal: Check whether a string can be casted to an int.'''
        try:
            int(s)
            return True
        except ValueError:
            return False
