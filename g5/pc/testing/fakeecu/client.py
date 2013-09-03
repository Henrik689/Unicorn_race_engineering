import socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(("192.168.1.1", 31337))
client_socket.send("Hest er ude!!")
print "hest"
data=client_socket.recv(512)
print data
client_socket.close()
            
