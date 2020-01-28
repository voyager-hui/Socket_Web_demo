# 名称：客户端代码
# 时间：20200127
# 作者：voyager
# 功能：读取输入并通过TCP发给服务器
# 注意：

import socket
import numpy as np

def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    host = '49.232.163.31'
    host = '192.168.1.107'
    port = 3333
    client.connect((host,port))

    start_msg = client.recv(1024)
    print(start_msg.decode('utf-8')) # receive inital messege

    # send msg
    place_loc = np.loadtxt('c_data.txt')
    print("待发送的信息：type(place_loc)", place_loc, '\n', type(place_loc))
    for i in range(25):
        msg = place_loc[i, 0] + ',' + place_loc[i, 0]
        client.send(msg.encode('utf-8'))
     
    client.send('python process.py\n'.encode('utf-8'))
    client.send('quit'.encode('utf-8'))

    reply = input("Please input:")
    client.send(reply.encode('utf-8'))
    client.close()


if __name__ == '__main__':
    main()
