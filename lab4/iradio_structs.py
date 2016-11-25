# Client to server messages
class station_info_request:

	def __init__(self, packet_type=10):
		self.type = 10 # hard_coding packet type to 10 (default)

# Server to client messages

class station_info():

	def __init__(self, station_number='',
				station_name_size='', station_name='',
				multicast_address='', data_port='', 
				info_port='', bit_rate=''):
	
		self.station_number = station_number
		self.station_name_size = station_name_size
		self.station_name = station_name
		self.multicast_address = multicast_address
		self.data_port = data_port
		self.info_port = info_port
		self.bit_rate = bit_rate


# station list is a list of all station's station info objects
class site_info:

	def __init__(self, type,
				 site_name_size, site_name,
				 site_desc_size, site_desc,
				 station_count, station_list):
		self.type = type
		self.site_name_size = site_name_size
		self.site_name = site_name # list
		self.site_desc_size = site_desc_size 
		self.site_desc = site_desc
		self.station_count = station_count
		self.station_list = station_list # list of station info type


# removed in the latest lab pdf

class station_not_found:

	def __init__(self, station_number, type=11):
		self.type = 11
		self.station_number = station_number


# Stream information messages (multicast)
class song_info:

	def __init__(self, type,
				 song_name_size, 
				 song_name,
				 remaining_time_in_sec,
				 next_song_name_size,
				 next_song_name):
		self.type = type
		self.song_name_size = song_name_size
		self.song_name = song_name
		self.remaining_time_in_sec = remaining_time_in_sec
		self.next_song_name_size = next_song_name_size # string length 
		self.next_song_name = next_song_name # string
