rmdir build /s /q
cd doc
doxygen
%PYTHON% doxy2swig.py ./_build/xml/index.xml ../ext/python/documentation.i
cd ..
%PYTHON% setup.py install --single-version-externally-managed --record=record.txt
REM remove potentially existing build dir to avoid conflicts with previous builds
rmdir build /s /q
"%PYTHON%" setup.py install --single-version-externally-managed --record=record.txt

