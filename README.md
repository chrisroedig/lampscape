# Lampscape

ESP-8266/MQTT Controlled LED Data Visualization Sculpture

![Lampscape Application Example](https://github.com/chrisroedig/lampscape/blob/master/lampscape.jpeg)

The idea is to have a device that displays relevant information with a pleasant led array animation.
In my case, I 3D printed a translucent "mountain range" and the LEDs illustrate real-time and cumulative power and energy offset from the solar panels on my house.

The project is built using platformIO using an "nodeMCU ESP-8266" board using the Arduino framework. These are super cheap ($4) wifi enabled MCUs that can be programmed with Arduino framework C++ code.

The runtime API is through MQTT. More details coming soon, but here's a general idea:

* lampscape announces itself on boot and waits for commands
* lampascape can receive...
  * animation pattern to display
  * a pallette of available colors
  * parameters relevant to the animation
* lampscape announces all changes in state via MQTT

In my setup I use nodered to collect data and send commands to lampscape.

## Using this Code

This is a very WIP project for my own amusement. There is unfinished, deprecated and generally junky code in here. 
