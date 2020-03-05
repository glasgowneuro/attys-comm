#!/bin/sh
doxygen
python3 ./doxy2swig.py -f -t -c -a -o doc/xml/index.xml docstrings.i
cd doc/pdf
make
