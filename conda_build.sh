if [ -z ${CONDA_BUILD+x} ]; then
    source /home/tpeulen/miniconda3/conda-bld/chinet_1618874035999/work/build_env_setup.sh
fi
#!/bin/bash
rm -r -f build
setup.py install --single-version-externally-managed --record=record.txt
