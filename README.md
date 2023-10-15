# Car-Safety-System
Car Safety System implemented using Arduino

### Problem Statement
Statistics show that 94% of fatal road accidents occur due to human error.   
This project aims to reduce such incidents with the help of a hardware implementation which includes:   
1. Alcohol detection: An MQ3 Alcohol sensor that helps to detect the presence of alcohol in the vehicle, in the vicinity of the driver.   
2. Sleep detection: An IR Eye Blink sensor which counts the number of eye blinks of the driver to detect fatigue.   
3. Proximity detection: An IR Proximity sensor is used to check the nearness of the vehicle to various obstacles in its path.   

All these sensors work in unison to ensure the safety of the driver and the passengers. On the activation of any of the first two sensors, a buzzer is activated and the driver is forced to adopt corrective measures. The proximity detection sensor, on the other hand, brings the car to a stop as part of safety.

> Details of implementation available in [car_safety.md](/car_safety.md)   
Source code available at [source_code.ino](/source_code.ino)
