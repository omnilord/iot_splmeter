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
- Arduino UNO R3: https://amzn.com/B07BTB3N3J $29.99
- Piezoelectric Vibration Sensor: https://amzn.com/B08211CMSP $6.99
- Extra Shift Registers: https://amzn.com/B07HFWB9L9 $7.49
- Extra Resistors: https://amzn.com/B06WRQS97C $6.49
- Large, 4-digit 7-segment LED Displays: https://amzn.com/B07J2PX5XM $5.99
- ESP8266 WiFi modules: https://amzn.com/B01N98BTRH $12.99
- ESP8266-Breadboard adapters: https://amzn.com/B01G6HK3KW $6.99
- Breadboard Jumpers: https://amzn.com/B07PQKNQ22 $12.99
- Extra Breadboards + Dumont Cables (for final product): https://amzn.com/B07H7V1X7Y $9.99
- 3x Arduino Nano (for final product): https://amzn.com/B07G99NNXL $14.99

Total project cost: $114.90

Learning how to work with electronics to create peace of mind: priceless

1. The Arduino UNO R3 kit is a a great starting place to pick up electronics since it comes with a bunch of the essentials for prototyping various circuits.  First, it comes with the all-important Arduino Microcontroller (MCP) board that is the the centerpiece.  This beautiful palm-sized device is the hub upon which all circuits spin.  You upload your programs to this device, you plug in your components to this device, it's everything.  Second, if the Arduino is the hub, then the breadboard is the wheel.  The breadboard is how everything beyond super basic components gets wired together (as we'll see with the LED display later).

2. The Piezoelectric vibration sensor is the fundamental peripheral for a home made SPL meter as it is the workhorse that will be capture the vibrations we intend to measure.  These super light-weight, seemingly flimsy pieces of metal-bonded-to-ceramic are absolutely amazing when you dig into the physics behind what makes them work.  The long story short is they convert changes in pressure into voltage; sometimes significantly high levels of voltage, up to 50V(!!!), so mind that you use resistors properly to protect your 10 fold less--only 5V for the Arduino UNO--circuits).

3. This project requires 2 shift registers per sensing unit that has an LED.  The package I purchased came with a whooping 25, so I guess I have plenty for extra projects.  These neat little chips allow you to extend the UNO registers using only 3 of the onboard digital pins.

4. Each LED is a 7 segment (8 when you include the decimal) so we need 8x 220 Ohm per unit to protect and regulate the display.  The substantial amount of resistors you can get for a few bucks in mind boggling...if you are following along, you are now the proud owner of hundreds of electronic components you might never needs.

5. Each unit will need one 4-digit 7-segment LED display to allow us to see at the instantaneous reading off the piezoelectric sensor.

6. An ESP8266 will allow us to connect to a Wireless network and transmit the readings over MQTT for long-term logging and data analysis.

7. These adapters will allow us to easily connect the ESP8266 to the circuit without having to worry about all the extra dumont cables that will eventually come loose.

8. Breadboard jumpers will come in handy for the final build.  You will see in my prototype that I formed 14 gauge steel wire by hand to craft jumpers.  These bespoke jumpers worked really well, but they were almost too large for the breadboard connection points and I am worried that I may have damaged the plastic with them.

9. For the final build, I will need 3 full-sized breadboards to build out the sensors I will leave connected to my windows.  I toyed with the idea of actually getting solderable boards and just making permanent circuits, but this will be faster.

10. The final piece when building the true sensing units will use Arduino Nanos.  These beautiful boards are a significantly smaller form-factor and sit right on the breadboard with the rest of the circuits making it much easier to deploy.

#### The Build Process

<Talk about how breadboards work, how to assemble the specific components and test there fir this project...>

## Part 2: Writing the On-Board Software ==
### Basic Arduino Build
<Quick overview of Arduino IDE basics>

### LED Displays
<This project uses the awesome SevSeg fork SevSegShift, cite/link/credits due>

### Sound Sensing (the really hard part!)
<voltage, adc, decibels>

#### Calibration
<Because this is an analog sensing device, we need to know about calibration>

### ESP8266 and MQTT Client


## Part 3: Data Science
### MQTT Broker - Processing to Storage
This part has a very interesting architecture.  I have Raspberry Pis laying around from previous projects, so I conscripted one of the unused one to act as an MQTT proxy / local network data lake to capture the otherwise unencrypted traffic locally (as opposed to going straight out to the internet; security first, everyone!!!) for forwarding to the ELK stack over a properly encrypted channel.

I'm not normally a python guy.  As a matter of fact, I loath the syntax of the language.  I much prefer the joy of working with Ruby over Python any day of the week, however, Python is the right tool for certain tasks, and gluing data pipelines together is most certainly one of those otherwise painful tasks where Python shines.


### ElasticSearch - Indexing Behind the Scene


### Kibana - Dashboards for User Interaction


## Lessons Learned
