# 名称：服务器端代码
# 时间：20200127-20200128
# 作者：voyager
# 功能：服务器接受客户机数据
#       调起另一个服务器程序进行处理
#       将处理结果返回给客户机
# 注意：

import os
import socket
import numpy as np


def receive(clientsocket):
    while True:
        msg = clientsocket.recv(1024).decode('utf-8')
        print('Server receive:', msg)

        if msg == 'complete':
            break
        else:
            with open('server_data.csv', 'a') as file_txt:
                file_txt.write(msg)
                file_txt.write('\n')

        clientsocket.send('Received'.encode("utf-8"))
        print('Server send:', 'Received')

def send(clientsocket):
    result = np.load("server_result.npy")
    for i in range(result.shape[0]):
        msg = str(result[i, :])  # match the formate
        clientsocket.send(msg.encode('utf-8'))
        print('Server send:', msg)
        client_msg = clientsocket.recv(1024)
        print('Server receive:', client_msg)
    clientsocket.send('complete'.encode("utf-8"))
    print('Server send:', 'complete')


if __name__ == '__main__':
    host = ''
    port = 3333
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((host,port))
    server.listen()
    
    clientsocket, addr = server.accept()
    print("Client addr info: ", addr)
    clientsocket.send('Connected Successfully'.encode('utf-8'))
    print('Server send:', 'Connected Successfully')

    # receive message
    receive(clientsocket)
    # process
    os.system("python server_process.py")
    # send message
    send(clientsocket)

    clientsocket.close()
    print("Server socket closed.")
