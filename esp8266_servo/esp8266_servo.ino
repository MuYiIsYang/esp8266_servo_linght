#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库，负责储存多个Wifi信息
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
#include <Servo.h>
Servo servo;
ESP8266WiFiMulti wifiMulti;     // 实例化ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

void setup(void) {
  Serial.begin(9600);
  servo.attach(2);
  servo.write(80);
  delay(1000);

  wifiMulti.addAP("MERCURY_A6EE", "13666381301");
  //通过addAp函数存储  WLAN网络信息，格式为 wiFiMulti.addAP(ssid, password)

  Serial.println("正在连接Wifi,请稍等：");
  while (wifiMulti.run() != WL_CONNECTED) {
    /*此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
      环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
      将会连接信号最强的那一个WiFi信号。一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。
      这也是此处while循环判断是否跳出循环的条件。
    */
    delay(500);
    Serial.print(". ");
  }

  // WiFi连接成功后将通过串口监视器输出连接成功信息
  Serial.println(' ');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP

  esp8266_server.begin();                              // 服务器启动
  esp8266_server.on("/", HTTP_GET, homepage);          // 设置服务器根目录即'/'
  esp8266_server.on("/OPEN", HTTP_POST, openlight);    // 收到OPEN请求，调用openlight子函数
  esp8266_server.on("/CLOSE", HTTP_POST, closelight);  // 收到CLOSE求，调用closelight子函数
  esp8266_server.on("/NORMOL", HTTP_POST, normal);     // 收到NORMAL请求，调用normal子函数
  esp8266_server.onNotFound(handleNotFound);           // 设置处理404情况的函数'handleNotFound'

  Serial.println("HTTP esp8266_server started");       //  告知用户ESP8266网络服务功能已经启动
}

void loop(void) {
  esp8266_server.handleClient();   //监听客户请求并处理
}

/*设置服务器根目录即'/'的函数'homepage'
  该函数的作用是每当有客户端访问NodeMCU服务器根目录时，
  NodeMCU都会向访问设备发送 HTTP 状态 200 (Ok) 这是send函数的第一个参数。
  同时NodeMCU还会向浏览器发送HTML代码，以下示例中send函数中第三个参数，
  也就是双引号中的内容就是NodeMCU发送的HTML代码。该代码可在网页中产生LED控制按钮。
  当用户按下按钮时，浏览器将会向NodeMCU的/LED页面发送HTTP请求，请求方式为POST。
  NodeMCU接收到此请求后将会执行handleServo函数内容*/
void homepage() {
  esp8266_server.send(200, "text/html; charset=utf-8" , //中文编码
     "<form action=\"/OPEN\" method=\"POST\"><input type=\"submit\" value=\"开灯\"style=\"width:1000px;height:150px;font-size:100px \"></form>"
     "<form action=\"/CLOSE\" method=\"POST\"><input type=\"submit\" value=\"关灯\"style=\"width:1000px;height:150px;font-size:100px\"></form>"
     "<form action=\"/NORMOL\" method=\"POST\"><input type=\"submit\" value=\"释放状态\"style=\"width:1000px;height:150px;font-size:100px\"></form>"
     "<font size=300>欢迎来到Wifi控制界面! </font>");


}

void openlight() {
  servo.write(110);
  delay(1000);
  servo.write(85);
  esp8266_server.sendHeader("Location", "/");           //回到homepage
  esp8266_server.send(303);
}
void closelight() {
  servo.write(60);
  delay(1000);
  servo.write(85);
  esp8266_server.sendHeader("Location", "/");
  esp8266_server.send(303);
}
void normal() {
  servo.write(85);
  esp8266_server.sendHeader("Location", "/");
  esp8266_server.send(303);
}

void handleNotFound() {
  esp8266_server.send(404, "text/plain", "404: Not found");
}
