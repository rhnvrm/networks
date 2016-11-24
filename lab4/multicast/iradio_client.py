# !/usr/bin/env python

from binarysocket import mysocket
import socket
import struct

import subprocess
import time
import threading
import os

MCAST_GRP = '224.1.1.1' # multicast group to subscribe to
MCAST_PORT = 5007 		# data port of multicast stream 
buf = 2048

def iradio():
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	
	# use MCAST_GRP instead of '' to listen only
	# to MCAST_GRP, not all groups on MCAST_PORT
	s.bind(('', MCAST_PORT)) 
	
	# some multicast options
	mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
	s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)


	f = open('client.ts','wb', buf) 
	# start reciving data 
	data, addr = s.recvfrom(buf)

	# spawn a thread to start playing the stream at suitable time
	# streaming_thread = threading.Thread(target=stream())	
	# streaming_thread.start()


	# piping data recived directly to ffplay stdin
	commands = ['ffplay', '-']
	player_process = subprocess.Popen(commands, stdin=subprocess.PIPE)
	try: 		
		
		while(data):	
			player_process.stdin.write(data)

			# f.write(data)
			# force write data to disk everytime there is a write
			# f.flush()
			# os.fsync(f.fileno())

			s.settimeout(5)
			data, addr = s.recvfrom(buf)

	except socket.timeout:
		f.close()
		s.close()
		print "Song completely buffered"

	# replace with stdin return codes to terminate
	time.sleep(50)

	
	

def stream():
	# import os 
	# cons = 2.5*1024*1024
	# size = os.path.getsize('client.ts')
	# while size < cons:
	# 	print "trying"
	# 	time.sleep(2)

	print "new thread spawned"
	print "sleeping.. for 5 sec"
	time.sleep(5)
	print "trying to play song"
	subprocess.call(['ffplay', 'client.ts'])

def main():
	iradio()

if __name__ == '__main__':
 	main() 