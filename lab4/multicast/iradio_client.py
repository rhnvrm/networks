# !/usr/bin/env python

import socket
import struct
import subprocess
import time
import threading
import os
import sys

MCAST_GRP = '230.192.3.255'
MCAST_PORT = 5432 	# data port of multicast stream 

buf = 2048
play = True

def toggle_music():
	global play
	play = False if play is True else True

def iradio(port):

	# redirect ouput to a file
	# sys.stdout = open('client.log', 'w')

	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	
	# use MCAST_GRP instead of '' to listen only
	# to MCAST_GRP, not all groups on MCAST_PORT

	MCAST_LOCAL_PORT = int(port)
	s.bind(('', MCAST_LOCAL_PORT)) 
	
	# some multicast options
	mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
	s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

	size = 10*1024*1024
	s.setsockopt(socket.SOL_SOCKET,socket.SO_RCVBUF, size)
		
	commands = ['ffplay', '-']

	FNULL = open(os.devnull, 'w')
	player_process = subprocess.Popen(commands, stdin=subprocess.PIPE, stdout=FNULL, stderr=FNULL)
	# player_process.communicate()

	while True:
		try:
			data, addr = s.recvfrom(buf)
			if play is True:
				player_process.stdin.write(data)
			else:
				player_process.stdin.flush()
			s.settimeout(5)		
		
		except socket.timeout:
			print ("connection dropped")
			# no need to close socket
			# s.close()

"""
	while True:
	# replace with stdin return codes to terminate
		try:
  			proc = subprocess.check_output(commands, stderr=subprocess.STDOUT)
  			# do something with output
  			print proc
		except subprocess.CalledProcessError:
  			print 'There was an error - command exited with non-zero code'
  			break

  	print 'exiting program'

 	player_process.wait()
"""

def main():
	iradio(MCAST_PORT)

if __name__ == '__main__':
 	main() 