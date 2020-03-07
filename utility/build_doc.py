#! /usr/bin/env python
import os
import subprocess

cwd = os.getcwd()
os.chdir('../utility/')
subprocess.call(
    "python doxy2swig.py ../docs/api/xml/index.xml ../pyext/documentation.i",
    shell=True
)
os.chdir(cwd)

os.chdir('../docs/')
subprocess.call("doxygen Doxyfile", shell=True)
subprocess.call("make html", shell=True)
