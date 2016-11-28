from jsonsocket import Server # udp
from multicast import iradio_server #multicast module
from iradio_structs import station_info_request # iradio 

from multiprocessing import Process 
import socket
import os

host = '0.0.0.0'
port = 6969
path = '/home/manu/Music/test_folder' # path to music library folders

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

def music_folder_names():
	folders_in_path = os.listdir(path)
	
	return folders_in_path

def main():
    ascii_art()

    port = 5432

    # yet to implement
    folder_list = get_music_folder_names()
    
    for i in range(1,4): # assuming directory contains 1.ts, 2.ts, etc. from where program is run
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
