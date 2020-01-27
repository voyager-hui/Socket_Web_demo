# 名称：客户端代码
# 时间：20200127
# 作者：voyager
# 功能：读取输入并通过TCP发给服务器
# 注意：

import socket

def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    host = '49.232.163.31'
    port = 3333
    client.connect((host,port))

    start_msg = client.recv(1024)
    print(start_msg.decode('utf-8')) # receive inital messege

    while True:
        sendmsg = input()
        client.send(sendmsg.encode('utf-8'))
        if sendmsg == 'quit':
            break
    client.close()


if __name__ == '__main__':
    main()
