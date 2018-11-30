
**Project Proposal**

* **Project title:** Smart Display
* **Team members:** Evander Deocariza, Julia Gong, Min Hyung (Dan) Kang
* **Description:** Our team plans to create an interactive smart display (“mirror”) that utilizes a variety of environmental sensors to display information to the user. A two-way mirror will be attached onto a computer monitor that, when idle, will function as a simple mirror. Once a person approaches, a motion proximity sensor will detect their presence and start displaying information. We hope to utilize multiple sensors from the sensor kit, starting from the motion sensor and temperature sensor. If time permits, we may add on a color sensor and ESP-32 module for background color picking and pulling news or other information from webpages, respectively. We will also implement a battery-run clock, which will display the current time. Ideally, we hope to have time to build an interface with a digital rotary encoder that would allow users to interact with our display and switch between sensor information displays.

**Hardware required:**
* Monitor: we will utilize one from the lab
* Sensor kit: a collection of sensors that we could integrate into the display
* Two-way mirror / screen: we will attach it onto our monitor using clamps
* Clamp to attach the mirror / screen
* Color (RGB) detection sensor and ESP32 module (possibly, if time permits)

**Funds requested:**

| Category       | Full Name                            | Vendor | Price ($) |
|----------------|--------------------------------------|--------|-----------|
| Sensor Kit     | ELEGOO 37 in 1 Sensor Modules Kit    | Amazon | 29.86     |
| Two-way Mirror | 12" x 24" Acrylic See-Through Mirror | Amazon | 29.99     |
| Total          |                                      |        | 59.85     |

**References (initial list):**
* Smart mirror project from hackaday: https://hackaday.io/project/13466-raspberry-pi-smart-mirror
* Manual for sensor kit: https://www.elegoo.com/download/
* References and tutorials for different sensors:
  * Motion sensor: https://www.mbtechworks.com/projects/pir-motion-sensor-with-raspberry-pi.html
  * Real-time clock: http://www.intellamech.com/RaspberryPi-projects/rpi_RTCds3231
  * Temperature and humidity sensor: http://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-the-raspberry-pi/

**Task Breakdown** (including goals for next week):

| Task                                   | Description                                                                                                                         | Point Person                                                                       | Timeline |
|----------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------|----------|
| Core Functionality                     |                                                                                                                                     |                                                                                    |          |
| Display / Application                  | Identify/design methods that are required                                                                                           | Dan                                                                                | 2-Dec    |
|                                        | Build on current display library                                                                                                    |                                                                                    |          |
|                                        | Create backbone of application                                                                                                      |                                                                                    |          |
|                                        | Combine with sensors                                                                                                                | All                                                                                |          |
| Proximity/Motion Sensor                | Get sensor to work (wiring)                                                                                                         | Julia                                                                              | 2-Dec    |
|                                        | Get sensor to report when a person is in front                                                                                      |                                                                                    |          |
|                                        | Distinguish three cases: person has entered proximity, person is in proximity, person has exited proximity (Look into trigger mode) |                                                                                    |          |
|                                        | Link with display function                                                                                                          |                                                                                    | 5-Dec    |
| Temperature Sensor                     | Get sensors to work (wiring)                                                                                                        | Evan                                                                               | 2-Dec    |
|                                        | Get sensors to report current temperature                                                                                           |                                                                                    |          |
|                                        | Link with display function                                                                                                          |                                                                                    | 5-Dec    |
| Clock                                  | Get RTC (real-time clock) module to work                                                                                            | All                                                                                | 5-Dec    |
|                                        | Link with display function                                                                                                          |                                                                                    | 7-Dec    |
| Basic UX / Rotary Dial                 | Get rotary encoder to work                                                                                                          | All (task delegation from here occurs incrementally; we currently haven’t decided) | 5-Dec    |
|                                        | Allow display to have multiple states                                                                                               |                                                                                    | 7-Dec    |
|                                        | Allow switching back and forth the states. Put in dummy screens for different states                                                |                                                                                    | 7-Dec    |
|                                        | Link rotary dial with switching back and forth                                                                                      |                                                                                    | 7-Dec    |
| Add-on Functionality (if time permits) |                                                                                                                                     |                                                                                    |          |
| Download from web                      | Add ability to pull from web pages                                                                                                  | All                                                                                | 9-Dec    |
|                                        | Pull weather information                                                                                                            | All                                                                                | 11-Dec   |
|                                        | Pull headline information                                                                                                           | All                                                                                | 13-Dec   |
| Advanced UX                            | Use specific color to customize user profile                                                                                        | All                                                                                | 13-Dec   |
|                                        | Allow setting time info (or pull time from web)                                                                                     | All                                                                                | 13-Dec   |
| Color Sensor                           | Get color sensor to work                                                                                                            | All                                                                                | 9-Dec    |
|                                        | Use sensed color to change display (e.g.font)                                                                                       | All                                                                                | 11-Dec   |
