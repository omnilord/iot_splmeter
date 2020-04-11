# DIY Arduino Soundmeter

## Introduction
For a while now I have been struggling with a particularly intrusive neighbor who has been...well, substantially more than a nuisance and has resulted in numerous calls to the police for noise complaints.  I have misophonia, so certain types of audio trigger intensely negative physiological reactions for me, especially with bassy audio since that can be felt and not just heard.  Add to that a penchant for sleeping disorders that leave me sleeping at weird hours, and I have a very difficult time with disruptive noises.  I won't go into details, but this neighbor is not a student of Mr. Rogers and has gotten wise to how to avoid dealing with police.  They are now being blatantly malicious about the noise.  While I cope with this torturous situation by keeping a ledger of the incidents (I am 6 months in and at 35 pages into a composition book recording my ire about this; comparison, if each day was a "quiet/no noteworthy disruptions" entry, I would be at around page 6).  I am desperately in need of a more cathartic relief, so I've decided to make lemonade and find an empirical way of documenting the persistent--albeit inconsistent--playing of excessively loud bassy music.

I spent some time perusing the NIST-qualified sound meters on Amazon, and let me tell you, they get expensive.  Sure, you could get a cheap meter, but there is no guarantee the device measures accurately, and worse, they don't measure &lt; 30dB, which is where I need to baseline.  Mobile devices / apps are fairly useless because they don't seem to pick up the bass-side of things or capture C-weighted tones, so unless it is so loud that it vibrates things in my house (sometimes it is, but I never seem to catch it on video fast enough) no luck there.

What's my lemonade then?  I am taking a joyride down the deep rabbit hole of electronics fabrication, IoT sensing device herding, and data science (SCIENCE!).  I am building Arduino with piezoelectric vibration sensors pasted to several windows in my house.


This project consists of three parts:

In Part 1, I will review the build, the hardware components, how they work, and end-of-day lessons learned.  This will include component acquisition, assembly, testing, and various other bits.

In Part 2, I will look at how the software of the distinct components function, how to read information from the sensor, and process that out to the LED display and broadcast it via MQTT to an aggregation platform.

In Part 3, I will dive into the construction of the aggregation platform and look at the setup of a data pipeline that will take the basic information from each device, convert it into useful information (converting voltage to decibels) and displaying the time series in a use-friendly dashboard.  I will also be crafting a special purpose tool that I can use to demarcate segments of the time series as "nuisance events" and add qualitative notes that I normally jot down in my composition book (eg. weather).

Come with me, and you'll be in a world of amazing electronic satisfaction!  Let's Go!



## Part 1: Components Assemble!
### Arduino Basics
#### Component List
1. Arduino UNO R3: https://amzn.com/B07BTB3N3J $29.99

The Arduino UNO R3 kit is a a great starting place to pick up electronics since it comes with a bunch of the essentials for prototyping various circuits.  First, it comes with the all-important Arduino Microcontroller (MCP) board that is the centerpiece.  This beautiful palm-sized device is the hub upon which all circuits spin.  You upload your programs to this device, you plug in your components to this device, it's everything.  Second, if the Arduino is the hub, then the breadboard is the wheel.  The breadboard is how everything beyond super basic components gets wired together (as we'll see with the LED display later).

2. Piezoelectric Vibration Sensor: https://amzn.com/B08211CMSP $6.99

The Piezoelectric vibration sensor is the fundamental peripheral for a home made SPL meter as it is the workhorse that will be capture the vibrations we intend to measure.  These super light-weight, seemingly flimsy pieces of metal-bonded-to-ceramic are absolutely amazing when you dig into the physics behind what makes them work.  The long story short is they convert changes in pressure into voltage; sometimes significantly high levels of voltage, up to 50V(!!!), so mind that you use resistors properly to protect your 10 fold less--only 5V for the Arduino UNO--circuits).

3. Extra Shift Registers: https://amzn.com/B07HFWB9L9 $7.49

This project requires 2 shift registers per sensing unit that has an LED.  The package I purchased came with a whooping 25, so I guess I have plenty for extra projects.  These neat little chips allow you to extend the UNO registers using only 3 of the onboard digital pins.

4. Large, 4-digit 7-segment LED Displays: https://amzn.com/B07J2PX5XM $5.99

Each unit will need one 4-digit 7-segment LED display to allow us to see at the instantaneous reading off the piezoelectric sensor.

5. Extra Resistors: https://amzn.com/B06WRQS97C $6.49

Each LED is a 7 segment (8 when you include the decimal) so we need 8x 220 Ohm per unit to protect and regulate the display.  The substantial amount of resistors you can get for a few bucks in mind boggling...if you are following along, you are now the proud owner of hundreds of electronic components you might never needs.

6. ESP8266 WiFi modules: https://amzn.com/B01N98BTRH $12.99

An ESP8266 will allow us to connect to a Wireless network and transmit the readings over MQTT for long-term logging and data analysis.

7. ESP8266-Breadboard adapters: https://amzn.com/B01G6HK3KW $6.99

The ESP8266 SP-01 adapters will allow us to easily connect the ESP8266 to the circuit without having to worry about all the extra dumont cables that will eventually come loose.

8. Breadboard Jumpers: https://amzn.com/B07PQKNQ22 $12.99

Breadboard jumpers will come in handy for the final build.  You will see in my prototype that I formed 14 gauge steel wire by hand to craft jumpers.  These bespoke jumpers worked really well, but they were almost too large for the breadboard connection points and I am worried that I may have damaged the plastic with them.

9. Extra Breadboards + Dumont Cables (for final product): https://amzn.com/B07H7V1X7Y $9.99

