# HomeSpan Watchdog Timer

The ESP32 operating system supports a task-based watchdog timer that checks to ensure tasks (or any select section of code) that subscribes to the task watchdog is being periodically run.  If not, then after a pre-specified period of time, the device automatically reboots.

Typically, it is only the background IDLE processes that subscribe to the task watchdog, but by default the Arduino-ESP32 library does NOT subscribe the IDLE task to the watchdog for single-core chips (ESP32-S2, C3, and C6) and only subscribes the IDLE process associated with Core-0 to the task watchdog for dual-core chips (ESP32 and ESP32-S3).  The task watchdog timeout (when used with dual-core chips) is typically set to 5 seconds by the Arduino-ESP32 library.

Based on these defaults, if a HomeSpan sketch hangs when run on a single-core chip, or if it hangs when run on Core-1 of a dual-core chip (the normal default), the device will be frozen indefinitely.

Fortunately, you can set an optional watchdog timer that specifically checks to ensure HomeSpan tasks and functions are periodically running as expected by calling the homeSpan  `enableWatchdog(uint16_t nSeconds)` method near the top of the `setup()` portion of your sketch.   When enabled, the task watchdog will time-out and automatically reboot the device after *nSeconds* if its watchdog is not reset by calling `homeSpan.resetWatchdog()`.  However, you **do not** have to call `resetWatchdog()` yourself - the HomeSpan library already embeds these calls in all HomeSpan functions as needed, such as at the end of the `poll()` task.

By setting *nSeconds* to a time that is sufficiently long to allow all of your setup process to run (perhaps 10-20 seconds), you can be assured that if any of the HomeSpan process are caught in an infinite loop, the device will reboot on its own.  This is particularly helpful when used in conjunction with OTA Rollbacks since you can configure things such that if you upload a new sketch via OTA and it hangs on start-up, it will reboot after a short period of time and rollback to the prior working version of the sketch.  See the [Over-the-Air (OTA) Updates](OTA.md) page for details on how to enable rollbacks.

If for whatever reason you need to perform a function within the HomeSpan framework that will prevent the `poll()` task from running withingi *nSeconds*, you can call `homeSpan.disableWatchdog()` to disable to watchdog and prevent a reboot, and then call `homeSpan.enableWatchdog()` once again to re-enable when your function is completed.  Alternatively, you can add periodic calls to `homeSpan.resetWatchdog()` from within your function to prevent the task watchdog from timing out. Note that if your function prevents the HomeSpan `poll()` task for more than a few seconds, this will lead the Home App to report "No-Response" for your device.

---

[↩️](../README.md) Back to the Welcome page
