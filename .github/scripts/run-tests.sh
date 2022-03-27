#!/bin/bash

set -e

echo "::group::PySteamTables unit tests"
pytest -v --pyargs pysteamtables \
       --cov=${pysteamtables_libdir:=$(python -c "import pysteamtables; print(pysteamtables.__path__[0])")} \
       --cov-config=${pysteamtables_srcdir:=$(realpath ./pysteamtables-src)}/.coveragerc
mv .coverage ${pysteamtables_srcdir}/.coverage.pysteamtables
echo "::endgroup::"

echo "::group::run pysteamtables.test() inside interpreter"
echo 'import pysteamtables; pysteamtables.test()' > runtest.py
coverage run --source ${pysteamtables_libdir} --rcfile ${pysteamtables_srcdir}/.coveragerc runtest.py
mv .coverage ${pysteamtables_srcdir}/.coverage.pysteamtables-inline

echo "::group::Combine coverage"
# needs to run from within pysteamtables source dir
cd ${pysteamtables_srcdir}
echo "  ${pysteamtables_libdir}" >> .coveragerc
coverage combine
coverage report
echo "::endgroup::"