# socket通信实现    
完成这么一个需求：  
客户机采集数据并存储在data.csv文件中，限于客户机处理能力不够，
将data.csv中的内容按行发送到服务器上，服务器接收并存储原始数据，
再调用数据处理程序处理，处理结果存储下来，服务器再将结果按行发送给客户机，
客户机接收结果并存储为result.csv。
本实现中data.csv中数据为25个地点的经度纬度数据。  
服务器中数据处理程序为计算25个地点两两之间的距离。  
result.csv中为结果。
## Python实现  
服务器和客户端代码为Python程序。
运行步骤：  
  1. 将server.py和server_process.py放在服务器上，将client.py和data.csv放在客户机上。  
  2. 服务器上运行命令python server.py  
  3. 客户机上运行命令python client.py  
*reference*:  
https://blog.csdn.net/qq_41427568/article/details/89049194  
## C实现  
服务器和客户端代码为C程序，服务器的处理程序为Python程序（可以把处理程序也换成C程序，
但感觉C程序来处理太麻烦了，文件读写和数据类型等等，就借用了上面的Python处理程序）。  
服务器与客户机均为Linux系统。Windows下运行不了，缺失相关的头文件。  

运行步骤：  
  1. 将server.c和server_process.py放在服务器上，将client.c和data.csv放在客户机上。  
  2. 服务器上运行命令gcc server.c -o server和./server  
  3. 客户机上运行命令gcc client.c -o server和./client  
运行结果：  
![运行结果1](./assets/c_1.png)  
![运行结果2](./assets/c_2.png)  
*reference*:  
https://www.binarytides.com/socket-programming-c-linux-tutorial/  
