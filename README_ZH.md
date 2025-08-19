# 欢迎！

> **语言**: [English](README.md) | **中文**

欢迎使用 HomeSpan - 一个强大且极其易用的 Arduino 库，用于创建基于 [ESP32](https://www.espressif.com/en/products/modules/esp32) 的 HomeKit 设备，完全在 [Arduino IDE](http://www.arduino.cc) 中开发。

HomeSpan 提供了专为 Espressif ESP32 微控制器设计的 Apple HomeKit 配件协议规范 Release R2 (HAP-R2) 的微控制器实现，在 Arduino IDE 环境中运行。HomeSpan 可以直接通过您家中的 WiFi 网络与 HomeKit 配对，无需任何外部桥接器或组件。使用 HomeSpan，您可以充分利用 ESP32 的 I/O 功能来创建自定义控制软件和/或硬件，从而通过 iPhone、iPad 或 Mac 上的家庭 App，或通过 Siri 自动操作外部设备。

运行 HomeSpan 的要求取决于您选择的版本：

|HomeSpan 版本 | Arduino-ESP32 板管理器 | 分区方案 | 支持的芯片|
|:---:|:---:|:---:|---|
|1.9.1 或更早版本 | v2.0.0 - v2.0.17 | *默认* (1.3MB APP) | ESP32, S2, S3, C3 |
|2.0.0 或更新版本 | v3.0.2 - **v3.3.0**<sup>*</sup> | *最小 SPIFFS* (1.9MB APP) | ESP32, S2, S3, C3, *以及 C6* |

<sup>*</sup>HomeSpan 已经测试到 Arduino-ESP32 板管理器的 **3.3.0 版本**（基于 IDF 5.5.0 构建）。更新的版本可能工作正常，但尚未经过测试。请注意，HomeSpan 不支持使用 Arduino-ESP32 板管理器的 alpha、beta 或预发布候选版本 - 仅在板管理器的生产版本上进行测试。

**附加要求**：Apple 的 HomeKit 架构[需要使用家庭中枢](https://support.apple.com/zh-cn/HT207057)（HomePod 或 Apple TV）才能正常完整地操作任何 HomeKit 设备，包括基于 HomeSpan 的设备。***不支持在没有家庭中枢的情况下使用 HomeSpan。***

### HomeSpan 亮点

* 提供自然、直观且**非常**易用的框架
* 采用独特的*以服务为中心*的方法来创建 HomeKit 设备
* 充分利用广受欢迎的 Arduino IDE
* 100% HAP-R2 合规性
* 数十种集成的 HomeKit 服务
* 支持配件模式或桥接模式运行
* 支持使用设置代码或二维码配对
* 支持 WiFi 和以太网连接到家庭网络
* 支持 IPv4 和 IPv6 地址的双栈使用

### 对于 HomeSpan 开发者

* 广泛使用 Arduino 串行监视器
  * 实时、易于理解的诊断信息
  * 对每个底层 HomeKit 操作、数据请求和数据响应的完全透明
  * 命令行界面，提供各种信息、调试和配置命令
* 内置数据库验证，确保您的配置满足所有 HAP 要求
* 专用类，利用 ESP32 的 16 通道 PWM 外设轻松控制：
  * LED 亮度（包括自动淡入淡出）
  * 舵机电机
* 集成的按钮和切换开关功能，支持以下设备的单击、双击和长按：
  * 将 ESP32 引脚连接到地或 VCC 的物理按钮
  * 连接到 ESP32 引脚的触摸板/传感器（适用于支持触摸板的 ESP32 设备）
* 集成访问 ESP32 的片上遥控外设，轻松生成 IR 和 RF 信号
* 专用类来控制单线和双线可寻址 RGB LED 和 LED 灯带
* 专用类来控制步进电机，可以在后台平稳运行而不干扰 HomeSpan
* 专用类，使用 ESP-NOW 促进 ESP32 设备之间的无缝点对点通信
* 集成的 Web 日志用于用户定义的日志消息
* 详细注释的教程示例，从 HomeSpan 的基础知识到高级 HomeKit 主题
* 展示 HomeSpan 实际应用的附加示例和项目
* 解释 HomeSpan API 各个方面的完整文档

### 对于 HomeSpan 终端用户

* 嵌入式 WiFi 接入点和 Web 界面，允许终端用户（非开发者）：
  * 使用自己的家庭 WiFi 凭据设置 HomeSpan
  * 创建自己的 HomeKit 配对设置代码
* 状态 LED 和控制按钮，允许终端用户：
  * 强制从 HomeKit 取消配对设备
  * 执行出厂重置
  * 启动 WiFi 接入点
* 独立、详细的终端用户指南

## ❗最新更新 - HomeSpan 2.1.3 (2025年8月2日)

### 更新和修正

* **添加了对 IPv6 地址的支持**
  * 通过在草图中添加 Arduino-ESP32 函数 `WiFi.enableIPv6()` 或 `ETH.enableIPv6()` 可以启用 IPv6
  * 启用 IPv6 时，HomeSpan 在串行监视器和 Web 日志中报告 IPv6 **唯一本地地址 (ULA)** 以及 IPv4 地址（如果未启用 IPv6，IPv6 地址报告为 "::"）
  * 每个获取的 IP 地址（无论是 IPv6 还是 IPv4）都会在从路由器接收时记录到串行监视器和 Web 日志
  * 请注意，如果使用 `homeSpan.setConnectionCallback()` 在初始 WiFi 或 ETH 连接时设置回调函数，回调函数只在从路由器接收到的第一个 IP 地址时调用一次（无论是 IPv4 还是 IPv6 地址）
  * 详情请参见 [WiFi 和以太网连接](docs/Networks.md)

* **更新了 HomeSpan 接入点代码，以（希望）解决之前阻止 HomeSpan 设置页面在非 Apple 设备上显示的问题**

* **修复了在 HomeSpan 2.1.2 中重构 JSON 解析器时引入的 HAP 定时写入 PID 解释错误**

* **添加了新的 *homeSpan* 方法 `forceNewConfigNumber()`**
  * 包含在草图中时，这会强制 HomeSpan 在启动时更新数据库配置号，以及在调用 `homeSpan.updateDatabase()` 时更新，无论数据库配置是否有任何更改
  * 此函数的目的是尝试鼓励 HomeKit 后端架构更快地重新建立与已重启的 HomeSpan 设备的连接，而无需用户打开家庭 App（在这种情况下 HomeKit 会立即连接到设备）
  * 以这种方式提示 HomeKit 的成功有限（希望 Apple 会在 iOS26 中更普遍地解决这个缺陷）

### 兼容性问题

* **解决了在 Arduino-ESP32 版本 3.2 或更高版本下运行时，HomeSpan 的 *LedPin*、*RFControl* 和 *Pixel* 模块的兼容性问题，这是由于 Espressif 在 IDF 5.4 中向各种 *LEDC* 和 *RMT* 配置结构添加了新字段导致的**
  * 这些模块中的初始化例程已修改为始终预清除所有相关的 IDF 配置结构，以便（希望）如果/当 Espressif 在后续 IDF 更新中添加任何附加配置字段时，此类问题不会重新出现

* **解决了在 IDF 5.5 / Arduino-ESP32 3.3.0 下 ESP-NOW 回调函数签名更改的兼容性问题**
  * 根据 ESP-NOW IDF 5.5 [破坏性更改](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/migration-guides/release-5.x/5.5/wifi.html)调整了回调签名

有关此更新中包含的所有更改和错误修复的详细信息，请参见[发布说明](https://github.com/HomeSpan/HomeSpan/releases)。

# HomeSpan 资源

HomeSpan 包含以下文档：

* [HomeSpan 入门指南](docs/GettingStarted.md) - 设置开发 HomeSpan 设备所需的软件和硬件
* [HomeSpan API 概述](docs/Overview.md) - HomeSpan API 概述，包括开发第一个 HomeSpan 草图的分步指南
* [HomeSpan 教程](docs/Tutorials.md) - HomeSpan 教程草图指南
* [HomeSpan 服务和特征](docs/ServiceList.md) - HomeSpan 支持的所有 HAP 服务和特征列表
* [HomeSpan 配件类别](docs/Categories.md) - HomeSpan 定义的所有 HAP 配件类别列表
* [HomeSpan 命令行界面 (CLI)](docs/CLI.md) - 从 Arduino IDE 串行监视器配置 HomeSpan 设备的 WiFi 凭据、修改其 HomeKit 设置代码、监视和更新其状态，以及访问详细的实时设备诊断
* [HomeSpan WiFi 和以太网连接](docs/Networks.md) - HomeSpan 的 WiFi 和以太网连接选项的高级讨论
* [HomeSpan 用户指南](docs/UserGuide.md) - 如何配置已编程的 HomeSpan 设备的 WiFi 凭据、修改其 HomeKit 设置代码以及将设备配对到 HomeKit 的交钥匙说明。无需计算机！
* [HomeSpan API 参考](docs/Reference.md) - HomeSpan 库 API 的完整指南
* [HomeSpan 二维码](docs/QRCodes.md) - 创建和使用二维码配对 HomeSpan 设备
* [HomeSpan OTA](docs/OTA.md) - 直接从 Arduino IDE 通过空中升级您的草图，无需串行连接
* [HomeSpan 看门狗定时器](docs/WDT.md) - 可选保护，如果您的草图长时间挂起或冻结，可以触发自动重启
* [HomeSpan PWM](docs/PWM.md) - 使用 ESP32 的片上 PWM 外设集成控制标准 LED 和舵机电机
* [HomeSpan RFControl](docs/RMT.md) - 使用 ESP32 的片上 RMT 外设轻松生成 RF 和 IR 遥控信号
* [HomeSpan Pixels](docs/Pixels.md) - 集成控制可寻址单线和双线 RGB 和 RGBW LED 及 LED 灯带
* [HomeSpan 步进电机控制](docs/Stepper.md) - 集成控制步进电机，包括 PWM 微步进
* [HomeSpan SpanPoint](docs/NOW.md) - 使用 ESP-NOW 促进 ESP32 设备之间的点对点双向通信
* [HomeSpan 电视服务](docs/TVServices.md) - 如何使用 HomeKit 的未记录电视服务和特征
* [HomeSpan 消息日志](docs/Logging.md) - 如何生成日志消息以在 Arduino 串行监视器上显示以及可选地发布到集成的 Web 日志页面
* [HomeSpan TLV8 特征](docs/TLV8.md) - 用于创建 TLV8 对象以与基于 TLV8 的特征一起使用的类和方法
* [HomeSpan 设备克隆](docs/Cloning.md) - 无缝地用新设备替换损坏的设备，无需重新配对和丢失 HomeKit 自动化
* [HomeSpan 项目](https://github.com/topics/homespan) - HomeSpan 库的实际应用
* [HomeSpan 常见问题](docs/FAQ.md) - 常见问题的答案
* [常见问题解决方案](docs/Solutions.md) - 使用/编译 HomeSpan 时一些常见问题的解决方案
* [HomeSpan 参考草图](https://github.com/HomeSpan/HomeSpanReferenceSketches) - 展示一些更复杂的 HomeKit 服务（如恒温器和灌溉系统）的独立参考草图集合

请注意，所有文档都进行版本控制并与每个分支绑定。*master* 分支通常指向最新发布版本。*dev* 分支（如果可用）将包含正在积极开发的代码。

# 外部资源

除了 HomeSpan 资源外，对 HomeKit 编程新手的开发者可能会发现 Apple 的 HomeKit 配件协议规范（非商业版本，Release R2 (HAP-R2)）的第8章和第9章很有用。不幸的是，Apple 不再提供此文档（可能是因为它最后更新于2019年7月，现在有些过时）。但是，您可能可以在网上其他地方找到此文档的副本。请注意，Apple 尚未用任何其他版本替换 HAP-R2 文档供非商业用途，Apple 的开源 [HomeKit ADK](https://github.com/apple/HomeKitADK) 仅反映原始 HAP-R2 规范（而不是 HomeKit 中可用于商业设备的所有最新服务和特征）。

---
### Matter 和 Thread

没有计划使 HomeSpan 与 Matter 兼容，因为 HomeSpan 完全围绕 HAP R2 构建。此外，Apple 和 Espressif 都已发布 Matter SDK 供公众使用，减少了对另一个 Matter SDK 的需求。

通过 Thread 直接将 HomeSpan 连接到 HomeKit 是不计划的（甚至可能不可能）。但是，Thread 可能对设备间通信有用，类似于 HomeSpan 使用 ESP-NOW 来实现远程、电池供电设备的方式。这可能会在未来版本中的某个时候添加。

### 反馈或问题？

请将 HomeSpan 的错误报告或其他问题发布到[问题板](https://github.com/HomeSpan/HomeSpan/issues)。请将关于 HomeSpan 的所有其他问题（使用、功能、规格、示例等）或您可能对新功能有的任何想法或建议，或关于 HomeSpan 或 HomeKit 的任何一般反馈发布到[讨论板](https://github.com/HomeSpan/HomeSpan/discussions)。对于与特定 HomeSpan 问题或功能无关的更一般的问题或反馈，您可以直接发邮件给我：[homespan@icloud.com](mailto:homespan@icloud.com)。

### 关于作者

HomeSpan 由 Gregg Berman 开发并继续维护和支持。它最初是为了解决无法用 Siri 操作 RF 控制厨房抽油烟机这个棘手问题而构思的。我希望您也发现它既有用又有趣。

这是我的第二个大型开源项目——我的第一个是设计一个开源系统，仅使用 Arduino Uno 和 Arduino 电机护盾来生成数字命令和控制 (DCC) 信号来操作模型铁路。虽然我多年来没有参与模型铁路爱好，但展示我原始系统（称为 DCC++）的视频以及它如何工作的详细教程仍然可以在 [DCC++ YouTube 频道](https://www.youtube.com/@dcc2840/videos)上找到。
