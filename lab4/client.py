from jsonsocket import Client
import json


class radio_info:

    def __init__(self, packet_type, senders_name):
        self.type = packet_type
        self.senders_name = senders_name

host = '0.0.0.0'  # server IP address
port = 4339 	 # hard coded port


def get_radio_info(iradio_req):
    client = Client()
    client.connect(host, port)

    # object variables as dict
    client.send(iradio_req.__dict__)
    response = client.recv()
    print(response)

    return response


def main():
    iradio_req = radio_info(10, "mohak")
    radio_details = get_radio_info(iradio_req)


if __name__ == '__main__':
    main()