For the final build, I will need full-sized breadboards to build out the sensors I will leave connected to my windows.  I toyed with the idea of actually getting solderable boards and just making permanent circuits, but this will be faster.

10. 3x Arduino Nano (for final product): https://amzn.com/B07G99NNXL $14.99

The final piece when building the true sensing unit will use Arduino Nano.  These beautiful boards are a significantly smaller form-factor and sit right on the breadboard with the rest of the circuits making it much easier to deploy.

11. Power adapter https://amzn.com/B07BXW19R7 $10.99

A dedicated power adapter will be needed for the final deployment so the device will function.  This power adapter comes with a set of tips that allow it to be used with other devices when this project comes to a conclusion, but the most important part is that it comes with a **mini USB** tip which seems practically impossible to find since search engines are optimized to convert "mini" to "micro". :roll\_eyes\_emoji:

12. Total project cost: $114.90

Learning how to work with electronics to create peace of mind: priceless

#### The Build Process

<Talk about how breadboards work, how to assemble the specific components and test there fir this project...>

##### Prototype 1
Just messing with Morse Code using the onboard LED.

##### Prototype 2
4-digit 7-segment.  Trying to light up the digits, going bonkers not understanding why I cannot just set the digits...then illumination!  The LED segments time-share the current from the board in a process called "strobing."  Yeah, going to use SevSeg for this stuff.

##### Prototype 3
Piezoelectric sound sensing was way easy to code than I expected.  Using serial to test out `analogRead()` on the sensor.

##### Prototype 4
Displaying the sensor ADC value on the 4d7sLED display.

##### Prototype 5
A premature optimization: shift registers!  Let's save some pins on the board!

##### Prototype 6
Arduino Nano, duplicating the Uno circuits.

##### Prototype 7 & 8
Space optimizing with jumpers

##### Prototype 9
Using the technique of threading circuits under the display, I discover the minimalist way to connect the display is literally to not use the shift registers at all.  I think I have my final pattern to work with.


## Part 2: Writing the On-Board Software
### Basic Arduino Build
<Quick overview of Arduino IDE basics>

### LED Displays
<This project uses the awesome SevSeg fork SevSegShift, cite/link/credits due>

### Sound Sensing (the really hard part!)
<voltage, adc, decibels>

#### Calibration
<Because this is an analog sensing device, we need to know about calibration>

### ESP8266 and MQTT Client
Connecting the sensor to an MQTT broker was actually a lot harder than it first appeared.


## Part 3: Data Science
### MQTT
This part has a very interesting architecture.  I have Raspberry Pis laying around from previous projects, so I conscripted one of the unused one to act as an MQTT proxy / local network data lake to capture the otherwise unencrypted traffic locally (as opposed to going straight out to the internet; security first, everyone!!!) for forwarding to the ELK stack over a properly encrypted channel.

I'm not normally a fan of using python.  As a matter of fact, I loath the syntax of the language; the whitespace-delimited blocking messes with my eyes (though I have found a compromise to cope with it).  I much prefer the joy of working with Ruby over Python any day of the week, however, Python is the right tool for certain tasks, and gluing data pipelines together is most certainly one of those otherwise painful tasks where Python shines.

#### Broker
The broker is the easiest part of the on-premise data pipeline.  Just install it, `sudo apt install mosquito`, and you're done.  If you need a crash course in MQTT, [HiveMQ](https://www.hivemq.com/) has an _AMAZING_ series of articles on both the [essentials](https://www.hivemq.com/blog/mqtt-essentials-part-1-introducing-mqtt/) and [security](https://www.hivemq.com/tags/iot-security/) of MQTT.  I am using an older version of MQTT, 3.1.1, but if you have the option, you should definitely go with MQTT version 5.

MQTT is a very simple message broker that handles passing messages in near realtime.  While we are going to use QoS level 0 (no guarantees of message delivery), there are two other levels that ensure deliverability of the message, and certain integrity aspects that we don't really need in this project.

<insert quick overview of how MQTT works>

#### Proxy Script
My proxy script is a very linear pipeline.  It connects to the MQTT broker, subscribes to the `/soundsensors/+/+` topic pattern (the `+`s are single level wildcards), receive any retained messages matching that pattern (I retain the state of the sensor, and the last timestamp rollover since Arduinos don't have an internal clock, they use milliseconds that roll over after a while, we'll use that to track the timing), receive sound level messages from the sensor, dump the records into an SQLite3 database, and ship the data over a secure channel to ElasticSearch.

[You can find the proxy script here](https://github.com/omnilord/iot_splmeter/blob/master/mqtt/mqtt_proxy.py).


### ElasticSearch - Indexing Behind the Scene


### Kibana - Dashboards for User Interaction


## Lessons Learned
### Wiring Puzzles
<discuss the optimal usage of space on the breadboard>

### Wifi IoT / Security Concerns
<the title is self-descriptive>

### The Whoops Factor: NodeMCU, ESP32
So, about halfway through this project *after having purchased all the components in Part 1* I discovered there were alternate devices with a WiFi module and LCD integrated into the board.  These ESP32 devices are programmed much the same way you would program an Arduino, but no need to breadboard the wifi or a display.  The first hurdle that justified my continuing to use the Arduinos instead of switch to these more powerful devices was the need for a 5V circuit on the piezoelectric disks.

I eventually stumbled on the NodeMCU style products, and kind had me salty because there are versions that support 5V.  In production runs in the future, these powerful and ecconomic devices will probably be my goto component, if not ESP32s, or whatever their successors may be.