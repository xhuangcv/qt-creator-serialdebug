## 1. 软件介绍

​	数字舵机调试软件V1.0（简称：SerialDebug1.0）通过串口通信，连接到舵机，给舵机发送指令，执行制定的操作，同时也能够更改舵机的一些参数，如：ID、波特率等。次要功能有发送设置、接受设置、日志记录、日志查看、ASCII表查看和计算器等。
​	本软件结合了多款舵机调试软件的优点，同时独创了一键操作功能，极大的方便了舵机的调试，给用户带来的更好的使用体验。在界面的设计方面，方便和简洁是两大宗旨，在保证用户操作方便的同时，最大话的最求简洁，整个软件给用户一种干净利落的感觉，这是本软件的有一大特色。

## 2. 平台

系统：win10

软件：Qt Creator 5.5.1

 CPU：Intel(R) Core(TM) i5-6200U CPU @2.30GHz（或者更高）

内存：512M或以上内存

## 3. 软件设计

### 3.1.   软件框架结构

该软件的主要是以主界面为核心，发散式的结构，具体结构布局，主界面如下图所示：

<img src=file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image002.jpg style="zoom:70%">

<center>图1 主界面</center>

### 3.2. 功能模块设计及原理

#### 3.2.1. 串口通信

​	此模块由5个QComboBox组件和5个QLabel组件组成。Qt5自带串口通信类QSerialPort，设置好端口名、波特率、数据位、校检位等即可连接好舵机，通过此模块，用户能够通过同不同型号的舵机对参数进行设置。从而实现了对多种型号舵机的调试。在用户点击开始按钮后，QSerialPort类的对象，先回获取QComboBox里的参数值进行初步的设置，然后进行串口连接。

![图片包含 屏幕截图  已生成极高可信度的说明](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image005.png)

<center>图2 串口设置</center>

#### 3.2.2. 单行发送

​	此模块由一个QTextEdit组件和两个QPushButton组件组成。当用户按下发送按钮时触发此按钮的槽函数将QTextEdit中的内容写进QSerialPort实例化的对象，在写入之间还需通过StringtoHex函数将字符串类型的数据转换成十六进制的数据。清空按钮，实现原理简单，只需，执行一下，QTextEdit的clear()函数即可。

![1554950329839](C:\Users\Huangxin\AppData\Roaming\Typora\typora-user-images\1554950329839.png)

<center>图3 单行发送</center>

#### 3.2.3. 一键操作

​	此模块由四个QSpinBox组件、四个QLabel组件和两个QPushButton组件组成。给舵机发送的数据包一般由多个十六进制的数组成，每个数据包都很冗长，为方便操作，独创一键操作功能，用户只需通过对按钮的点击，即可实现舵机转动和修改ID等功能。在软件内部封装了数据包生成函数，如转动功能，当用户点击转动按钮后，首先获取舵机ID、角度和速度三个参数传入数据包生成函数，即可生成相应命令的数据包，最后和发送功能的原理一样，写入数据即可。

<img src="file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image007.jpg" style="zoom:50%">

<center>图4 一键操作</center>

#### 3.2.4. 发送设置

此模块由两个QCheckBox组件、一个QSpinBox组件和一个QLabel组件组成。发送设置能够通过通行协议的不同，自行通过一个QCheckBox选择是否发送新行，也就是在发送的字符串结尾处是否添加“\r\n”，每当用户选着，发送信号，触发槽函数，判断QCheckBox是否被选着，从而执行相应的操作。除此之外，还有一个QCheckBox是用来选着是否周期性自动发送数据包，周期可以在旁边的QSpinBox中设置，原理同上，不同之处在于，自动发送之前，还得读取QSpinBox的周期数值，进行自动发送的周期设置。

 

![图片包含 屏幕截图  已生成极高可信度的说明](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image008.png)

<center>图5 发送设置</center>

#### 3.2.5. 接受设置

​	此模块由四个QCheckBox组件组成。通过if else语句判断QCheckBox是否被选择从而确定是否执行对应的操作。由于舵机发来的数据本身就是十六进制的数据，十六进制显示只需只需将接收到的数据直接写入接收区即可；显示时间的实现，可以通过QDate类获取当前时间，将其与接收到的数据同时写入接收区。黑底白色，及设置接收区的背景颜色和字体颜色，可以通过QTextEdit类的setStyleSheet方法setTextColor方法分别进行设置；自动换行，只需在接收到的数据之后添加“\n”。

![图片包含 屏幕截图  已生成极高可信度的说明](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image009.png)

<center>图6 接收设置</center>

### 1.2.6.    日志功能

日志功能分为两大块，一是记录日志，二是查看日志。记录日志原理，将收到的舵机消息，写入接收区的同时，将其存储在一个txt文件当中。查看日志原理，新建一个对话窗口，将日志文件的内容读取出来，并写入到一个QTextEdit组件当中。

![img](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image010.png)

<center>图7 日志功能</center>

### 1.2.7.    辅助工具

辅助工具有两个，一是ASCII表，二是计算器。ASCII表的实现原理，新建一个对话窗口，再将ASCII表的图片在上面显示出来；计算器的实现，则是直接调用Windows系统自带的计算器。

<img src  = file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image012.jpg style="zoom:70%" >

<center>图8 ASCII表</center>

<img src = file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image014.jpg style="zoom:80%">

<center>图9 系统计算器</center>

## 4. 操作说明

### 4.1. 串口连接

​	首先将舵机通过USB接口连接电脑，在主界面的串口设置中设置好参数，最后在工具栏中找到开始图标，点击即可。除此之外，在编辑菜单栏中也可找到开始按钮。

![img](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image016.jpg)

<center>图11 串口连接</center>

### 4.2. 指令发送

​	首先点击单条发送，在输入区输入指令，然后点击发送按钮即可。

![img](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image018.jpg)

<center>图12 指令发送</center>

### 4.3. 一键功能

​	首先点击一键操作，输入角度和速度参数，点击转动，舵机即可开始转动；输入目标ID点击修改，则可以修改舵机的ID。

![img](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image020.jpg)

<center>图13 一键功能</center>

### 4.4. 日志功能

​	在工具栏中点击日志图标即可，开始记录日志。在文件菜单栏中也可记录日志和查看日志。

![img](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image022.jpg)

<center>图14 日志功能</center>

### 4.5. 辅助工具的使用

​	在工具菜单栏中可以打开ASCII码辅助工具，同时也可打开计算器功能。

![img](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image024.jpg)

<center>图15 辅助工具</center>

 	同时软件设有拖动功能，只需要将需要发送的文档，拖到主窗口界面，即可将文件在发送区打开。

![img](file:///C:/Users/Huangxin/AppData/Local/Temp/msohtmlclip1/01/clip_image026.jpg)

<center>图16 拖动功能</center>
