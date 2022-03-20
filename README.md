#  esp8266_servo_linght  
这是一个通过乐鑫Esp8266结合Serco实现物理控制开关等的示例程序。

作品的原理很简单，通过Esp8266连接Wifi，并创建网络服务器，生成可访问的网页，其他设备通过局域网连接访问，通过发送和处理网络请求控制Servo角度直接控制灯的开关。

## 1 硬件准备工作

- 乐鑫Esp8266 NodeMCU一个
![](https://www.nodemcu.com/images/thumbnail/zms.jpg_1200x1200.jpg)
- Servo舵机一个
![](https://www.robotshop.com/media/catalog/product/cache/image/1350x/9df78eab33525d08d6e5fb8d27136e95/g/e/geekservo-9g-270-servo-compatible-with-lego.jpg)
- 3D打印机一个  （配合Cura软件使用）
不是必须，代码中会附加模型文件，可联系网上店铺打印。（精度要求并不高）
## 2 软件准备工作  

- Arduino IDE  PC端（最优解）或者是APP端  
![](https://www.arduino.cc/wiki/370832ed4114dd35d498f2f449b4781e/arduino.svg)
 [Arduino IDE](https://www.arduino.cc/en/software)
- freeCAD  (修改打印尺寸并导出打印模型STL格式)  
![](https://www.freecadweb.org/svg/logo-freecad.svg)  
 [FreeCAD](https://www.freecadweb.org/downloads.php)
- Cura （3D输出打印，我使用的时gcode格式。网店打印可不用安装，配合3D打印机使用）  
 [Cura DGO下载](https://cura-dgo-14-07-01.updatestar.com/en)
## 3 ESP8266开发板的导入  
### 一、添加 ESP8266 支持
- Start Arduino and open Preferences window.
- 启动Arduino,打开`文件`进入`首选项`  
- Enter `https://arduino.esp8266.com/stable/package_esp8266com_index.json` into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
- 在`附加开发板管理器网址`中输入`https://arduino.esp8266.com/stable/package_esp8266com_index.json`，可换行添加，`好的`确认。
- Open Boards Manager from Tools > Board menu and find esp8266 platform.
- 打开`工具`找到`开发板`选择`开发板管理器`搜索`esp8266`选择最新版本安装。
- Select the version you need from a drop-down box.
- Click install button.
- 选择版本，点击安装。
- Don’t forget to select your ESP8266 board from Tools > Board menu after installation.
- 在`工具`内的`开发板选择`界面选择自己的开发板。
- 本示例程序以NodeMCU 0.9/1.0作为示例。
### 二、引脚连接
![](http://www.taichi-maker.com/wp-content/uploads/2016/12/esp8266_devkit_horizontal-01.png)
采用 `GND` `3V3` `D4`(GPIO2)三个引脚连接舵机
#### 红色连接正极、棕色连接负极、黄色连接D4
## 4程序修改项
### 1. 修改添加自己的Wifi信息

```
wifiMulti.addAP("2.4GWifi名称", "密码");  
//通过addAp函数存储  WLAN网络信息，格式为 wiFiMulti.addAP(ssid, password)
```  
### 2. 修改网络服务器的内容  
所有中文提示都可随意修改

     "<form action=\"/OPEN\" method=\"POST\"><input type=\"submit\" value=\"开灯\"style=\"width:1000px;height:150px;font-size:100px \"></form>" 

     "<form action=\"/CLOSE\" method=\"POST\"><input type=\"submit\" value=\"关灯\"style=\"width:1000px;height:150px;font-size:100px\"></form>"

     "<form action=\"/NORMOL\" method=\"POST\"><input type=\"submit\" value=\"释放状态\"style=\"width:1000px;height:150px;font-size:100px\"></form>"

     "<font size=300>欢迎来到Wifi控制界面! </font>"
### 3. 修改开关灯时的舵机角度


	servo.write(角度);//根据开灯状态、关灯状态自行调整


	void normal() {

 		servo.write(85);//中置状态，避免长时间挤压开关

  		esp8266_server.sendHeader("Location", "/");

  		esp8266_server.send(303);
	}

## 5 访问Esp8266的局域网IP地址，即网络服务器地址。
可访问路由去或串口监视器查看。建议在路由器中加入IP地址绑定。
