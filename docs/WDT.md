# HomeSpan Watchdog Timer

The ESP32 operating system supports a task-based watchdog timer that checks to ensure tasks (or any select section of code) that subscribes to the task watchdog is being periodically run.  If not, then after a pre-specified period of time, the device automatically reboots.

Typically, it is only the background IDLE processes that subscribe to the task watchdog, but by default the Arduino-ESP32 library does NOT enable the IDLE task watchdog for single-core chips (ESP32-S2, C3, and C6) and only enables the IDLE task watchdog associated with Core-0 for dual-core chips (ESP32 and ESP32-S3).  The Arduino-ESP32 library default is to set this task watchdog timeout to 5 seconds.

Based on these defaults, if the your sketch hangs, or otherwise goes into an infinite loop, the device will be frozen indefinitely until you manually reset it or cycle the power.

Fortunately, you can enable an optional watchdog timer that specifically checks to ensure all needed HomeSpan tasks and functions are running as expected.  To do so, simply call the homeSpan  method `enableWatchdog(uint16_t nSeconds)` near the top of the `setup()` portion of your sketch.   Once enabled, the task watchdog will time-out and automatically reboot the device after *nSeconds* unless the watchdog timer has been reset.  HomeSpan resets its watchdog timer automatically upon reaching the very end of the HomeSpan `poll()` function.  As long as the end of the `poll()` function is reached every *nSeconds* seconds, the watchdog will be silent.  But if the `poll()` function hangs, or another process blocks the `poll()` function from even being called, the watchdog timer will trigger after *nSeconds* seconds and the device will reboot.

By setting *nSeconds* to a time that is sufficiently long to allow all of your setup process to run (perhaps 10-20 seconds), you can be assured that if any of the HomeSpan process are caught in an infinite loop, the device will reboot on its own.  This is particularly helpful when used in conjunction with OTA Rollbacks since you can configure things such that if you upload a new sketch via OTA and it hangs on start-up, it will reboot after a short period of time and rollback to the prior working version of the sketch.  See the [HomeSpan OTA](OTA.md) page for details on how to enable rollbacks.

If for whatever reason you need to perform a function within the HomeSpan framework that will prevent the `poll()` task from running for an extended period of time (i.e. longer than *nSeconds*) you can call `homeSpan.disableWatchdog()` to temporarily disable the HomeSpan watchdog and prevent a reboot.  Then simply call `homeSpan.enableWatchdog()` once again when your function is complete to re-enable the HomeSpan watchdog.  See the [HomeSpan API Reference](Reference.md) for details on the `enableWatchdog()` and `disableWatchdog()` methods.

### Implementation Notes



---

[↩️](../README.md) Back to the Welcome page
