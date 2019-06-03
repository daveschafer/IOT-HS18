# Raspberry Pi USB Serial Communication to Arduino

Using the serial Interface over USB has advantages over using I2C to connect the Arduino to the Raspberry Pi:

1. Power is supplied to the Arduino over USB --> no additional power source needed
2. Easy to setup
3. Not wasting any GPIO Pins on the Raspberry Pi

## HW Setup
Connect the Raspberry Pi to the Arduino by USB

## Software
```
pip3 install pyserial
pip3 install serial
```

**Check if Serial Interface is available**
```
ls /dev/tty*
```

This will list all available Devices. Either unplug the Arduino and run the command again to see whether there is a new device or just check if device `/dev/ttyACM0` is available.

### Communicating with the Arduino

Send `arduino_serial.write("read_co2")` in order to wake the Arduino and retrieve Air Polution data.