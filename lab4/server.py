from jsonsocket import Server # udp
from multicast import iradio_server #multicast module
from iradio_structs import station_info_request # iradio 

from multiprocessing import Process 
import socket
import os


host = '0.0.0.0'
port = 6969

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

        print ("Radio list sent to client #{}".format(count_clients))
        count_clients += 1
        print (count_clients)
        # server.send({'success': '1'})

def create_multicast_radio():
    thread = threading.Thread(target=iradio_server.iradio, args=['one.ts'])
    thread.start()


def main():
    ascii_art()

    port = 5432
    for i in range(1,4):
        multicast_process = Process(target=iradio_server.iradio, args=('{}.ts'.format(i), port,))
        multicast_process.start()
        port += 1

    print ('finally..')
    iradio_req_process()


def ascii_art():

    print ("""

                                       ._ o o
                                       \_`-)|_
                                    ,""       \
                                  ,"  ## |      .
                                ," ##   ,-\__    `.
                              ,"       /     `--._;)
                            ,"     ## /
                          ,"   ##    /
    """)

if __name__ == '__main__':
    main()
