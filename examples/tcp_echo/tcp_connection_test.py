import sys
import socket
import time
import datetime

num = 1
server_addr = (sys.argv[1], int(sys.argv[2]))
#for i in range(int(sys.argv[1])) :
while True:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_addr)
    if num % 100 == 0:
        print("connection test #%d" % num)
    client_socket.close()
    num = num + 1
