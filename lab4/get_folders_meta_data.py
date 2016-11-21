import os

# absolute path to the music library
absolute_path = '/home/manu/Music/'

# create list of all genres availble in music library
genres = [f for f in os.listdir(absolute_path) if os.path.isdir(
    os.path.join(absolute_path, f))]

# print all folders
for folders in genres:
    print folders

# traverse all the folders inside iradio directory
# and list out details of the all song files inside it

