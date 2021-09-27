import sys
import os
import pprint
import re

# Get the folder we are logging
stat_path = sys.argv[1]

# Create the SVN log
os.system(f'svn log "{stat_path}" -r 7:HEAD --diff > Tools/stats/svn.log')

# Read the log file into a string
with open('Tools/stats/svn.log', 'r') as f:
    log_data = f.read()

# Split the log into revisions
revisions = log_data.split(
    '------------------------------------------------------------------------')[1:]

# Simple revisions will hold a very simplified version of the revisions
simple_revisions = []
# Process and simplify every revision
for rev in revisions:
    # We just want to ignore any triple pluses or minuses
    rev = rev.replace('+++', '')
    rev = rev.replace('---', '')
    # Mark lines that are meant to indicate an added line, a subtracted line, or an authorship line
    rev = re.sub(r'^\+.*?$', 'PLUS', rev, flags=re.DOTALL | re.MULTILINE)
    rev = re.sub(r'^-.*?$', 'MINUS', rev, flags=re.DOTALL | re.MULTILINE)
    rev = re.sub(r'^r[0-9]* \|', 'AUTHOR', rev, flags=re.DOTALL | re.MULTILINE)
    # Remove all lines that are just file contents
    rev = '\n'.join([line for line in rev.splitlines()
                     if 'PLUS' in line or 'MINUS' in line or 'AUTHOR' in line])
    # Now that the revision is very simple data, push it to our new revisions object
    simple_revisions.append(rev)

# Counters keeps track of how many lines were added/removed
counters = {}
# Extract author and increase counters for each simplified revision
for rev in simple_revisions:
    # Our counters dictionary will be keyed by revision authors
    author = ''
    # Read each line of the simplified revision
    for line in rev.splitlines():

        # Extract the author and initialize their counter if necessary
        if 'AUTHOR' in line:
            author = line.split()[1]
            if not author in counters:
                counters[author] = {
                    'added': 0,
                    'deleted': 0,
                    'net': 0,
                    'absolute': 0
                }

        # Keep track of additions/deletions
        if 'PLUS' in line:
            counters[author]['added'] += 1
            counters[author]['net'] = counters[author]['added'] - \
                counters[author]['deleted']
            counters[author]['absolute'] = counters[author]['added'] + \
                counters[author]['deleted']
        if 'MINUS' in line:
            counters[author]['deleted'] += 1
            counters[author]['net'] = counters[author]['added'] - \
                counters[author]['deleted']
            counters[author]['absolute'] = counters[author]['added'] + \
                counters[author]['deleted']

# Print our final stats
print("Table of Lines Committed:\n")
print("{:>9} | {:>8} | {:>8} | {:>8} | {:>8}".format('Author', 'Add', 'Del', 'Net', 'Absolute'))
print('-'*53)
for author, c in counters.items():
    print("{:>9} | {:>8} | {:>8} | {:>8} | {:>8}"
          .format(author, c['added'], c['deleted'], c['net'], c['absolute']))
