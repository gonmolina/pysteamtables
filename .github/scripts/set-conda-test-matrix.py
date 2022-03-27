""" set-conda-test-matrix.py
Create test matrix for conda packages
"""
import json, re
from pathlib import Path

osmap = {'linux': 'ubuntu',
        # 'osx': 'macos',
         'win': 'windows',
         }


combinations = {'ubuntu': [''],
                # 'macos': [''],
                'windows': [''],
               }

conda_jobs = []
for conda_pkg_file in Path("pysteamtables-conda-pkgs").glob("*/*.tar.bz2"):
    cos = osmap[conda_pkg_file.parent.name.split("-")[0]]
    m = re.search(r'py(\d)(\d+)_', conda_pkg_file.name)
    if m is not None:
        pymajor, pyminor = int(m[1]), int(m[2])
        cpy = f'{pymajor}.{pyminor}'
    else:
        cpy = 0.0

    cjob = {'packagekey': f'{cos}-{cpy}',
            'os': cos,
            'python': cpy
            }
    conda_jobs.append(cjob)

matrix = { 'include': conda_jobs }
print(json.dumps(matrix))