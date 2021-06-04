git submodule update --recursive --init --remote
rmdir build /s /q
cd doc
doxygen
%PYTHON% ../tools/doxy2swig.py ./_build/xml/index.xml ../pyext/documentation.i
cd ..
%PYTHON% setup.py install --single-version-externally-managed --record=record.txt
