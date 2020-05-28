#!/bin/bash
# runs the kantan tests

python3 ./kantan-test/src/main.py $(realpath ../compiler) $(realpath ./files) --valgrind=True --suppress=$(realpath ../suppress-llvm-errors.supp)
rm out.o
