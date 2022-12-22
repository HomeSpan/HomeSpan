::construction::  Under Construction - Solutions Discussed Below are PRELIMINARY and still being TESTED

# Common Problems and Potential Solutions

### *HomeSpan works correctly when my ESP32 is plugged into a computer or separately powered through the USB port, but it fails to work when powered directly through the ESP32 5V pin without any USB connection*

On some ESP32 boards, the USB-UART chip only receives power if power is applied through the USB port, and thus remains unpowered if the ESP32 is powered solely through the ESP32 5V pin.  As a result, the Serial RX pin associated with UART0 on the ESP32, which is normally driven by the USB-UART chip, is free to float at any voltage.  If the pin floats low, this triggers the ESP32 into thinking there is data to be read from the Serial Monitor, and HomeSpan falls into an infinite loop trying to read this non-existent data.  The solution to this problem is to force the UART0 RX pin to a HIGH state when the USB-UART is unpowered.  To do so, simply change the mode of the RX pin to *INPUT_PULLUP* **after** your initial call to `Serial.begin()` as follows:

```C++
#include "HomeSpan.h"

void setup(){
  Serial.begin();
  
  // Choose ONE of the following: 
  
  pinMode(3,INPUT_PULLUP);    // for ESP32 chips, OR
  pinMode(44,INPUT_PULLUP);   // for ESP32-S2 and ESP32-S3 chips, OR
  pinMode(20,INPUT_PULLUP);   // for ESP32-C3 chips
  
  ...
}
```

Alternatively, you can create a generic version of the above code that works for any ESP32 chip by using the macro variable *U0RXD_GPIO_NUM*, which (starting with version 2.0.1 of the Arduino-ESP32 Board Manager) is defined in *soc/uart_pins.h*:

```C++
#include "HomeSpan.h"
#include "soc/uart_pins.h"    // this is where U0RXD_GPIO_NUM is defined for each chip

void setup(){
  Serial.begin();
  pinMode(U0RXD_GPIO_NUM,INPUT_PULLUP);  // works for any ESP32 chip
  
  ...
}
```
