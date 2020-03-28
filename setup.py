#! /usr/bin/env python
import os
import re
import platform
import subprocess
import fileinput
import pathlib

from setuptools import setup, Extension
from distutils.command.build_ext import build_ext
from distutils.version import LooseVersion


IMP = None


def read_version(
        header_file='./include/CNode.h'
):
    version = "0.0.0"
    with open(header_file, "r") as fp:
        for line in fp.readlines():
            if "#define" in line and "VERSION" in line:
                version = line.split()[-1]
    return version


def patch_windows_imp():
    import IMP
    if IMP.__version__ > '2.12.0':
        return
    try:
        # we are likely in a conda build environment
        library_path = pathlib.Path(os.environ['LIBRARY_LIB'])
        filename = library_path / pathlib.Path("./cmake/IMP/IMPConfig.cmake")
    except KeyError:
        # we are likely not in a conda build environment
        library_path = pathlib.Path(os.environ['CONDA_PREFIX']) / \
                       pathlib.Path("./Library/lib/cmake/IMP/")
        filename = library_path / pathlib.Path("IMPConfig.cmake")
    with fileinput.FileInput(
            filename, inplace=True, backup='.bak'
    ) as file:
        for line in file:
            print(
                line.replace(
                    ".so." + IMP.__version__, ".lib"
                ).replace(
                    '${IMP_LIB_DIR}/lib', '${IMP_LIB_DIR}/'
                ),
                end=''
            )

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):

    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake include_dirs.append(st be installed to build the "
                "following extensions: " + ", ".join(
                    e.name for e in self.extensions)
            )

        if platform.system() == "Windows":
            cmake_version = LooseVersion(
                re.search(r'version\s*([\d.]+)', out.decode()).group(1)
            )
            if cmake_version < '3.13.0':
                raise RuntimeError("CMake >= 3.13.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(
                self.get_ext_fullpath(ext.name)
            )
        ).replace('\\' , '/')
        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DCMAKE_SWIG_OUTDIR=' + extdir,
        ]
        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        if platform.system() == "Windows":
            patch_windows_imp()
            cmake_args += [
                '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                    cfg.upper(), extdir
                ),
                '-GVisual Studio 14 2015 Win64'
            ]
        else:
            build_args += ['--', '-j8']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version()
        )
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(
            ['cmake', ext.sourcedir] + cmake_args,
            cwd=self.build_temp,
            env=env
        )
        subprocess.check_call(
            ['cmake', '--build', '.'] + build_args,
            cwd=self.build_temp
        )


__name__ = "chinet"  # name of the module
__version__ = read_version()


cmdclass = {
    'build_ext': CMakeBuild
}


setup(
    name=__name__,
    version=__version__,
    license='MPL v2.0',
    author='Thomas-Otavio Peulen',
    author_email='thomas.otavio.peulen@gmail.com',
    ext_modules=[CMakeExtension('chinet')],
    cmdclass=cmdclass,
    install_requires=[
        'numpy'
    ],
    setup_requires=[
        'numpy'
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
