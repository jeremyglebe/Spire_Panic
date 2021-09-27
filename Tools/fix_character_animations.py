import os
import sys

path = sys.argv[1]
base = sys.argv[2]
base_sz = len(base)
ext = '.png'

tran = [
    (0, 3),
    (1, 2),
    (2, 0),
    (3, 1)
]

files = [file for file in os.listdir(path) if file[:base_sz] == base]
num_files = len(files)

# Sort the list by frame numbers
files.sort(key=lambda x: int(x[base_sz:-4]))
# print(files)

num = 0
for file in files:
    name = file[:base_sz]
    os.rename(os.path.join(path, file), os.path.join(path, f'{name}{num}{ext}'))
    num += 1

new_ids = []
for i in range(num_files):
    step = num_files / 4
    tr_id = int(i / step)
    tr = tran[tr_id]
    nid = int(i - (tr[0] * step) + (tr[1] * step))
    new_ids.append(nid)

for i in range(len(files)):
    new_file = f'{base}{new_ids[i]}'
    os.rename(os.path.join(path, files[i]), os.path.join(path, new_file))

files = [file for file in os.listdir(path) if file[:base_sz] == base]
for file in files:
    os.rename(os.path.join(path, file), os.path.join(path, f'{file}{ext}'))
