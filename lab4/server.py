from jsonsocket import Server

import socket
import os

from iradio_structs import station_info_request

# host = socket.gethostbyname('hp')
host = 'localhost'
port = 5432

def _process(data):

    print (data['type'])

def iradio_req_process():

    # create server object
    server = Server(host, port)
    # accepting connections idefinately
    print ("ready to listen to requests")
    while True:
        server.accept()
        data = server.recv()

        _process(data)
        server.send({'success': '1'})


def main():
    """ 
    create_multicast_radio_in_child_process()
    """ 
    iradio_req_process()



if __name__ == '__main__':
    main()
