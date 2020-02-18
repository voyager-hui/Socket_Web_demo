# 名称：客户端代码
# 时间：20200218
# 作者：voyager
# 功能：发送数据到服务器
# 注意：

import socket
import random
import numpy as np


def socket_comm(client):
    for _ in range(10):
        name = str(random.sample(['pi','pc','pad','phone'], 1)[0])
        lon = random.random()*100
        lat = random.random()*100
        print("name",name)
        print("lon",lon)
        print("lat",lat)
        msg = name+','+str(lon)+','+str(lat)
        client.send(msg.encode('utf-8'))
        print('Client send:', msg)
        server_msg = client.recv(1024)
        print('Client receive:', server_msg)
        if server_msg == 'complete':
            break
        elif server_msg == 'shutdown':
            pass
        elif server_msg == 'ledon':
            pass        
        elif server_msg == 'ledoff':
            pass
        else:
            pass
    client.send('complete'.encode('utf-8'))
    print('Client send:', 'complete')


if __name__ == '__main__':
    host = '49.232.163.31'
    port = 3333
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((host,port))
    start_msg = client.recv(1024)
    print(start_msg.decode('utf-8'))

    socket_comm(client)

    client.close()
    print("Client socket closed.")
