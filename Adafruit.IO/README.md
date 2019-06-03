# Adafruit.IO

This SUbsection describes how to work with adafruit. You will find some code samples in Python and in C here.

## What is Adafruit.IO?

Adafruit.IO is a cloud based platform for sending, receiving and storing Data.

We will use it in this project, to visualize our collected data (temp, humidity, pressure, co2, gps) via a Dashboard.

## Getting Started

First you will need to create an account and configure your first Adafruit **Dashboard** and Adafruit **Feed**.

This process is best described here, carefully follow all the instructions and read **all** the sublinks:
https://learn.adafruit.com/mqtt-adafruit-io-and-you/getting-started-on-adafruit-io


**Feeds:** https://learn.adafruit.com/adafruit-io-basics-feeds  


**Important note about feeds**: "For example, if you have a project with one temperature sensor and two humidity sensors, you would need to create three feeds."  
**Important note 2**: adafruit only knows '-' and no '_'. It will change '_' to '-', pleas remember this when creating geeds

## Adafruit.IO and Raspberry

Since wo use a Raspberry Pi as our Datacollector, we can send our data with REST or MQTT:

https://learn.adafruit.com/adafruit-io/python

The Libraries can be found here:
https://github.com/adafruit/io-client-python

The easiest setup with python is via pip

    pip3 install adafruit-io


Please take a look at the "dummy_adafruit.io.py" example to understand how it works with Python

## Adafruit.IO and ESP

When we use an ESP to send Data (like in our example with the ESP LORA Gateway), we can use the provided C Libraries and send the Data with MQTT:

https://learn.adafruit.com/mqtt-adafruit-io-and-you/arduino-plus-library-setup

Please take a look at the "dummy_esp32/dummy_esp32.ino" example to understand how it works with Python

## Adafruit.IO and GPS

GPS with adafruit is a bit more complex.

Most ist described here: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-io-basics-gps.pdf

But to understand how to build the messages, you have to take a deeper look int their code from here:

https://learn.adafruit.com/track-your-treats-halloween-candy-gps-tracker/software

https://github.com/adafruit/Track_Your_Treats

For Python:

https://github.com/adafruit/io-client-python/blob/master/examples/basics/location.py
