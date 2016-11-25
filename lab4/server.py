from jsonsocket import Server
from multicast import iradio_server
import socket
import os

from iradio_structs import station_info_request

# host = socket.gethostbyname('hp')
host = 'localhost'
port = 5432

def _process(data):
    pass
    # print (data['type'])

def iradio_req_process():

    # Create jsonserver object
    server = Server(host, port)

    # Accepting client requests idefinately
    print ("ready to listen to requests")

    # Keeping track of no. of clients 
    count_clients = 0

    while True:
        server.accept()
        data = server.recv()

        _process(data)

        # testing with dummy data
        from file_parser import hard_coded 
        server.send(hard_coded().__dict__)

        print "Radio list sent to client #{}".format(count_clients)
        count_clients += 1
        print count_clients
        # server.send({'success': '1'})


def create_multicast_radio():
    

def ascii_art():

    print """

                                       ._ o o
                                       \_`-)|_
                                    ,""       \ 
                                  ,"  ## |      . 
                                ," ##   ,-\__    `.
                              ,"       /     `--._;)
                            ,"     ## /
                          ,"   ##    /
    """

def main():
    ascii_art()
    
    # spwan multicast radio's 
    create_multicast_radio()
    
    iradio_req_process()

if __name__ == '__main__':
    main()
