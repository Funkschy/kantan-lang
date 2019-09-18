#!/usr/bin/python3

import pathlib
import subprocess
import os
from os import path as osp

class Expected(object):
    def __init__(self, path):
        with open(path, 'r') as f:
            self.string = f.read()

        self.file = f.name
        self.returncode = self.read_returncode()
        self.text = '\n'.join(self.string.splitlines()[1:])

    def read_returncode(self):
        if not self.string.startswith('# ret'):
            return None

        return int(self.string.splitlines()[0][6:])

def run_cmd(command_list):
    return subprocess.run(command_list, stdout=subprocess.PIPE, encoding='utf-8')

def compile(exe, files):
    args = [exe] + files
    return run_cmd(args)

def get_expected(f):
    e = pathlib.Path(f).with_suffix('.expected')

    if not osp.exists(e):
        return None

    return Expected(e)

def get_first_diff(first, second):
    if len(first) != len(second):
        return min(len(first), len(second))

    for i, c in enumerate(first):
        if second[i] != c:
            return i

    return 0

def compare_result(result, expected, f):
    if result.returncode != expected.returncode:
        print('FAILED: wrong returncode, expected {}, but got {}: {}'.format(
            expected.returncode, result.returncode, f
        ))

    if result.stdout != expected.text:
        diff = get_first_diff(str(result.stdout), expected.text)
        print('FAILED: wrong output, expected\n\'{}\',\nbut got:\n{}\nin {}'.format(
            expected.text[diff:], str(result.stdout)[diff:], f
        ))

def run_tests(exe, testdir):
    files = map(lambda f: osp.join(testdir, f), os.listdir(testdir))

    for f in files:
        if f.endswith('.expected'):
            continue

        if osp.isdir(f):
            files = list(map(lambda child: osp.join(f, child), os.listdir(f)))
            result = compile(exe, files)
        elif f.endswith('.kan'):
            result = compile(exe, [f])
        else: continue

        expected = get_expected(f)
        if expected is None:
            print('ERROR no \'expected\' file for ' + f)
            continue
        elif expected.returncode is None:
            print('ERROR no returncode in \'expected\' file: ' + expected.file)
            continue

        compare_result(result, expected, f)

if __name__ == '__main__':
    p = pathlib.Path(__file__).parent
    run_tests(str(p) + '/../compiler', str(p) + '/files')
