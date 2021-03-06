#!/usr/bin/python3
# Temporary test runner, until kantanc has commandline arguments and this
# can be properly integrated into kantan-test

import subprocess

from os import listdir, remove
from os.path import splitext, join, isdir, exists

kantan_path = '/home/felix/Documents/programming/kantan/compiler/'
test_path = join(kantan_path, 'test/runnable')
kantan_exe = 'compiler'

class TermColor:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def colored(color, text):
    return color + text + TermColor.ENDC

def collect():
    filename_to_expected = {}

    for f in listdir(test_path):
        if not '.expected' in f:
            continue

        f = join(test_path, f)

        expected_content = ''
        with open(f) as expected:
            expected_content = expected.read()

        kantan_filename = splitext(f)[0]
        if not isdir(kantan_filename):
            kantan_filename = splitext(f)[0] + '.kan'

        filename_to_expected[kantan_filename] = expected_content

    return filename_to_expected


def compile(filename):
    files = [filename]
    compiler = join(kantan_path, kantan_exe)

    if isdir(filename):
        files = list(map(lambda f: join(test_path, filename ,f), listdir(filename)))

    exe_output = join(kantan_path, 'test.exe')
    return subprocess.run([compiler, '-g', '-O1','-o', exe_output] + files, stdout=subprocess.PIPE).returncode


def execute():
    exe_output = join(kantan_path, 'test.exe')

    return subprocess.run(
            exe_output,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
    ).stdout.decode('utf8')


def cleanup():
    obj_file = join(kantan_path, 'out.o')
    exe_output = join(kantan_path, 'test.exe')

    if exists(obj_file):
        remove(obj_file)

    if exists(exe_output):
        remove(exe_output)


if __name__ == '__main__':
    tests = collect()
    error = f'{colored(TermColor.FAIL, "ERROR")}'
    success = f'{colored(TermColor.OKGREEN, "SUCCESS")}'
    failed_count = 0

    items = sorted(tests.items(), key = lambda item: item[0])

    for filename, expected in items:
        if compile(filename) != 0:
            print(f'{error}: Compilation failed for {filename}')
            failed_count += 1
            continue

        output = execute()
        if output.strip() != expected.strip():
            print(f'{error}: in {filename}')
            print(f'expected:\n{expected}, but got:\n{output}')
            failed_count += 1
        else:
            print(f'{success} {filename}')

    print(f'{colored(TermColor.OKGREEN, "OK")}: {len(items) - failed_count}')
    if failed_count > 0:
        print(f'{colored(TermColor.FAIL, "FAILED")}: {failed_count}')

    cleanup()

