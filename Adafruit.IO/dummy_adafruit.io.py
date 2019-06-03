#this is just for demonstrating how to send data to adafruit.io
#i created a dummy_rpi feed for this on Adafruit.IO

# Simple example of sending and receiving values from Adafruit IO with the REST
# API client.
# Author: Tony Dicola, Justin Cooper

# Import Adafruit IO REST client.
from Adafruit_IO import Client, Feed, Data, RequestError
import datetime
import time

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_KEY = 'fb1e2ed9d91848ea9f0f43d85e960fb0'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
ADAFRUIT_IO_USERNAME = 'hslu_iot_hs18'

# Create an instance of the REST client.
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Assign a test feed, if one exists already
try: 
    test = aio.feeds('dummy-rpi') #wichtig: auch wenn man _ im adafruit IO macht, es kennt nur - (Bindestriche)
except RequestError: # Doesn't exist, create a new feed
    feed = Feed(name="dummy_rpi_created")
    test = aio.create_feed(feed)

# Send some continuous values to adafruit.io
for x in range(0, 25):
    aio.send_data(test.key, x)
    x += 1
    print('sending value: '+str(x))
    time.sleep(1) #sleepy sleep for a second

# Now read the most recent value from the feed 'Test'.  Notice that it comes
# back as a string and should be converted to an int if performing calculations
# on it.
data = aio.receive(test.key)
print('Retrieved value from Test has attributes: {0}'.format(data))
print('Latest value from Test: {0}'.format(data.value))
