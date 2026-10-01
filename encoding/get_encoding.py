import sys
import numpy as np
import string



def update_charlist(chars, file):
    """Gets character counts from a file and updates a dict"""
    with open(file, 'r') as fin:
        text = fin.read()
    for i in text:
        chars[i] += 1
    return chars

def get_all_chars(file):
    """
    Takes file of list of filepaths and runs update charlist on all
    """
    characters = {i:0 for i in string.prinable}
    with open(file, 'r') as fin:
        files = fin.read().split('\n')
    for path in files:
        update_charlist(characters, path)
    return charlist

if __name__ == '__main__':
    files = sys.argv[1]
    get_all_chars(files)
