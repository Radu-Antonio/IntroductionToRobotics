# Homework 5 - Pseudo-Smart Environment Monitor and Logger

I developed a "Smart Environment Monitor and Logger" using Arduino. This system is utilizing various sensors to gather environmental data, log this data into EEPROM, and provide both visual feedback via an RGB LED and user interaction through a Serial Menu. 

# How it works

You have a meniu with options and for each option you go to a submeniu and then you can update values of the sensors, show current readings, reset logged date, etc.

The menu structure is like this:

1. Sensor Settings
  
  - Sensors Sampling Interval. - here you are prompted for a value between 1 and 10 seconds. this value is the sampling rate for the sensors
  
  - Ultrasonic Alert Threshold. - here you are prompted for a threshold value for the ultrasonic sensor. 
  
  - LDR Alert Threshold. - here you are prompted for a threshold value for the LDR sensor.
  
  - Back - return to main menu

2. Reset Logger Data

  - Yes.

  - No.

3. System Status 

  - Current Sensor Readings - continuously print sensor readings at the set sampling rate, from all sensors (exit using any key)

  - Current Sensor Settings - it displays the sampling rate and threshold value for all sensors

  - Display Logged Data - here it displays the last 10 sensor readings for both sensors

  - Back - here you return to main menu
  
4. RGB LED Control 

  - Manual Color Control - here you can set the RGB colors manually

  - LED: Toggle Automatic ON/OFF - switch between automatic mode and manual mode

  - Back - return to main menu

<img src="https://github.com/Radu-Antonio/IntroductionToRobotics/blob/master/homework5/img5.jpeg" alt="image" width="500" height="480"/>

[demo](https://www.youtube.com/watch?v=1rRSC7MKfB4)
