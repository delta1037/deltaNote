# deltaNote

## 1.Linux server

进入server文件夹根目录，执行./deltaNote

### 1.1note

- 通过`netstat -anp | grep 8888`查看运行状态，8888是默认端口
- 可以编辑deltaNote，自定义ip（一般来讲，本地IP配置为127.0.0.1；服务器IP配置为0.0.0.0）和端口

## 2.Linux client

### 2.1 安装

默认安装位置为`/usr/local/deltaNote`

### 2.2 设置开机启动

打开startup Application，添加软件位置`/usr/local/deltaNote/deltaNote`

## 3.Window client

双击exe文件进行安装(不支持装在C盘)