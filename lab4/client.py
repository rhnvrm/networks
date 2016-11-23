from jsonsocket import Client
import multicast 

import threading
import json
import sys # for exit

host = '0.0.0.0'  # server IP address
port = 5432 	 # hard coded port


def get_radio_info():
    client = Client()
    client.connect(host, port)

    print "Sending request to server..."
    # object variables as dict
    client.send(iradio_req().__dict__)
    radio_list = client.recv()

    return radio_list


def menu_invoke(respnse='NaN', init=0):
    if init is True:
        print """
        Welcome to SNU's best Radio, 
        Please input options accordingly
        """

        print """
        Radio playing at the moment, 
        Please choose relevant options...
        P: stop playback and close muticast reception
        R: resume playback
        C: change station, view updated radio list 
        X: terminate the client program
        """ 

        return str(raw_input())

def play_multicast_radio():
    # create thread and pass the play multicast radio
    thread = Thread(target = multicast.client.play_multicast_radio())
    thread.start()

    rev_input = menu_invoke()
    if rev_input == 'P':
        # close the thread
        # todo: gracefully close the socket and then thread.join()
    elif rev_input == 'R':
        # recall same funtion recursively ? search better option
    elif rev_input == 'C'
        pass
    elif rev_input == 'X':
        # exit program
        sys.exit() 

    # parent handles te input from the user 
    # and stops the thread accordingly 

def main():
    print "Requesting station list from server..."
    radio_list = get_radio_info()
    menu_invoke(radio_list, 1)
    

if __name__ == '__main__':
    main()
