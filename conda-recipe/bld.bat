rmdir build /s /q
REM Install doxygen for documentation.
REM I do not use conda-forge. Thus, I download and install doxygen from the web
curl https://www.doxygen.nl/files/doxygen-1.8.20.windows.x64.bin.zip -O doxygen-1.8.20.windows.x64.bin.zip
REM https://stackoverflow.com/questions/17546016/how-can-you-zip-or-unzip-from-the-script-using-only-windows-built-in-capabiliti/26843122#26843122
powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('doxygen-1.8.20.windows.x64.bin.zip', 'doxygen'); }"
set PATH=%cd%\doxygen;%PATH%
cd doc
doxygen
$PYTHON doxy2swig.py ./_build/xml/index.xml ../ext/python/documentation.i
cd ..

$PYTHON setup.py install --single-version-externally-managed --record=record.txt
REM remove potentially existing build dir to avoid conflicts with previous builds
rmdir build /s /q
"%PYTHON%" setup.py install --single-version-externally-managed --record=record.txt
