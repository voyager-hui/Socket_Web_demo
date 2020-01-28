# 名称：服务器端代码
# 时间：20200127
# 作者：voyager
# 功能：接受客户数据并存储，并能调起另一个py程序
# 注意：

import socket
import os

def receive(clientsocket):
    while True:
        recv_msg = clientsocket.recv(1024)
        msg = recv_msg.decode('utf-8')
        # quit
        if msg == 'quit':
            break
        # call another .py
        if msg == 'python process.py':
            os.system("python process.py")
            continue
        with open('data.txt', 'w') as file_txt:
            file_txt.write(msg)
            file_txt.write('\n')
            
def send(clientsocket):
    with open('result.txt', 'r') as file_txt:
        msg = file_txt.write(msg)
    print(msg)
    # clientsocket.send(msg.encode('utf-8'))

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    host = '0.0.0.0'
    port = 3333
    server.bind((host,port))
    server.listen()
    clientsocket, addr = server.accept()
    print("addr: ", addr)
    clientsocket.send('Successfully, continue:'.encode('utf-8'))
    
    # receive message
    receive(clientsocket)
    # send message
    send(clientsocket)

    clientsocket.close()


if __name__ == '__main__':
    main()
