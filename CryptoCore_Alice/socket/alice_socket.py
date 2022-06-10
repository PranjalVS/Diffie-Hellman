import subprocess
import socket
import threading
import sys
import os
import time

## Receives public key from Bob
def receive_msg(s):

    received='a'
    while received != '':
	
        received = s.recv(4096)
	if received == '':
            pass
            

        elif received.decode() == 'BobP':   # Receiving public key of Bob and write it to BobP.txt
           
            file = open("/home/alice/BobP.txt", "wb")
            RecvData = s.recv(4096)
            file.write(RecvData)
            file.close()
            print("\nPublic key of Bob has been received")

	else:
            print(received.decode())   
        

## Sends Publice key of Alice to Bob
def sending_msg(s):

    send_msg = u_input.encode('utf-8')

    if send_msg == '':
        pass

    elif send_msg.decode() == 'E': ##reading Alice's Public key and send it to Bob

	print("sending AliceP")
        s.send(b'AliceP')
        file = open("/home/alice/AliceP.txt", "rb")
        SendData = file.read(4096)
        s.send(SendData)
        file.close()

    else:
        s.sendall(send_msg)



if __name__ == '__main__':

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    s.connect((socket.gethostname(),1234))
    
    thread1 = threading.Thread(target = receive_msg, args = ([s])) ##thread for receiving public key
    thread2 = threading.Thread(target = sending_msg, args = ([s])) ##thread for sending public key

    thread1.start()
    
    u_input= ''

    if u_input != '0':
     
        print("\nEnter P to generate public key of Alice")
        u_input=input()

        if u_input =='P':
            ##Generating Alice's public key(AliceP) by running the compiled c program ( Public_key )
            subprocess.call('/home/root/Diffie_Hellman/CryptoCore_Alice/applications/prime/Public_key', shell=True)
        
        print("\nPublic key of Alice has been generated")

    	print("\nEnter E to exchange AliceP with Bob")
        u_input=input()

        if u_input =='E':
	    print("Exchange")
            #sending AliceP to Bob
            thread2.start()
            thread2.join()


        print("\nEnter S to generate the Commen Secret key")
        u_input=input()
        
        if u_input =='S':
            subprocess.call('/home/root/Diffie_Hellman/CryptoCore_Alice/applications/prime/Alice_CS', shell=True)

        print("\nCommen Secret Key has been created.")

      
