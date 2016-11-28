# !/usr/bin/env python

import socket
import struct

import subprocess
import time
import threading
import os

# MCAST_GRP = '224.1.1.1' # multicast group to subscribe to
MCAST_GRP = '230.192.3.255'
MCAST_PORT = 5432 		# data port of multicast stream 
buf = 2048

def iradio(port):
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	
	# use MCAST_GRP instead of '' to listen only
	# to MCAST_GRP, not all groups on MCAST_PORT

	MCAST_LOCAL_PORT = port
	s.bind(('', MCAST_LOCAL_PORT)) 
	
	# some multicast options
	mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
	s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

	size = 10*1024*1024
	s.setsockopt(socket.SOL_SOCKET,socket.SO_RCVBUF, size)
		
	commands = ['ffplay', '-']
	player_process = subprocess.Popen(commands, stdin=subprocess.PIPE)

	while True:
		try:
			data, addr = s.recvfrom(buf)
			player_process.stdin.write(data)
			s.settimeout(5)		
		
		except socket.timeout:
			print ("connection dropped")
			# no need to close socket
			# s.close()


	# f = open('client.ts','wb', buf) 
	# # start reciving data 
	# data, addr = s.recvfrom(buf)

	# # spawn a thread to start playing the stream at suitable time
	# # streaming_thread = threading.Thread(target=stream())	
	# # streaming_thread.start()


	# # piping data recived directly to ffplay stdin
	# commands = ['ffplay', '-']
	# # pipe sub process output to null
	# FNULL = open(os.devnull, 'w')
	# player_process = subprocess.Popen(commands, stdin=subprocess.PIPE, stdout=subprocess.PIPE) #stdout=FNULL)
	# try: 		
		# s.settimeout(5)
	# 	while(data):	
	# 		player_process.stdin.write(data)

	# 		# f.write(data)
	# 		# force write data to disk everytime there is a write
	# 		# f.flush()
	# 		# os.fsync(f.fileno())


	# 		data, addr = s.recvfrom(buf)

	# except socket.timeout:
	# 	f.close()
	# 	s.close()
	# 	print "Song completely buffered"

	# while True:
	# # replace with stdin return codes to terminate
	# 	try:
 #  			proc = subprocess.check_output(commands, stderr=subprocess.STDOUT)
 #  			# do something with output
 #  			print proc
	# 	except subprocess.CalledProcessError:
 #  			print 'There was an error - command exited with non-zero code'
 #  			break

 #  	print 'exiting program'

 # 	player_process.wait()
	
	

# def stream():
# 	# import os 
# 	# cons = 2.5*1024*1024
# 	# size = os.path.getsize('client.ts')
# 	# while size < cons:
# 	# 	print "trying"
# 	# 	time.sleep(2)

# 	print ("new thread spawned")
# 	print ("sleeping.. for 5 sec")
# 	time.sleep(5)
# 	print ("trying to play song")
# 	subprocess.call(['ffplay', 'client.ts'])

def main():
	iradio()

if __name__ == '__main__':
 	main() 