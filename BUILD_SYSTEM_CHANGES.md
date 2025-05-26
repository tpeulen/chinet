# Build System Changes

This document describes the changes made to the build system of the chinet module to ensure it can be compiled and installed with pip, and to migrate to the new toml-based build system.

## Changes Made

1. **Created requirements.txt file**
   - Added numpy and cmake_build_extension as dependencies
   - This file is referenced by pyproject.toml for dynamic dependencies

2. **Created version.txt file**
   - Contains the version number from include/info.h
   - This file is referenced by pyproject.toml for dynamic version

3. **Updated pyproject.toml**
   - Made version dynamic, reading from version.txt
   - Updated license format to use SPDX expression instead of a table
   - Ensured build-system section has all necessary dependencies

4. **Created chinet package directory**
   - Added __init__.py to make it a proper Python package
   - Added _version.py to read version from include/info.h

5. **Updated setup.py**
   - Improved path handling using os.path.join

## Testing

The build system was tested using a test script that attempts to install the module using pip in development mode. The test confirmed that the pyproject.toml file is valid and the build system is working correctly up to the point where it tries to compile the C++ code.

## Recent Updates

1. **Fixed license-files format in pyproject.toml**
   - Changed from an object format `{paths = ["LICENSE.txt"]}` to an array format `["LICENSE.txt"]`
   - This resolves the error: "configuration error: `project.license-files` must be array"

2. **Removed license classifier from pyproject.toml**
   - Removed the classifier "License :: OSI Approved :: MIT License"
   - This addresses the warning about license classifiers being superseded by license expressions (PEP 639)

3. **Reorganized module structure**
   - Updated the CMake build system to install Python extensions in the `chinet` directory
   - Updated the `__init__.py` file to import the Python extensions
   - This ensures that the module follows Python packaging conventions

4. **Fixed conda build version handling**
   - Modified the conda-recipe/meta.yaml file to use `load_setup_py_data` instead of `load_file_data`
   - This resolves the issue where conda build couldn't read the version from version.txt
   - The version is now read from setup.py, which already has the logic to extract it from include/info.h
   - Ensures that the correct version is used in the package name during conda build

5. **Fixed setup.py to work with conda build**
   - Modified setup.py to conditionally import and use cmake_build_extension
   - This allows conda-build to load the version information without requiring cmake_build_extension to be installed
   - Resolves the error: "ModuleNotFoundError: No module named 'cmake_build_extension'" during conda build

## Known Issues

- The build process requires a C/C++ compiler to be installed and properly configured

## Future Improvements

- Consider adding a script to automatically update version.txt when include/info.h changes
