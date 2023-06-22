# Timer Switch with Arduino

## Introduction

I have a usb 2.0 powered external device (A mouse jiggler) that has a simple touch interface based on-off switch. The one that I am using in this project can be purchased off of Amazon [here](https://www.amazon.com/Undetectable-Mover-Parts-Software-Automatically/dp/B08GPGZ4Z6/ref=sr_1_5?keywords=mouse+jiggers&qid=1687389928&sprefix=mouse+jigger%2Caps%2C141&sr=8-5). The switch requires manual intervention for it to be actuated. I want to add in a functionality to the device so that it can be switched off automatically after a certain amount of time has passed.

There's numerous ways to solve this engineering problem. However, the route I chose to go is to not interfere too much with the existing circuitry and on-board microcontroller instead to simply use a micro-controller (like an Arduino Uno) to actuate an electrically controlled switch - which could be a transistor or a relay. The rest of this document details how I went about it.

**Note**: This project can be extended to any external device other than the mouse jiggler I use here. Of course, you would have to assess the current & voltage requirements for your own device which will affect the components you choose for your project.

## Table of Contents

1. [The external device](#the-external-device)
2. [The Idea](#the-idea)
3. [Prototype](#prototype)
    * [Schematic](#schematic)
    * [Calculations](#calculations)
    * [Prototype Build and Test](#prototype-build-and-test)
4. [Final Build](#final-build)
3. [References](#references)

## The external device
This section will simply go over the details of the device I am modifying and the existing features.

I am working with a [mouse jiggler](https://www.amazon.com/Undetectable-Mover-Parts-Software-Automatically/dp/B08GPGZ4Z6/ref=sr_1_5?keywords=mouse+jiggers&qid=1687389928&sprefix=mouse+jigger%2Caps%2C141&sr=8-5) which is a usb 2.0 powered device. It's a relatively simple device with the following working parts:
1. A male usb 2.0 type b to male usb type c cable
    * The cable has a built in touch actuated switch
3. A usb type c powered micro-controller board
4. Motor
5. Casing for components

### Cable
<img src="">

### Built-in micro-controller board
<img src="">

### Motor
<img src="">

### Casing
<img src="">

### Assembled Device
<img src="">

### Simplified device circuit
<img src="">

## The idea
A possible solution is to simply introduce a break in the existing circuit and fill in that break with a electrically controlled switch of some sort that takes commands from an Arduino. The device will continue to be powered by its own usb 2.0 port and the Arduino can run off of its own power supply. This is because the Arduino has a limited output voltage & current from its pins.

The device is powered by a usb 2.0 port which can deliver a maximum of 5 volts and 500 milliAmps. So, our switch must be sufficiently rated for these requirements.

## Prototype

### Schematic

I use a standard transistor as a switch circuit as shown below:
<img src="">

This means I need to source the following additional components:
1. A transistor
2. Two resistors
3. An Arduino device
4. Power supply for Arduino

The critical component here is the transistor which can act has a switch. It is in an ON state when sufficiently saturated and in an OFF state when in cut-off. To select the right transistor make sure the transistor satisfied the following minimum requirements, which are dictated by your external device and the arduino:
1. Maximum Ic (Collector current) >= 500 mA
2. Maximum VCE (Collector Emitter Voltage) >= 5v DC

A great read that helps you understand how to select the right transistor as a switch for your application can be found [here](https://www.nutsvolts.com/?/magazine/article/may2015_Secura).

In my case, I chose to use a BC337 NPN transistor which is rated for max Ic = 800mA DC and max VCE = 45v DC. I use the standard [Arduino Uno Rev3 micro-controller board](https://store-usa.arduino.cc/products/arduino-uno-rev3?selectedStore=us). The values of resistors I need will be decided based on my calculcations below.

### Calculations

Next, I calculated the values for Rc, Ib, and Rc that are needed to build the circuit.

Since, the maximum current requirement for my external device is 500 mA, I chose to design my circuit with an assumed Ic value of 450mA to pass through the Collector Emitter circuit. The only caveat is that a transistor uses up some input voltage (as a fee for providing its switching services) when running in a saturated state. This value for my transistor, as per its datasheet is 0.7v (Vce sat). This would leave about 4.3v to power my external device which I'm guessing should be enough. We can address any issues that may arrive later.
Since the collector current is already limited by the power supply, I choose not to use a current limiting resistor. So, Rc = 0 ohms.

To calculate the value of base current (Ib) to sufficiently saturate the transistor, I use the below:
Ib = (Ic / Bmin) * ODF
Here, Ib = base current; Ic = Collector current; Bmin= Minimum hFe value for my transistor; ODF = Overdrive factor
As per the datasheet for my transistor, Bmin = 60. I chose a value of 2 for the Overdrive factor to ensure the base current provided will be enough to clear the edge of saturation (the point where the transistor just saturates).
Ib = (0.450 / 60) * 2
Ib = 0.015 A
Typically the ODF value can range anywhere from 2 to 10. The higher the value, the more it is guranteed to saturate the transistor. Since an Arduino Uno can only provide a maximum current output of 20 mA, I was restricted to an ODF of 2. A calculated base current value of 15 mA is should be sufficient to saturate the transistor while accounting for the Arduino current output limitations.

Lastly, I compute the resistor value for Rb:
Rb = (Vin - (Vbe sat)) / Ib
As per the datasheet for the transistor, the value of Base Emitter voltage at saturation Vbe sat = 1.2v.
Vin = 5v (power output from Arduino)
Rb = (5 - 1.2) / 0.015
Rb = 253.3 ohms

So, to ensure saturation I need a resistor Rb of value <= 253.3 ohms. Again, I choose to forego using a resistor Rb here because I figure the current is already limited to 20 mA because of the Arduino. Also, once a transistor is saturated any further increase in base current will not have a significant impact. So, Rb = 0 ohms.

### Prototype Build and Test

To build out the prototype, I used the following components:
|No.| Component Name | Description|Quantity|
|---|---             |---         |---     |
|1  |Breadboard      |Standard Size | 1 |
|2  |Transistor      |BC337 NPN| 1 |
|3  |usb type a male to usb type b male | - | 1 |
|4  |22 AWG hookup wire | - | As much as needed |
|5  |Soldering equipment/alligator clips| - | - |
|6  |Arduino Uno     | -| 1|
|7  |Jumper Wires|male to male| As many as needed|

Also, I wrote a simple sketch for the Arduino which can be found [here](https://github.com/arku22/arduino_switch/blob/master/main/main.ino). The logic is depicted in the below flowchart:

This is what the finished prototype looks like:
<img src="">

I read the following values from my build:
Vce sat = ___
Ib = ___

## Final Build

<img src="">

## References

- 
