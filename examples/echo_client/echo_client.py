import sys
import socket
import time
import datetime

server_addr = ("localhost", 6666)
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(server_addr)
num = 0
send_data = "test echo message"
while True:
    client_socket.send(send_data)
    data = client_socket.recv(1024)
    if num % 10000 == 0:
        print("recieved data %d" % num)
        print(str(datetime.datetime.now()))
    #time.sleep(1)
    time.sleep(1/1000000.0)
    num = num + 1;
