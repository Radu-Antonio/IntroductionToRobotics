# Extra homework 1 - Arduino alarm system

I developed an alarm system using Arduino. This alarm gets triggered when a person gets too close to the system and and in order to stop the alarm you need to introduce a secret code

# How it works

The system reads values from the ultrasonic sensor, once the distance is too small it means that the person is too close and the alarm will start.

Once the alarm starts, the rgb led will be red the a buzzer will make noise.

To stop the alarm you need to press the buttons in a specific order (1 1 3 2) and then the noise will stop and it goes back to the intial state.

If you introduce a wrong sequence the sound will get even worse.

<img src="https://github.com/Radu-Antonio/IntroductionToRobotics/blob/master/extrahomework1/img.jpeg" alt="image" width="580" height="480"/>

[demo](https://www.youtube.com/watch?v=OW2HsRVJ_-I)

[code](https://github.com/Radu-Antonio/IntroductionToRobotics/blob/master/extrahomework1/extrahomework1.ino)