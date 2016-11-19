from jsonsocket import Server
import socket

# host = socket.gethostbyname('hp')
host = 'localhost'
port = 4339


def iradio_req_process():

    # create server object
    server = Server(host, port)
    # accepting connections idefinately
    print ("ready to listen to requests")
    while True:
        server.accept()
        data = server.recv()

        print data
        # _process(data)
        server.send({'success': '1'})


def main():
    """ 
    create_multicast_radio_in_child_process()
    """
    iradio_req_process()


if __name__ == '__main__':
    main()
