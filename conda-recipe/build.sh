#!/usr/bin/env bash
git submodule update --recursive --init --remote
rm -r -f build
cd doc
doxygen
$PYTHON ../tools/doxy2swig.py _build/xml/index.xml ../pyext/documentation.i
cd ..
$PYTHON setup.py install --single-version-externally-managed --record=record.txt
