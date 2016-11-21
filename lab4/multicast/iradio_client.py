# !/usr/bin/env python

from binarysocket import mysocket
import socket
import struct

MCAST_GRP = '224.1.1.1' # multicast group to subscribe to
MCAST_PORT = 5007 		# data port of multicast stream 
buf = 1024

def iradio():
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	# use MCAST_GRP instead of '' to listen only
	# to MCAST_GRP, not all groups on MCAST_PORT
	s.bind(('', MCAST_PORT)) 
	
	mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
	s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)


	f = open('client.mp3','wb')
	# start reciving data 
	data, addr = s.recvfrom(buf)
	try: 
		while(data):
			f.write(data)
			s.settimeout(5)
			data, addr = s.recvfrom(buf)

	except socket.timeout:
		f.close()
		s.close()
		print "Song completely buffered"

def main():
	iradio()

if __name__ == '__main__':
 	main() 