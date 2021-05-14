#!/usr/bin/env bash
git submodule update --recursive --init --remote
rm -r -f build
$PYTHON setup.py install --single-version-externally-managed --record=record.txt
