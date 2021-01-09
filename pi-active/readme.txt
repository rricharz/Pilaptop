
 pi-active
 Version for the Pilaptop (Waveshare)

 rricharz 2021

 This daemon displays a row on the 5x5 LED matrix showing
 the activity of the CPU. It uses the bottom row which is
 even visible if the cover above the led matrix is in place.

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
It can be restarted with a reboot or with the command
     sudo systemctl start pi-active.service

Or proceed as follows:

Pi-active does not use the led display if a program called "R65"
is running, which is sometimes the case on my Pilaptop. Search for
"R65" in pi-active, replace it with any other name of a program
which uses the led display on your Pilaptop, and recompile pi-active
with "make"
