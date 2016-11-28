from iradio_structs import site_info, station_info
import os 

# absolute path to the music library
absolute_path = '/home/manu/Music/test_folder'

def file_parser():
	# create list of all genres availble in music library
	stations = [f for f in os.listdir(absolute_path) if os.path.isdir(
    	os.path.join(absolute_path, f))]

	for station in stations:
		# list of all radios
		all_radios = []
		with open(os.path.join(absolute_path,station,'config'), 'r') as f:
			# temp radio object
			radio1 = station_info()
			for line in f:
				if '#' in line or len(line.strip()) == 0:
					pass
				else:
					# create a list of radio and put in all_radio_list
					line = line.strip()
					tmp = line.split('\t')
					
					print 
					# radio1.tmp[0] = tmp[1]

		print radio1			


def hard_coded():
	r1 = station_info(1, 99,
					'RockFM', '224.1.1.1',
					5007, 5008,
					99)

	r2 = station_info(2, 99,
					'PopFM', '224.1.1.1',
					5009, 5010,
					99)

	all_radio = [r1.__dict__, r2.__dict__]

	final_object_send = site_info(10, 
							99,	'SNU\'s internet radio', 
							99, 'sample descrip',
							2, all_radio)
	
	return final_object_send

if __name__ == '__main__':
	# hard_coded()
	file_parser()