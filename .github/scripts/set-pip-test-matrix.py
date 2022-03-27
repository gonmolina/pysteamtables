""" set-pip-test-matrix.py

Create test matrix for pip wheels
"""
import json
from pathlib import Path

system_opt_blas_libs = {'ubuntu': [],
                        'macos' : []}

wheel_jobs = []
for wkey in Path("pysteamtables-wheels").iterdir():
    wos, wpy = wkey.name.split("-")
    wheel_jobs.append({'packagekey':  wkey.name,
                        'os': wos,
                        'python': wpy,
                        })

matrix = { 'include': wheel_jobs }
print(json.dumps(matrix))