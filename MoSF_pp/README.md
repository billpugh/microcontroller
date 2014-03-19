Light controll for MoSF Photon Probe

4 components. Each one is controlled by one pin.

Teensy digitial pin assignments

Inputs controls

- pin 1 -  Warp engine
- pin 3 - onboard computer
- pin 5 - particle collector
- pin 7 - gaseous anomaly collector

Outputs:

- pin 14 - gaseous anomaly collector
- pin 16 - particle collector
- pin 18 - onboard computer
- pin 20 - Warp core
- pin 22 - Fuel tanks

**Warp engine**

* 2 Fuel tanks, 12", 18 LEDs each, connected to same pin
* 1 warp core, 8 LED stick

When off:

 - fuel tanks off
 - warp core is dim blue pulses, 1 sec for pulses to travel length of LED stick
 
When on:

 - fuel tanks red
 - warp core is blue pulses over grey background, 0.5 sec for pulses to travel length of LED stick
 
 
When Pin 1 turns on:

 - 5 second ramp up
 
When turns off:

 - 2 second ramp down 
 

**Onboard computer**

8 LED stick

When on: 

**particle collector**

12 LED circle plus center LED which is LED 0

When off:

- low steady green glow

When on:

 - center light pulses
 - green particles chase, constant direction and speed


**gaseous anomaly collector**

???
