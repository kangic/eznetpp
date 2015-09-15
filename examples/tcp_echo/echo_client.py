import sys
import socket
import time
import datetime
import thread

server_addr = (sys.argv[1], 6666)
num_of_threads = int(sys.argv[3])

def echotest_func(idx):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(server_addr)
    num = 0
    send_data = "echotest"
    while True:
        client_socket.send(send_data)
        data = client_socket.recv(1024)
        #print(data)
        if num % 10000 == 0:
            print("[%d] recieved data %d" % (idx, num))
            print(str(datetime.datetime.now()))
        #time.sleep(1)
        time.sleep(1/1000000.0)
        num = num + 1;

for i in range(0, num_of_threads):
    thread.start_new_thread(echotest_func, (i, ))

while True:
    time.sleep(1)
