# Homework 4 - Stopwatch Timer

Using the 4 digit 7 segment display, I implemented a stopwatch timer that counts in 10ths of seconds and it has a pause, a reset and a save lap button

# How it works

- Button 1: start / pause
– Button 2: reset (if in pause mode) / reset saved laps (if in lap mode)
– Button 3: save lap (if in counting mode) / cycle through saved laps (max 5)

1. Display shows ”000.0”. When pressing the Start button, the timer starts
2. During timer counter, each time I press the lap button, the time gets saved; If I press the reset button while timer works, nothing happens, if I press the pause button, the timer stops
3. In Pause Mode, the lap button doesn’t work anymore. Pressing the reset button resets to 000.0
4. After reset, I can press the flag buttons to cycle through the lap times. Pressing the reset button while in lap state resets all the times saved and takes the timer back to ”000.0”.

<img src="https://github.com/Radu-Antonio/IntroductionToRobotics/blob/master/homework4/img4.jpeg" alt="image" width="620" height="460"/>

[demo](https://www.youtube.com/watch?v=S-c_NVw9IUQ)

[code](https://github.com/Radu-Antonio/IntroductionToRobotics/blob/master/homework4/homework4.ino)
