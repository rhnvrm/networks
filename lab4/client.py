from jsonsocket import Client
from multicast import iradio_client
import iradio_structs

import threading
import json
import sys # for exit

from multiprocessing import Process

host = sys.argv[1] if len(sys.argv) > 1 else '0.0.0.0'  # server IP address
port = 5432       # hard coded port

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

        print ("""
        Radio playing at the moment, 
        Please choose relevant options...
        P: stop playback and close muticast reception
        R: resume playback
        C: change station, view updated radio list 
        X: terminate the client program
        """)

    return str(raw_input())

def play_multicast_radio():


    # # create thread and pass the play multicast radio
    thread = threading.Thread(target=iradio_client.iradio, args=[udp_port])
    thread.start()

    # rev_input = menu_invoke()
    # if rev_input == 'P':
    #   # close the thread
    #   # todo: gracefully close the socket and then thread.join()
    # elif rev_input == 'R':
    #   pass
    #   # recall same funtion recursively ? search better option
    # elif rev_input == 'C'
    #   pass
    # elif rev_input == 'X':
    #   # exit program
    #   sys.exit() 

#   # parent handles te input from the user 
#   # and stops the thread accordingly 

def main():
    print ("Requesting station list from server...")
    radio_list = get_radio_info()
    print ("Recieved station list..")
    print (radio_list)
 
    # thread = threading.Thread(target=iradio_client.iradio, args=[udp_port])
    # thread.start()


    process = Process(target=iradio_client.iradio, args=(udp_port,))
    process.start()

    # play_multicast_radio()
    # print_radio_info(radio_list)
    
    # menu_invoke(radio_list, 1)

    while True:
        user_in = raw_input('PL: play or PA: pause')
        if user_in == 'PA':
            process.join()
        elif user_in == 'PL':
            process = Process(target=iradio_client.iradio, args=(udp_port,))
            process.start()
        else:
            print ('enter better input')

    

if __name__ == '__main__':
    main()
