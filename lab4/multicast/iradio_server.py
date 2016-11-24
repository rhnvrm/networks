# !/usr/bin/env python

import socket
import time

MCAST_GRP = '224.1.1.1'
MCAST_PORT = 5007

buf = 1024

def iradio():
	# create socket 
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	
	# some multicast options
	s.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
	
	""" Sample way of sending data to multicast group
	s.sendto("robot", (MCAST_GRP, MCAST_PORT))
	"""

	# sending one song at a time for now 
	f = open('output_mp2.ts', 'rb')

	data = f.read(buf)
	while data:
		if s.sendto(data, (MCAST_GRP, MCAST_PORT)):
			print "sending"
			data = f.read(buf)
			time.sleep(0.005) # 5 milli

	s.close()
	f.close()
	print "Send Successful!"


def main():
	iradio()

if __name__ == '__main__':
	main() 