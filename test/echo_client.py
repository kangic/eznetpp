import sys
import socket
import time

server_addr = ("localhost", 6666)
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(server_addr)
while True:
    send_data = raw_input()
    client_socket.send(send_data)
    data = client_socket.recv(1024)
    print("recieved data = %s" % data)
    time.sleep(1)
