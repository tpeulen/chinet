#!/usr/bin/env bash
rm -r -f build
cd doc
doxygen
$PYTHON doxy2swig.py _build/xml/index.xml ../pyext/documentation.i
cd ..
$PYTHON setup.py install --single-version-externally-managed --record=record.txt
