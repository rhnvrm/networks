class station_info_request:

    def __init__(self, packet_type, senders_name):
        self.type = packet_type
        self.senders_name = senders_name


class site_info:

    def __init__(self, type,
                 site_name_size, site_name,
                 site_desc_size, site_desc,
                 station_count, station_list):
        self.type = type
        self.site_name_size = site_name_size
    # skipping array based


class song_info:

    def __init__(self, type,
                 song_name_size, song_name,
                 remaining_time_in_sec,
                 next_song_name_size,
                 next_song_name)
