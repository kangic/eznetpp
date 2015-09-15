import sys
import socket
import time
import datetime

client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
num = 0
send_data = "echotest"

print("%d" % int(sys.argv[2]))
while True:
    client_socket.sendto(send_data, (sys.argv[1], int(sys.argv[2])))
    data = client_socket.recvfrom(1024)
    print(data)
    if num % 10000 == 0:
        print("recieved data %d" % num)
        print(str(datetime.datetime.now()))
    #time.sleep(1)
    time.sleep(1/1000000.0)
    num = num + 1;
