# 一、客户机与服务器进行socket通信    
完成Socket通信的demo。  
客户机将data.csv中的内容按行发送到服务器上，服务器接收并存储原始数据。
服务器再调用数据处理程序处理，得到处理结果。
服务器再将结果按行发送给客户机，客户机接收结果并存储为result.csv。  
为让程序实用，其中data.csv中数据为25个地点的经度纬度数据。result.csv中为地点之间的两两距离。  
服务器中数据处理程序为计算25个地点两两之间的距离的程序。  

## 1、Python实现  
服务器和客户端代码为Python程序。  

运行步骤：  
  1. 将server.py和server_process.py放在服务器上，将client.py和client_data.npy放在客户机上。  
  2. 服务器上运行命令python server.py  
  3. 客户机上运行命令python client.py  

*reference*:  
https://blog.csdn.net/qq_41427568/article/details/89049194  
## 2、C实现  
服务器和客户端代码为C程序。  
服务器与客户机均为Linux系统。
Windows下缺失诸如sys/socket.h、arpa/inet.h、unistd.h头文件。  

运行步骤：  
  1. 将server.c和server_process.c放在服务器上，将client.c和data.csv放在客户机上。  
  2. 服务器上运行命令gcc server_process.c -o server_process、gcc server.c -o server和./server  
  3. 客户机上运行命令gcc client.c -o server和./client  

运行结果：  
![socket1](./assets/c_1.png)  
![socket2](./assets/c_2.png)  

*reference*:  
https://www.binarytides.com/socket-programming-c-linux-tutorial/  

# 二、Web网页实时显示数据  
Web网页实时显示服务器后台发送来的数据。  
demo主要分为两部分：1、服务器程序不断生成新数据，并推送新数据到data.txt文件。
2、网页通过JS定时访问data.txt文件获取新数据，显示在网页上。  
注意：本demo的更新数据方法在本地运行会因安全策略而不能数据更新。在云服务器上运行不会。  

## 1、服务器后台程序推送新数据  
由refresh.c实现功能：refresh.c先生成新数据，然后拼成shell命令，最后执行shell命令将新数据写入到data.txt中。  
shell命令为  
```bash  
echo "new_data" > /var/www/html/sensor/data.txt
```    
注意：
	1. 文件/var/www/html/sensor/data.txt需要完全的权限，不然无法访问。  
	2. 因为需要用字符串来拼成shell命令，所以读得的字符串需要先去掉换行符
	(linux换行符为\n， windows换行符为\r\n，Mac换行符为\r)。 
## 2、网页定时刷新获取新数据  
由sensor_data.html中的JS完成刷新功能：设置setInterval函数使得客户端每间隔5秒就读取一下data.txt。  
如果当前页面中的数据与data.txt文件中的数据相同，则跳过，否则更新当前页面。  

运行步骤：  
  1. 将sensor_data.html和style.css放在服务器html文件夹内，使网页可以访问。  
  2. 将refresh.c和data.csv放在服务器的某个文件夹内。  
  3. 服务器上运行命令gcc refresh.c -o refresh和sudo ./refresh  
  4. 访问sensor_data.html所在的网页，观察效果  

运行结果：  
![web1](./assets/web.png)  

# 三、Socket和Web网页demo  
综合上面两个demo。  
客户机采集数据，通过Socket传递给服务器程序。  
服务器程序将采集到的数据推送到data.txt。
浏览器通过javascript/ajax读取data.txt的数据，并展示到Web界面中。  
浏览器通过javascript/ajax获取用户的指令，将指令数据传给服务器php脚本，由php脚本将指令数据写入到command.txt中。  
服务器程序再读取command.txt，获取到用户通过Web网页发出的指令。  
服务器程序最后将用户指令通过Socket传递给客户机。  
![flow](./assets/web_socket_flow.png)  

运行步骤：  
  1. 将sensor_data.html、style.css和command.php放在服务器html文件夹内，  
     并在此建立777权限的./sensor/data.txt和./sensor/command.txt文件。  
  2. 将server.c放在服务器上，将client.c和data.csv放在客户机上。  
  3. 服务器上运行命令gcc server.c -o server和./server  
  4. 客户机上运行命令gcc client.c -o server和./client  
  5. 访问sensor_data.html所在的网页，观察效果  

运行结果：  
(pi黑框为树莓派客户端client.c，JupyterLab黑框为服务器端server.c)  
![program](./assets/web_socket_demo.png)  
![browser](./assets/web_socket_demo_browser.png)  

**改进**
1. 将data.txt和command.txt换为Mysql数据库。这样操作更安全更优雅。
Python已经可以与数据库进行连接和操作，但C与PHP还不行，环境配置过于麻烦。 
2. 加入多线程，使得服务器可以同时支持多个客户机。

