# !/usr/bin/env python

import socket
import time
import os
import json
import shlex
import subprocess

MCAST_GRP = '230.192.3.255'
MCAST_PORT = 5432

buf = 2048

def get_bit_rate_radio(path):
	cmd = 'ffprobe -print_format json -show_streams'
	args = shlex.split(cmd)
	args.append(path)

	ffprobeOutput = json.loads(subprocess.check_output(args).decode('utf-8'))
	
	return int(ffprobeOutput['streams'][1]['bit_rate'])


def iradio(file_name, port):
	# create socket 
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	
	# some multicast options
	s.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
	
	""" Sample way of sending data to multicast group
	s.sendto("robot", (MCAST_GRP, MCAST_PORT))
	"""
	size = 10*1024*1024
	s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, size)

	sleep = (buf*8.0/get_bit_rate_radio(file_name))*0.90
	print (sleep)

	MCAST_LOCAL_PORT = int(port) 

	while True:

		# sending one song at a time 
		f = open(file_name, 'rb')

		data = f.read(buf)
		while data:
			if s.sendto(data, (MCAST_GRP, MCAST_LOCAL_PORT)):
				# print (".")
				data = f.read(buf)
				time.sleep(sleep)

		
		f.close()
		print ("Send Successful!")

	s.close()


def main():
	iradio('2.ts', MCAST_PORT)

if __name__ == '__main__':
	main() 