# Moon-Rover

The aim of this project was to design a remote controlled rover that can explore a lunar terrain and identify rocks based on the signals they emit. This involved splitting up the project into two parts: motion and sensing.

<img width="250" alt="image" src="https://github.com/anish-narain/Moon-Rover/assets/69715492/b8ad9c22-d8c4-4c7e-8f93-808a69b3fd75">


For motion, the rover required motors to drive wheels that enabled forwards, backwards, left, and right movement. Furthermore, the rover’s movements were to be controlled over Wi-Fi. This required using a Wi-Fi module with a Metro M0 microcontroller Board and creating the interface to communicate with the board. The chosen user-interface was an Android application built on MIT App Inventor. The buttons on the app enabled the rover to move in the four directions mentioned above. It also included four additional “orbit” buttons, a scan button, and textboxes for the results of the scan.

For sensing, the rover needed to detect infrared, radio, ultrasound, and magnetic signals. For infrared, a pre-built infrared “flame” sensor was incorporated into an amplifier circuit to detect the relevant 353Hz and 571Hz infrared pulses. For radio waves, an inductor in parallel with a capacitor was used to receive the modulated radio waves. An envelope detector was used to extract the modulating signal, and this was fed through a comparator to amplify and clean up the signal. This enabled detection of radio waves and allowed the identification of their modulated frequencies. The ultrasound waves were detected by taking an ultrasound transmitter/receiver, de-soldering the transmitter component and only using the receiver functionality. Finally, two hall effect sensors incorporated into a differential amplifier circuit were used to detect static magnetic fields and determine their polarity.

Once motor controls and the sensor circuitry were functioning, a chassis was built to incorporate all the hardware onto one structure. Finally, the code for the motors and the sensing circuits were combined. All the components were fine-tuned to remove bugs and the project was complete.
