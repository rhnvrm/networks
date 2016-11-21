# !/usr/bin/env python

from binarysocket import mysocket
import socket
import time

MCAST_GRP = '224.1.1.1'
MCAST_PORT = 5007

buf = 1024

def iradio():
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	s.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
	s.sendto("robot", (MCAST_GRP, MCAST_PORT))

	f = open('test.mp3', 'rb')

	data = f.read(buf)
	while data:
		if s.sendto(data, (MCAST_GRP, MCAST_PORT)):
			print "sending"
			data = f.read(buf)
			time.sleep(0.0005)

	s.close()
	f.close()
	print "send successful!"


def main():
	iradio()

if __name__ == '__main__':
	main() 