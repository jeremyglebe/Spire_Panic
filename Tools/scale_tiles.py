#!/usr/bin/python
from PIL import Image
import os, sys

path = sys.argv[1]
dirs = os.listdir( path )

def resize():
    for item in dirs:
        if os.path.isfile(path+item) and item[:4] == 'tile':
            im = Image.open(path+item)
            f, e = os.path.splitext(path+item)
            imResize = im.resize((32,32))
            imResize.save(f + '.png', 'PNG')

resize()