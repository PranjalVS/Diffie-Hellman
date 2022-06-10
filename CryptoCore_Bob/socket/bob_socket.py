import subprocess
import socket
import threading
import time
import os
import sys

## Receives public key from Alice
def receive_msg(s):
    received='a'
    while received != '':

        received = s.recv(4096)

        if received == '':
            pass

        elif received.decode() == 'AliceP':  # Receiving public key of Alice and write it to AliceP.txt
            
            file = open("/home/bob/AliceP.txt", "wb")
            RecvData = s.recv(4096)
            file.write(RecvData)
            file.close()
            print("\nPublic key of Alice has been received")

	else:
	    print(received.decode())   
    

## Sends Publice key of Bob to Alice
def sending_msg(s):
    send_msg = u_input.encode('utf-8')
    
    if send_msg == '':
        pass

    elif send_msg.decode() == 'E': ##reading Bob's Public key and send it to Alice

	print("sending BobP")
   	s.send(b'BobP')
        file = open("/home/bob/BobP.txt", "rb")
        SendData = file.read(4096)
        s.send(SendData)
        file.close()

    else:
        s.sendall(send_msg)



if __name__ == '__main__':

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	
    s.bind((socket.gethostname(),1234))

    #starts TCP listener
    s.listen(5)
    print('Bob is listening...')

    #Establish connection with Alice.
    (conn, addr) = s.accept()


    thread1 = threading.Thread(target = receive_msg, args = ([conn])) ##thread for receiving public key
    thread2 = threading.Thread(target = sending_msg, args = ([conn]))##thread for sending public key

    thread1.start()

    u_input= ''

    if u_input != '0':
	print("\nEnter P to generate public key of BobP")
        u_input=input()

        if u_input =='P':
            subprocess.call('/home/root/Diffie_Hellman/CryptoCore_Bob/applications/prime/Public_key', shell=True)
        
        print("\nPublic key of Bob has been generated")

	print("\nEnter E to exchange BobP with Alice")
        u_input=input()

        if u_input == 'E':
	#sending BobP to Alice
            thread2.start()
            thread2.join()


        print("\nEnter S to generate the Commen Secret key")
        u_input=input()
            
        if u_input =='S':
            subprocess.call('/home/root/Diffie_Hellman/CryptoCore_Bob/applications/prime/Bob_CS', shell=True)

        print("\nCommen Secret Key has been created.")
	exit()
        
    
