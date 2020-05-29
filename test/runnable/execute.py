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

    return subprocess.run([compiler] + files, stdout=subprocess.PIPE).returncode


def execute():
    obj_file = join(kantan_path, 'out.o')
    exe_output = join(kantan_path, 'test.exe')
    subprocess.run(['gcc', obj_file, '-o', exe_output], stdout=subprocess.PIPE)

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

    for filename, expected in tests.items():
        if compile(filename) != 0:
            print(f'{error}: Compilation failed for {filename}')
            continue

        output = execute()
        if output != expected:
            print(f'{error}: in {filename}')
            print(f'expected {expected}, but got {output}')
        else:
            print(f'{success} {filename}')

    cleanup()

