# Master

Master Python class to bring together everything:

- Sensor reading
- Data Transfer to Adafruit
- LORA Sending Framework

## Add to Autostart

If you want to automatically start the Application as soon as the Raspberry starts, proceed as follows:

    sudo crontab -e

    Add the following Line to the Crontab:
    
    @reboot sleep 10 && /home/pi/IOTPROJECT/launcher.sh >> /home/pi/IOTPROJECT/logfile.log
