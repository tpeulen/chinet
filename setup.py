#! /usr/bin/env python
import sys
import os
import re
import platform
import subprocess
import fileinput
from distutils.version import LooseVersion

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


try:
    import IMP
except ImportError:
    IMP = None


def read_version(
        header_file='./include/CNode.h'
):
    version = "0.0.0"
    with open(header_file, "r") as fp:
        for line in fp.readlines():
            if "#define" in line and "CHINET_VERSION" in line:
                version = line.split()[-1]
    return version.replace('"', '')

VERSION = read_version()
LICENSE = 'Mozilla Public License 2.0 (MPL 2.0)'
NAME = "chinet"  # name of the module
DESCRIPTION = "Python bindings for chinet"
LONG_DESCRIPTION = """"chinet is a C++ library with Python wrapper to construct \
networks of computation node with associated ports that can be deposited in a \
mongoDB. Chinet is the data management backend of chisurf."""


def patch_windows_imp():
    if IMP.__version__ == '2.12.0':
        try:
            # we are likely in a conda build environment
            library_path = pathlib.Path(os.environ['LIBRARY_LIB'])
            filename = library_path / pathlib.Path("./cmake/IMP/IMPConfig.cmake")
        except KeyError:
            # we are likely not in a conda build environment
            library_path = pathlib.Path(os.environ['CONDA_PREFIX']) / \
                           pathlib.Path("./Library/lib/cmake/IMP/")
            filename = library_path / pathlib.Path("IMPConfig.cmake")
        with fileinput.FileInput(filename, inplace=True, backup='.bak') as file:
            for line in file:
                print(
                    line.replace(
                        ".so." + IMP.__version__, ".lib"
                    ).replace(
                        '${IMP_LIB_DIR}/lib', '${IMP_LIB_DIR}/'
                    )
                )
    elif '2.13' in IMP.__version__:
        try:
            # we are likely in a conda build environment
            library_path = pathlib.Path(os.environ['LIBRARY_LIB'])
            filename = library_path / pathlib.Path("./cmake/IMP/IMPConfig.cmake")
        except KeyError:
            # we are likely not in a conda build environment
            library_path = pathlib.Path(os.environ['CONDA_PREFIX']) / \
                           pathlib.Path("./Library/lib/cmake/IMP/")
            filename = library_path / pathlib.Path("IMPConfig.cmake")
        with fileinput.FileInput(filename, inplace=True, backup='.bak') as file:
            for line in file:
                print(line.replace(".dll", ".lib"),)
    else:
        return


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


def build_swig_documentation():
    # build the documentation.i file using doxygen and doxy2swig
    if not os.path.isfile("./pyext/documentation.i"):
        print("-- building documentation.i using doxygen and doxy2swig")
        path = os.path.dirname(os.path.abspath(__file__)) + "/doc"
        env = os.environ.copy()
        subprocess.check_call(["doxygen"], cwd=path, env=env)
        subprocess.check_call(
            ["python", "doxy2swig.py", "./_build/xml/index.xml", "../pyext/documentation.i"],
            cwd=path,
            env=env
        )


class CMakeBuild(build_ext):

    def run(self):
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        print("CHINET VERSION:", VERSION)
        build_swig_documentation()
        extdir = os.path.abspath(
            os.path.dirname(
                self.get_ext_fullpath(ext.name)
            )
        ).replace('\\', '/')

        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DCMAKE_SWIG_OUTDIR=' + extdir
        ]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        if platform.system() == "Windows":
            cmake_args += [
                '-DBUILD_PYTHON_INTERFACE=ON',
                '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir),
                '-GVisual Studio 14 2015 Win64'
            ]
        else:
            build_args += ['--', '-j8']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get(
                'CXXFLAGS', ''
            ),
            self.distribution.get_version()
        )
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        print("BUILDING: " + " ".join(cmake_args))
        subprocess.check_call(
            ['cmake', ext.sourcedir] + cmake_args,
            cwd=self.build_temp,
            env=env
        )
        subprocess.check_call(
            ['cmake', '--build', '.'] + build_args,
            # ['ninja'], # + build_args,
            cwd=self.build_temp
        )




setup(
    name=NAME,
    version=VERSION,
    license=LICENSE,
    author='Thomas-Otavio Peulen',
    author_email='thomas@peulen.xyz',
    ext_modules=[
        CMakeExtension('chinet')
    ],
    cmdclass={
        'build_ext': CMakeBuild
    },
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    install_requires=[
        'numpy'
    ],
    setup_requires=[
        'setuptools'
    ],
    zip_safe=False,
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: MIT License',
        'Natural Language :: English',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python',
        'Topic :: Scientific/Engineering',
    ]
)
