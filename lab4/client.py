from jsonsocket import Client
from multicast import iradio_client
import iradio_structs

import threading
import json
import sys

from multiprocessing import Process
from flask import Flask, jsonify, render_template, request

app = Flask(__name__)

process = None



host = sys.argv[1] if len(sys.argv) > 1 else '0.0.0.0'  # server IP address
port = 6969       # hard coded port

udp_port = sys.argv[2] if len(sys.argv) > 2 else 5432

@app.route("/")
def hello():
    return render_template("index.html")

@app.route("/toggle")
def toggle():
    global process
    if process.is_alive():
        process.terminate()
    elif not process.is_alive():
        process = Process(target=iradio_client.iradio, args=(udp_port,))
        process.start()

    return "Toggle Completed!"

@app.route("/shutdown")
def shutdown():
    process.terminate()
    sys.exit()
    return ('terminating program')

@app.route("/radioinfo")
def get_radio_info():
    client = Client()
    client.connect(host, port)

    print ("Sending request to server...")
    # object variables as dict
    client.send(iradio_structs.station_info_request().__dict__)
    radio_list = client.recv()
    print ("radio_list recieved")

    return jsonify(**radio_list)

@app.route("/change")
def change_radio():
    global udp_port
    global process
    udp_port = request.args.get('port')
    process.terminate()
    process = Process(target=iradio_client.iradio, args=(udp_port,))
    process.start()
    return "Port changed to "+ udp_port


def print_radio_info(): 
    pass
    
def menu_invoke(response='NaN', init=0):
    if init is True:
        print ("""
        Welcome to SNU's Radio, 
        Please input options accordingly
        """)

    return str(raw_input())

def play_multicast_radio():
    # create thread and pass the play multicast radio
    thread = threading.Thread(target=iradio_client.iradio, args=[udp_port])
    thread.start()

def main():

    print ("Requesting station list from server...")
    #radio_list = get_radio_info()
    #print ("Recieved station list..")
    #print (radio_list)
    global process
    process = Process(target=iradio_client.iradio, args=(udp_port,))
    process.start()

    app.run()


if __name__ == '__main__':
    main()
