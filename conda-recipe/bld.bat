 
set BLAS_ROOT=%PREFIX%
set LAPACK_ROOT=%PREFIX%
set GSL_ROOT_DIR=%PREFIX%

"%PYTHON%" setup.py install -G "NMake Makefiles"

if errorlevel 1 exit 1