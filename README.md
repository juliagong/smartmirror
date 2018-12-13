# Ƨ|SmartMirror
## The CS 107e project that belongs in every tech-savvy bedroom!

The SmartMirror displays the information you need to start your day right. It connects to your WiFi to display the current time, weather, and headlines. The SmartMirror is customizable to show the information you want in the style you want.

## Features
+ Connects to your WiFi to display the most current information
+ Home Page: Displays the Date & Time and Ambient Temperature
+ Weather Page: Displays current weather conditions in Palo Alto
+ Headlines Page: Displays top 10 headlines in World and US news
+ Choose between the classic Apple II font, or the sleek Segoe UI font
+ Choose between 5 color Themes
  - "Classic Binary",
  - "Midnight Office Hours",
  - "Royal Hacker's Hex",
  - "Sunset Soiree in C",
  - "Plantiful Pleasantree"
  

## Specs
+ Raspberry Pi Model A+ (Running the bare-metal code library written over the course of CS107e, in addition to project code)
+ Power Supply: MB-V2 Breadboard Power Supply with 9v power adapter 
+ Display: Dell E228WPFC Black LCD Monitor with an acrylic, 2-way mirror mounted in front
+ Motion Sensor: HC-SR501 PIR Motion Sensor
+ Wifi: ESP-WROOM-32
+ Temperature & Humidity: DHT-11
+ Navigation Dial: KY-040 Rotary Encoder

## Hardware Setup
### Display
The monitor must be plugged into an outlet and connected to the Pi with an HDMI cable. The acrylic 2-way mirror was mounted on the front with ????

### Power Supply 
(Evan TODO) Describe how it's wired, how it communicates to the pi, and anything notable about troubleshooting it.
The power supply is mounted on a breadboard. With the power supply at the "top", the 3.3V rail is on the right, and the 5v rail is on the left. The Pi is powered using the 5v and ground pins in the middle of the power supply while all other components are powered using the rails. 

### Motion Sensor
(Julia TODO) Describe how it's wired, how it communicates to the pi, and anything notable about troubleshooting it.

### Temperature/Humidity
(Evan TODO) Describe how it's wired, how it communicates to the pi, and anything notable about troubleshooting it.

### Rotary Dial
(Dan TODO) Describe how it's wired, how it communicates to the pi, and anything notable about troubleshooting it.

### ESP-WROOM-32
(Evan TODO) The ESP-32 is used to connect the Raspberry Pi to the internet in order to fetch current date, time, weather, and headlines. 
It is powered using the 3.3v rail on the breadboard. It is connected to the Pi's TX/RX pins via its own TX/RX pins, and communicates using UART. The ESP-32 was programmed using Arduino IDE based on code provided by Chris Gregg. When uploading to the ESP-32, make sure the 3.3v wire is disconnected and the Pi is off. When it Arduino IDE says it is "Connecting..." press and hold the button labeled "DOI" until upload begins. 



## Code
Currently code is listed in alphabetical order, but order can be changed to give the best overall understanding of how it works 
High-level description of what each file does
### module.c
### output_formatter.c
### profile.c
### sensors.c
### settings.c
### smart_display.c
### website
Three python scripts are hosted at Evander's Stanford directory. The 
+ gettime.py: Utilizes python's datetime object to print out the current date and time
+ getweather.py: Utilizes OpenWeatherMap to print current weather conditions in Palo Alto. Current temperature is provided in Fahrenheit and Celsius, with the integer and decimal parts printed separately to ease use with snprintf.
+ getheadlines.py: Displays the top 10 headlines from this [Google News rss feed](https://news.google.com/_/rss/search?q=reuters+news+-schedule&hl=en-US&gl=US&ceid=US:en). Each headline is terminated with a '^' to ease processing by the pi. 



