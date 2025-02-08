# HomeSpan Watchdog Timer

The ESP32 operating system supports a task-based watchdog timer (TWDT) that checks to ensure tasks (or any select section of code) that subscribes to the task watchdog is being periodically run.  If not, then after a pre-specified period of time, the device automatically reboots.

Typically, it is only the background IDLE processes that subscribe to the TWDT, but by default the Arduino-ESP32 library does NOT enable the IDLE task watchdog for single-core chips (ESP32-S2, C3, and C6) and only enables the IDLE task watchdog associated with Core-0 for dual-core chips (ESP32 and ESP32-S3).  The Arduino-ESP32 library default is to set the TWDT timeout to 5 seconds.

Based on these defaults, if the your sketch hangs, or otherwise goes into an infinite loop, the device will be frozen indefinitely until you manually reset it or cycle the power.

Fortunately, you can add an optional watchdog to the TWDT that specifically checks to ensure all needed HomeSpan tasks and functions are running as expected.  To do so, simply call the homeSpan  method `enableWatchdog(uint16_t nSeconds)` near the top of the `setup()` portion of your sketch.   Once enabled, the TWDT will time-out and automatically reboot the device after *nSeconds* unless the HomeSpan watchdog has been reset.  HomeSpan resets its watchdog automatically upon reaching the very end of the HomeSpan `poll()` function.  As long as the end of the `poll()` function is reached every *nSeconds* seconds, the TWDT will be silent.  But if the `poll()` function hangs, or another process blocks the `poll()` function from even being called, the TWDT will trigger after *nSeconds* seconds and the device will reboot.

By setting *nSeconds* to a time that is sufficiently long to allow all of your setup process to run (perhaps 10-20 seconds), you can be assured that if any of the HomeSpan process are caught in an infinite loop, the device will reboot on its own.  This is particularly helpful when used in conjunction with OTA Rollbacks since you can configure things such that if you upload a new sketch via OTA and it hangs on start-up, it will reboot after a short period of time and rollback to the prior working version of the sketch.  See the [HomeSpan OTA](OTA.md) page for details on how to enable rollbacks.

If for whatever reason you need to perform a function within the HomeSpan framework that will prevent the `poll()` task from running for an extended period of time (i.e. longer than *nSeconds*) you can call `homeSpan.disableWatchdog()` to temporarily disable the HomeSpan watchdog and prevent a reboot.  Then simply call `homeSpan.enableWatchdog()` once again when your function is complete to re-enable the HomeSpan watchdog.  See the [HomeSpan API Reference](Reference.md) for details on the `enableWatchdog()` and `disableWatchdog()` methods.

### Implementation Notes

* Internally, HomeSpan call the homeSpan `resetWatchdog()` method to reset its watchdog at the end of the `poll()` function.  HomeSpan also embeds calls to `resetWatchdog()` at the end of any other HomeSpan function that, by design, temporarily blocks the `poll()` function, such as when you are typing your WiFi Credentials into the Serial Monitor, you've used the Control Button to place HomeSpan into Command Mode, or you've started the HomeSpan Setup Access Point.  This extra embedded calls ensure the TWDT will not trigger while these other functions are running, but the TWDT will indeed trigger and reboot the device if those functions themselves hang.
  
* The `resetWatchdog()` method serves two purposes:  it resets the HomeSpan watchdog (if it is enabled) and it calls `vTaskDelay(1)` (regardless of whether or not the HomeSpan watchdog is enabled).  The call to `vTaskDelay` cause HomeSpan to yield for 1 ms so that other separate tasks of the same or lower prioriy (such as the IDLE tasks) get an opportunity to run.

* The homeSpan `resetWatchdog()` method is public and you can call it yourself from within your sketch if needed (though this is uncommon).  For example, if your sketch runs the HomeSpan `poll()` function by calling `homeSpan.poll()` from within the main Arduino `loop()` and you add code into to the `loop()` that takes more than *nSeconds* to run, you should either call `homeSpan.disableWatchdog()` before that code is run, or call `resetWatchdog()` from within your long-running code in a place that is ony reached if your code is running correctly, but will not be called if your code hangs.  This effectively allows the TWDT to monitor your long-running code and trigger a reboot if it hangs.

* If instead of calling `homeSpan.poll()` from within the Arduino `loop()` function your sketch calls `homeSpan.autoPoll()` to launch HomeSpan polling in a separately-running task then you do not have to worry about creating any delays in code you add to the Arduino `loop()` function since it will not block HomeSpan `poll()` from running (though it may block other important operating system processes).

* However, even if you use `homeSpan.autoPoll()` to run HomeSpan polling in its own task, you still need to make sure you don't add any long-running code to the Arduino `setup()` function that delays the initial launch of polling by more than *nSeconds* seconds from the point you call `homeSpan.enableWatchdog()`.

---

[↩️](../README.md) Back to the Welcome page
