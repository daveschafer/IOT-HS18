# ***************************************************
# * By Group 5 @ HSLU I.BSCI.HS18.IOT               *
# ****************************************************

import board
import busio
import adafruit_bme280

class BME280Wrapper:
    def __init__(self):
        self.bme280 = None
        self.initialized = False

    def setup_bme(self, i2c, i2cAddress, seaLevelPressure):
        '''Setup BME280 Sensor by I2C with a given address.'''
        try:
            # default address = 0x77 for adafruit and 0x76 for china version
            self.bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c, address=i2cAddress)
            # change this to match the location's pressure (hPa) at sea level
            self.bme280.sea_level_pressure = seaLevelPressure
            self.initialized = True
            return True
        except:
            print('BME280Wrapper: BME280 not initialized')
            return False

    def read_bme_temp(self):
        '''Retrieve Temperature from Sensor. Returns an infinite Float value if illegal.'''
        bmeTemp = self.bme280.temperature
        if (self._is_float(bmeTemp)):
            return float('%0.1f' % self.bme280.temperature)
        else:
            # Return largest possible float value
            print('BME280Wrapper: Not reading BME Temperature.')
            return float('inf')

    def read_bme_humi(self):
        '''Retrieve Humidity from Sensor. Returns an infinite float value if illegal.'''
        bmeHumi = self.bme280.humidity
        if (self._is_float(bmeHumi)):
            return float('%0.1f' % bmeHumi)
        else:
            # Return largest possible float value
            print('BME280Wrapper: Not reading BME Humidity.')
            return float('inf')

    def read_bme_press(self):
        '''Retrieve Pressure from Sensor. Returns an infinite Float value if illegal.'''
        bmePres = self.bme280.pressure
        if (self._is_float(bmePres)):
            return float('%0.1f' % bmePres)
        else:
            # Return largest possible float value
            print('BME280Wrapper: Not reading BME Pressure.')
            return float('inf')

    def _is_float(self, s):
        '''Internal: Check whether a string can be casted to a float.'''
        try:
            float(s)
            return True
        except ValueError:
            return False