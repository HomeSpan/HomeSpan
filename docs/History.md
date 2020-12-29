### Background (and the history of how HomeSpan came to be)

:construction:

*(This section under construction)*

As part of a kitchen remodel we selected a <a href="https://zephyronline.com/product/lux-island-range-hood/">Lux Island</a> vent hood fan by Zephyr to install in the ceiling over a kitchen island cooktop.  The hood contains a very powerful 3-speed fan and sports LED lights to illuminate the cooking area below with 3 levels of brightness.  Since the fan is mounted directly in the ceiling, there are no manual controls or buttons to turn it on or off.  Instead, Zephyr provides this stylistic RF remote control you're supposed to place on your countertop:

![Remote Control](images/zephyr-remote.png)

As shown, it's a simple 4-button remote with buttons for power, the fan, the lights, and a timer control.  Unfortunately, it uses a psuedo-touch sensitive mechanism that is so finicky that there is only a 5% chance that it registers when you press the button.  A replacement remote from the manufacturer performed even worse and it was nearly impossible to get the remote to detect when you pressed one of the buttons.  Rather than ask the manufacturer for another replacement, I decided to try my hand at creating my own RF remote.

Reverse-engineering the RF signals was quite straigtforward.  Opening the remote revealed it was using a standard 433.92 MHz oscillator (it was printed on the oscillator itself).  With the help of a simple Arduino sketch and an Arduino Mega to perform rapid analog reads, judicious use of the the Arduino Serial Plotter (a terrific feature of the Arduino IDE!), and a simple [434 MHz receiver](https://www.sparkfun.com/products/10532) from SparkFun, I was able to decode the RF signals.

Each button produced a fixed pattern of 20 high/low pulses, each lasting 850𝛍s. The same sequence is repeated 8 times, separated by a 4ms delay.  There are two types of pulses:  one is 230𝛍s high followed by 620𝛍s low, which I arbitrarily took to represent a 0-bit; the other is 620𝛍s high followed by 230𝛍s low, which I therefore took to represent a 1-bit.  Comparing the resulting 20-bit pattern for each button suggested which bits may be related to the address of the device, and which were used to trigger specific functions (power, fan, light, timer).  Since I was not looking to create a universal remote, determining exactly which bits represented the device address and which represented the functions did not matter.  All I needed was to be able to reproduce the exact bit patterns to control the vent hood.

A slightly more complicated Arduino sketch (still using the Arduino Mega), a few pushbuttons, and a simple [434 MHz transmitter](https://www.sparkfun.com/products/10534) from SparkFun and I had myself a fully working remote control to replace the stylistic, though useless, Zephyr remote.

### First Attempts - Connecting to HomeKit via HomeBridge

I'm a huge HomeKit fan and have 100+ HomeKit-enabled Lutron wall dimmers, wall switches, and lamp dimmers, so connecting my kitchen vent fan to HomeKit was a must.  After searching the web I stumbled onto the Apples HAP documentation for HomeKit and found it a bit overwhelming.  Also, I quickly realized that an Arduino Mega would not have the computational power, speed, or memory, to implement the HAP protocol.  However, while searching for solutions I came across a superb open-source project called HomeBridge that seemed to fit the bill.  HomeBridge acts as a HomeKit bridge and allows you to create your own accessories, though many of the examples seem to be use HomeBridge as a true bridge to connect to other devices that, in turn, control a real-world applicance.

I decided to install HomeBridge on a standalone Raspberry PI.  My intention was to create code to control an RF transmitter on the PI the same way I controlled it on the Arduino Mega.  HomeBridge itself is written in Javascript and uses a Javascript interpreter called Node.js.  I commonly program in C++ (and sometimes Java) but it was years since I used Javascript (and that was only for web-page design).  The learning curve was a bit steep, but eventually I managed to install run, and configure HomeBridge.  Next step was to develop the code to control the RF transmitter, and this is where I ran into a wall.

The Raspberry PI is a terrfic microcomputer.  The problem is that it is a *microcomputer*, not a *microcontroller*.  Though it has numerous input/output pins, the microprocerssor is not designed to allow fine-grained control over the pins.  There are numerous libraries that can be used to control the pins, but I'm not a big fan of using software to generate real-time sifgnals, especially in a multi-threaded system like Linux, which is often juggling many different tasks in the background.  I was used to programming the hardware timers embedded in the ATmega328P (Arduino Uno) and ATmega2560 (Arduino Mega) chips to generate custom PWM signals with 1𝛍s accuracy.  Using "delay" functions in software to create such signals would probably be fine on a device as fast as the Raspberry PI, but I didn't want to go down that path only to find out later that once every 20 times the remote did not work because of a timing glitch on the PI as it had decided to access the WiFi while transmitting the RF code to power up the vent fan.

So instead, I followed a number of the examples provided by other HomeBridge users and configured HomeBridge to connect to the Arduino Mega, which would in turn generate the RF signals.  To do so, I created a simple web-server on the Mega, and had HomeBridge send an HTTP GET request to the Arduino web-server instructing it to turn on the fan and lights whenever it received a HomeKit command.  In principle this should have been straighforward, but real-world appliances don't always fit neatly into the HomeKit model of the universe.

### The challenges of real-world appliances

As mentioned, the Zephyr remote control has four buttons: power, fan, light, and timer.  The timer button simply sets a timer, after which the power shuts off (and is not needed with HomeKit since you can create your own scenes and automations).  The other buttons have more complex operations.

If both the fan and light are off:

* a press of the power button turns on the fan to low speed;
* a press of the fan button turns on the fan to low speed;
* a press of the light button turns on the light to high brightness.

Pressing the light button when the light is on causes the brightness to cycle high → medium → low → off, and then back to high.  Pressing the fan button when the fan is running causes the speed to cycle low → medium → high, and then back to low.  There is no off setting when pressing the fan button.  To turn the fan off, you need to press the power button.  Which also turns off the light.  Which means the light controls and the fan controls are actually coupled in a somewhat complex way.  If the fan is off and the light is off, the fan comes on when you press the power button.  But if the light is on and you press the power button, the light turns off, and nothing happens to the fan.  This makes for some complicated logic.  Even worse, there is no way to directly set the speed of the fan or the brightness of the light - you can only cycle through the different levels with repeated button presses.

I double-checked to see if this is a limitation of the Zephyr remote, or a limitation of the Zephyr hood itself.  Perhaps there are other RF codes that can be used to operate the fan and light with more granularity?  I tried sending a whole range of other RF codes to the hood, but I could not find any hidden features.

Alas, the controls for what appeared to be a simple fan/light appliance where in reality more complex and convoulted than HomeKit's somewhat idealized fan and light controls.

Fortunately, it is possible to program such logic it you are willing to compromise on some things (such as directly setting the brightness of the light).  Part of the logic I needed to program in Javascript so it could run within HomeBridge.  The other other part was in C++ on the Arduino Mega, made even more complex by the fact that the Mega was also running a web-server just to listen from commands from HomeBridge.

However, it all seemed to work fine most of the time.  Sometimes the Raspberry PI would crash and needed to be rebooted.  Sometimes commands transmitted from the PI to the Mega over HTTP were dropped.  Though I was pleased I could control my vent hood fan and light with HomeKit (for the most part), I found the whole setup unsatisfactory, overly complex, and not very scalable.  There were other appliances I wanted to connect to HomeKit, but instead I was spending time on updating device drivers for the Raspberry PI and installing Linux patches to try to fix WiFi dropouts.

### Creating HomeSpan

For a host of reasons I finally decided I had enough of the Raspberry PI / HomeBridge method.  It was time to bite the bullet and develop my own HomeKit library.  My main requirement was that it had to be rock-solid stable.  It also needed to be flexible enough to allow me to create my own logic to make up for the shortcomings of HomeKit itself.  This meant *not* using any other HomeKit libraries or SDKs.  I began by testing different devices, mostly from Arduino since I was very familar with the platform from a previous open-source project, but I could not find a device the had type of WiFi stability and functionality needed.

I finally settled on the ESP32 since:

* It contains a plethora of hardware level peripherals that can be used to generate custom signals
* It contains two processors so WiFi is rock-solid stable even when I'm running very complex code (or generating custom signals)
* It... 


