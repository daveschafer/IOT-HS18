
# Preparation for Coding on Heltec ESP32 Wifi Lora V2

1. Follow the procedure in this link (use "Easy Method) --> https://robotzero.one/heltec-wifi-kit-32/
2. Development Example for the ESP32 --> https://robotzero.one/heltec-wifi-lora-32/


# LORA Master

The LORA Master Gateway File are in the Subfolder ESP32_Gateway_Master [ESP32_Gateway_Master](/esp32/LORA/ESP32_Gateway_Master/)

# ESP32 Code

## GPS NMEA Message

```
Time: 14:1:54.0
Date: 29/11/2018
Fix: 1 quality: 1
Location: 4708.6611N, 826.1391E
Location (in degrees, works with Google Maps): 47.1444, 8.4357
Speed (knots): 1.60
Angle: 114.85
Altitude: 434.00
Satellites: 4
$PGTOP,11,2*6E
$GPGGA,140155.000,4708.6608,N,00826.1396,E,1,04,3.00,434.0,M,48.0,M,,*62
$GPRMC,140155.000,A,4708.6608,N,00826.1396,E,1.55,115.43,291118,,,A*69
$PGTOP,11,2*6E
$GPGGA,140156.000,4708.6606,N,00826.1402,E,1,04,3.00,434.0,M,48.0,M,,*65
$GPRMC,140156.000,A,4708.6606,N,00826.1402,E,1.53,114.80,291118,,,A*66


Time: 14:3:26.0
Date: 29/11/2018
Fix: 0 quality: 0
$PGTOP,11,2*6E
$GPGGA,140327.000,,,,,0,01,,,M,,M,,*7A
$GPRMC,140327.000,V,,,,,0.02,300.41,291118,,,N*48
$PGTOP,11,2*6E
$GPGGA,140328.000,,,,,0,01,,,M,,M,,*75
$GPRMC,140328.000,V,,,,,0.06,302.56,291118,,,N*47
```