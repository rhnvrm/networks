# !/usr/bin/env python

import socket
import time
import os
import json
import shlex
import subprocess

# MCAST_GRP = '224.1.1.1'
MCAST_GRP = '230.192.3.255'
MCAST_PORT = 5432

buf = 2048

def get_bit_rate_radio(path):
	cmd = 'ffprobe -print_format json -show_streams'
	args = shlex.split(cmd)
	args.append(path)

	ffprobeOutput = json.loads(subprocess.check_output(args).decode('utf-8'))
	
	return int(ffprobeOutput['streams'][1]['bit_rate'])


def iradio():
	# create socket 
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	
	# some multicast options
	s.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
	
	""" Sample way of sending data to multicast group
	s.sendto("robot", (MCAST_GRP, MCAST_PORT))
	"""
	size = 10*1024*1024
	s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, size)

	sleep = (buf*8.0/get_bit_rate_radio('output_mp2.ts'))*0.95
	print (sleep)

	while True:

		# sending one song at a time for now 
		f = open('output_mp3.ts', 'rb')

		data = f.read(buf)
		while data:
			if s.sendto(data, (MCAST_GRP, MCAST_PORT)):
				# print (".")
				data = f.read(buf)
				time.sleep(sleep) # 5 milli

		
		f.close()
		print ("Send Successful!")

	s.close()

	# while True:
	# 	for i in range(100):
	# 		s.sendto(str(i), (MCAST_GRP, MCAST_PORT))
	# 		print "sending ", i
	# 		time.sleep(0.05/10) 


def main():
	iradio()

if __name__ == '__main__':
	main() 