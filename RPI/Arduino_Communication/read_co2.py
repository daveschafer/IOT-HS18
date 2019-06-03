import serial
import time

arduinoReady = False

def setup_arduino():
    global arduinoReady
    global arduino_serial
    # Device may change if there is another serial device connected!
    # Use ls /dev/tty* to check new USB Serial devices.
    try:
        arduino_serial = serial.Serial('/dev/ttyACM0', 9600, timeout = 3)
    except:
        return 
    
    if arduino_serial.is_open:
        print('Arduino serial connection already open.')
        time.sleep(5)
        arduinoReady = True
    else:
        try:
            arduino_serial.open()
            # Arduino is being resetted, wait until available
            time.sleep(5)
            arduinoReady = True
        except:
            print('Arduino not initialized')

def read_co2():
    if arduinoReady:
        # Tell Arduino to start reading
        # Encode as Bytes first
        arduino_serial.write("read_co2".encode("utf-8"))
        print("Request sent.")
        response = arduino_serial.readline()
        
        # Remove '\r\n' and decode Bytes back to String
        response = response.rstrip().decode("utf-8")
        print("Arduino response: ")
        print(response)
        if is_float(response):
            return float(response)
        else:
            print('Not reading Co2 data')

    return 'NA'

def is_float(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

setup_arduino()

while True:
    co2 = read_co2()
    # Print if returned value
    if not co2 == 'NA':
        print('sending Co2:'+str(co2))
        ## 1. Directly send to Adafruit.IO
        #if(useAIO):
        #    aio.send_data(tempFeed.key, co2)
        ## 2. Send to Lora
        #if(useLORA):
        #    call_lora("'co2:" + str(co2) + "'") #message need to be encapsulated " ' message ' "
    else:
        print("Co2 not initialized!")
    time.sleep(5)
