# 名称：客户端代码
# 时间：20200127-20200128
# 作者：voyager
# 功能：将原始数据发送给服务器
#       接收服务器处理结果
# 注意：

import socket
import numpy as np


def send(client):
    data = np.load("client_data.npy")
    for i in range(data.shape[0]):
        msg = str(data[i, 0]) + ',' + str(data[i, 1])
        client.send(msg.encode('utf-8'))
        print('Client send:', msg)
        server_msg = client.recv(1024)
        print('Client receive:', server_msg)
    client.send('complete'.encode('utf-8'))
    print('Client send:', 'complete')

def receive(client):
    while True:
        msg = client.recv(1024).decode('utf-8')
        print('Client receive:', msg)

        if msg == 'complete':
            break
        else:
            with open('client_result.csv', 'a') as file_txt:
                file_txt.write(msg)
                file_txt.write('\n')

        client.send('Received'.encode("utf-8"))
        print('Client send:', 'Received')


if __name__ == '__main__':
    host = '49.232.163.31'
    port = 3333
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((host,port))

    start_msg = client.recv(1024)
    print(start_msg.decode('utf-8'))

    # send message
    send(client)
    # receive message
    receive(client)

    client.close()
    print("Client socket closed.")
