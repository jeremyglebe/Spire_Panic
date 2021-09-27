import os
import sys

prefix = sys.argv[2]
path = sys.argv[1]
all_files = os.listdir(path)
for file in all_files:
    tile_id = int(file[0:-4])
    nfile = f'{prefix}{tile_id}.png'
    os.rename(os.path.join(path, file), os.path.join(path, nfile))