
 pi-active
 Version for the Pilaptop (Waveshare)

 rricharz 2019

 This daemon displays a row on the 5x5 LED matrix showing
 the activity of the CPU. 

 The speed of the LED blinking shows current CPU usage

 cpu usage code from Matheus (https:github.com/hc0d3r)

 To install:
     chmod +x install
     sudo ./install

 To uninstall
     chmod +x uninstall
     sudo ./uninstall

Important:
Pi-active needs to be stopped if other programs want to use
the LED matrix
     sudo systemctl stop pi-active.service
It can be rstarted with a reboot or with the command
     sudo systemctl start pi-active.service
