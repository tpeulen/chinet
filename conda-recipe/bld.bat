git submodule update --recursive --init --remote --force
rmdir build /s /q
"%PYTHON%" setup.py install --single-version-externally-managed --record=record.txt

