# Ƨ|SmartMirror
## The CS 107e project that belongs in every tech-savvy bedroom!

The SmartMirror displays the information you need to start your day right. It connects to your WiFi to display the current time, weather, and headlines. The SmartMirror is customizable to show the information you want in the style you want.

## Features
+ Connects to your WiFi to display the most current information
+ Home Page: Displays the Date & Time and Ambient Temperature/Humidity
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
The monitor must be plugged into an outlet and connected to the Pi with an HDMI cable. The acrylic 2-way mirror was cut to size by scoring the mirror then snapping off the excess. It was mounted to the monitor using velcro strips.

### Power Supply 
The MB-V2 power supply is mounted on a breadboard. With the power supply at the "top", the 3.3V rail is on the right, and the 5v rail is on the left. The jumpers on the power supply determine what voltage goes to the rails. 
The Pi is powered using the 5v and ground pins in the middle of the power supply while all other components are powered using the rails. 

### Motion Sensor
The HC-SR501 motion sensor has 3 pins. It is powered with 5v and is connected to GPIO_PIN19. The jumper is set to react to multiple triggers, meaning the amount of time the sensor is high after being triggered is restarted every time the sensor picks up motion. 

### Temperature/Humidity
The DHT-11 has 3 pins. It is powered with 3.3v and is connected to GPIO_PIN26. 

### Rotary Dial
The KY-040 Rotary Encoder has 5 pins. It is powered using 3.3 V. 
The CLK pin is connected to GPIO_PIN16
The DT pin is connected to GPIO_PIN20
The SW pin is connected to GPIO_PIN21

### ESP-WROOM-32
(Evan TODO) The ESP-32 is used to connect the Raspberry Pi to the internet in order to fetch current date, time, weather, and headlines. 
It is powered using the 3.3v rail on the breadboard. It is connected to the Pi's TX/RX pins via its own TX/RX pins, and communicates using UART. The ESP-32 was programmed using Arduino IDE based on code provided by Chris Gregg. When uploading to the ESP-32, make sure the 3.3v wire is disconnected and the Pi is off. When it Arduino IDE says it is "Connecting..." press and hold the button labeled "DOI" until upload begins. 


## Code
### smart_display.c
Contains void main() which initializes the modules, profiles, and sensors. It then calls display() which is the main loop that draws the page if motion is detected. It also allows user to click into settings page. 

### module_display.c
Takes current page configuration and displayis all the modules within the page. We check if we need to update information for each module (e.g. new time), fetch the data if so. We then display each module's content. 

### module.c
A module encapsulates/represents a stand-alone entity with one defined function. We have 5 defined modules: Proximity, Temperature, DateTime, Weather, and Headline. 
Each module should have the following characteristics:
- It should be able to report if we have new information: this is most relevant with proximity sensor
- It should have a defined function to update its content: in most of our cases, we call corresponding function in sensors, which then populates our content with formatted, newest information.
- It should have contents, which are sets of string buffers. Each buffer is then displayed using our module_disply.c. 

### sensors.c
Contains the code for communicating directly with the various sensors. It initializes and reads in data from the sensors. 
Sensors.c also implements interrupts for the rotary dial. Data is initially stored in an array of strings, which is formatted into a resultBuf for printing to the display. 

### output_formatter.c
Formats the data array of strings from sensor.c into buf arrays that can be printed to the display. 

### profile.c
Each *profile* represents a set of settings which include the following:
- Configuration for each module
- Configuration for each page: information about which module to display at which location
- Font/theme setting

The code is written so that we can have multiple profiles to switch and back between, yet we do not support it in user interface.

### settings.c
Implements the setting menu which allows the user to customize the display format of the Date, Time, Temperature, and Humidity. The user can also choose between 5 theme colors and 2 fonts. 

### font.c
Contains definition for our two different fonts and allow switching between them.

### custom_font_draw.c
Allows drawing strings of different integer scales.


### website
Three python scripts are hosted at Evander's Stanford directory. The 
+ [gettime.py](http://web.stanford.edu/~evandeo/cgi-bin/gettime.py): Utilizes python's datetime object to print out the current date and time
+ [getweather.py](http://web.stanford.edu/~evandeo/cgi-bin/getweather.py): Utilizes OpenWeatherMap to print current weather conditions in Palo Alto. Current temperature is provided in Fahrenheit and Celsius, with the integer and decimal parts printed separately to ease use with snprintf.
+ [getheadlines.py](http://web.stanford.edu/~evandeo/cgi-bin/getheadlines.py): Displays the top 10 headlines from this [Google News rss feed](https://news.google.com/_/rss/search?q=reuters+news+-schedule&hl=en-US&gl=US&ceid=US:en). Each headline is terminated with a '^' to ease processing by the pi. 

## Contributions 
+ Evander Deocariza:
    - Temperature/humidity sensor
    - ESP-32 setup
    - Python scripts to retrieve information from web
    - Wiring / hardware 
+ Julia Gong:
    - Implementing a new font 
    - Support for different themes
    - Support for different font size
    - Proximity sensor
+ Minhyung (Dan) Kang:
    - Rotary encoder
    - Platform design / implementation
    - Support setting changes

