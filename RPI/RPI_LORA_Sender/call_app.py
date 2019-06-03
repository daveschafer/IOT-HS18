#App for testing to call another app with python (here the LORA_Sender app which we compiled)

import os
import time
# Function Definition


def call_lora(message):
    # call the c app
    #print('call_lora')
    os.system('/home/pi/LORA/dragino_lora_app sender '+ message) #path to LORA App here please, domo aligatoh


# main program sequence
print('Lora send framework started...')

message = "'temp: 12.3'"
print('Lora send message: ' + message)
x = 1
while True:
    call_lora(message)
    message = "'temp: 12." + str(x) + "'" #Wîchtig: die message muss gewrapped werden " ' message ' "
    x = x+1
    print('Lora send message: ' + message)
    # important to sleep for 5 second or else the chip would broadcast too much information
    time.sleep(5) #has to match with line 462 in c code
