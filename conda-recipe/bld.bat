git submodule update --recursive --init --remote
rmdir build /s /q
"%PYTHON%" setup.py install --single-version-externally-managed --record=record.txt

