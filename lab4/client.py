from jsonsocket import Client
from multicast import iradio_client
import iradio_structs

import threading
import json
import sys # for exit

from multiprocessing import Process

host = sys.argv[1] if len(sys.argv) > 1 else '0.0.0.0'  # server IP address
port = 6969       # hard coded port

udp_port = sys.argv[2] if len(sys.argv) > 2 else 5432

def get_radio_info():
    client = Client()
    client.connect(host, port)

    print ("Sending request to server...")
    # object variables as dict
    client.send(iradio_structs.station_info_request().__dict__)
    radio_list = client.recv()
    print ("radio_list recieved")

    return radio_list


def print_radio_info(): 
    pass
    
def menu_invoke(response='NaN', init=0):
    if init is True:
        print ("""
        Welcome to SNU's best Radio, 
        Please input options accordingly
        """)

    return str(raw_input())

def play_multicast_radio():
    # create thread and pass the play multicast radio
    thread = threading.Thread(target=iradio_client.iradio, args=[udp_port])
    thread.start()

def main():
    print ("Requesting station list from server...")
    radio_list = get_radio_info()
    print ("Recieved station list..")
    print (radio_list)

    process = Process(target=iradio_client.iradio, args=(udp_port,))
    process.start()

    # user play and pause
    while True:
        user_in = raw_input('1: play or 0: pause or -1: exit \n')
        
        if user_in == '0' and process.is_alive() is True:
            process.terminate()
        elif user_in == '1' and process.is_alive() is False:
            process = Process(target=iradio_client.iradio, args=(udp_port,))
            process.start()
        elif user_in == '-1':
            print ('terminating program')
            process.terminate()
            sys.exit()
        else:
            print ('enter correct input \n')


if __name__ == '__main__':
    main()
