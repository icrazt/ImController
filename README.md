# ImController
> 开发中 可用时间未知

基于 ImGUI 的 UI 控制面板，主要用于嵌入式设备的调试与监控。使用串行通信的方式（串口 或 wifi+socket）, 使用 TinyFrame 作为上层协议。

使用 websocket 时可由 PC 端直接建立 websocket 服务器，用于接收下位机的数据。或是直接在两者间建立 socket 连接，传送数据。

## 基于

[Dear ImGui]() - *Docking branch*

Implot :[epezent/implot: Advanced 2D Plotting for Dear ImGui](https://github.com/epezent/implot)

TinyFrame:[MightyPork/TinyFrame: A simple library for building and parsing data frames for serial interfaces (like UART / RS232)](https://github.com/MightyPork/TinyFrame)

CSerial: [CSerial - A C++ Class for Serial Communications](https://www.codeguru.com/cpp/i-n/network/serialcommunications/article.php/c2503/CSerial--A-C-Class-for-Serial-Communications.htm)

[zaphoyd/websocketpp: C++ websocket client/server library](https://github.com/zaphoyd/websocketpp/)

[Asio C++ Library](http://think-async.com/Asio/index.html) 
