import json
import sys
from pprint import pprint

# Get json file from args
jsonfile = sys.argv[1]
# Base name of the map
map_name = jsonfile.split('/')[-1].split('.')[0]

# Create dictionary from json file
with open(jsonfile, 'r') as f:
    json_dict = json.loads(f.read())

# Get sizes of world and tiles
tile_size = json_dict['tilewidth']
width = json_dict['width']
height = json_dict['height']

# Extract all the layers into specific objects
layers = json_dict['layers']
layer_dict = {la['name']: la for la in layers}
paths = layer_dict['paths']
collision = layer_dict['collision']
bg = layer_dict['background']
bg2 = layer_dict['background2']
fg = layer_dict['foreground']
oh = layer_dict['overhead']

# Correct tile ids for each display layer
for i in range(width*height):
    bg['data'][i] -= 1
    bg2['data'][i] -= 1
    fg['data'][i] -= 1
    oh['data'][i] -= 1

# Save data to file in format of
# tile_size map_width_in_tiles map_height_in_tiles
with open(f'{map_name}_data.txt', 'w') as f:
    f.write(f'{tile_size} {width} {height}')

# Save each main display layer as simple numbers
with open(f'{map_name}_bg.txt', 'w') as f:
    for tile_id in bg['data']:
        f.write(f'{tile_id} ')
with open(f'{map_name}_bg2.txt', 'w') as f:
    for tile_id in bg2['data']:
        f.write(f'{tile_id} ')
with open(f'{map_name}_fg.txt', 'w') as f:
    for tile_id in fg['data']:
        f.write(f'{tile_id} ')
with open(f'{map_name}_oh.txt', 'w') as f:
    for tile_id in oh['data']:
        f.write(f'{tile_id} ')

with open(f'{map_name}_collision.txt', 'w') as f:
    size = len(collision['objects'])
    f.write(f'{size}\n')
    for obj in collision['objects']:
        x = obj['x']
        y = obj['y']
        w = obj['width']
        h = obj['height']
        f.write(f'{x} {y} {w} {h}\n')


with open(f'{map_name}_paths.txt', 'w') as f:
    size = len(paths['objects'])
    f.write(f'{size}\n')
    for obj in paths['objects']:
        x = obj['x']
        y = obj['y']
        name = obj['name']
        if 'properties' in obj:
            nex = obj['properties'][0]['value']
        else:
            nex = 'null'
        f.write(f'{name} {x} {y} {nex}\n')
