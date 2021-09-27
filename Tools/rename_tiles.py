import os
import sys

base = sys.argv[2]
path = sys.argv[1]
all_files = os.listdir(path)
tile_files = [file for file in all_files if 'tile' in file]
for file in tile_files:
    tile_id = int(file[4:-4])
    nfile = f'{base}{tile_id}.png'
    os.rename(os.path.join(path, file), os.path.join(path, nfile))