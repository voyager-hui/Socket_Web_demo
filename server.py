# 名称：服务器端代码
# 时间：20200127
# 作者：voyager
# 功能：接受客户数据并存储
# 注意：

import socket

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    host = '0.0.0.0'
    port = 3333
    server.bind((host,port))
    
    clientsocket, addr = server.accept()
    # print("addr: ", addr)
    clientsocket.send('Successfully, continue:'.encode('utf-8'))

    while True:
        recv_msg = clientsocket.recv(1024)
        msg = recv_msg.decode('utf-8')
        if msg == 'quit':
            exit(0)
        print('Received: ' + msg)

    clientsocket.close()


if __name__ == '__main__':
    main()
