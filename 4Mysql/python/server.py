# 名称：服务器端代码
# 时间：20200218
# 作者：voyager
# 功能：服务器接受客户机数据并存到Mysql数据库
# 注意：

import os
import socket
import numpy as np
import pymysql

# connect the DB
db = pymysql.connect("localhost","root","1234","place" )
cursor = db.cursor()

def socket_comm(clientsocket):
    while True:
        msg = clientsocket.recv(1024).decode('utf-8')
        print('Server receive:', msg)

        if msg == 'complete':
            break
        else:
            insert_data(msg)

        clientsocket.send('Received'.encode("utf-8"))
        print('Server send:', 'Received')


def insert_data(msg):
    sql = "INSERT INTO data(name,lon,lat)VALUES('"+msg.split(",")[0]+"', "+str(msg.split(",")[1])+","+str(msg.split(",")[2])+");"
    print("sql:", sql)
    try:
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
        print("ERROR TO INSERT")


def print_db():
    sql = "SELECT * FROM data"
    try:
        cursor.execute(sql)
        results = cursor.fetchall()
        for row in results:
            name = row[1]
            lon = row[2]
            lat = row[3]
            print ("name=%s, lon=%d, lat=%d" %(name, lon, lat))
    except:
        print ("Error: unable to fetch data")


if __name__ == '__main__':
    print("通信前，数据库里面的数据：")
    print_db()  # before insert

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
    socket_comm(clientsocket)  # socket communication
    print("通信后，数据库里面的数据：")
    print_db()  # after insert
    db.close()
    clientsocket.close()
    print("Server socket closed.")
