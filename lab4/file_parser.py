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
					'PopFM', '224.1.1.1',
					5432, 6969,
					len('PopFM'))

	r2 = station_info(2, 99,
					'ClassicFM', '224.1.1.1',
					5433, 6969,
					len('ClassicFM'))	
	r3 = station_info(3, 99,
					'RockFM', '224.1.1.1',
					5434, 6969,
					len('RockFM'))	
	r4 = station_info(4, 99,
					'HindiFM', '224.1.1.1',
					5435, 6969,
					len('HindiFM'))	
	r5 = station_info(4, 99,
					'Video', '224.1.1.1',
					5436, 6969,
					len('Video'))

	all_radio = [r1.__dict__, r2.__dict__, r3.__dict__, r4.__dict__, r5.__dict__]

	final_object_send = site_info(10, 
							len('SNU\'s internet radio'),	'SNU\'s internet radio', 
							len('sample descrip'), 'sample descrip',
							len(all_radio), all_radio)
	
	return final_object_send

if __name__ == '__main__':
	# hard_coded()
	file_parser()