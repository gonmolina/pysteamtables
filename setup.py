#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""PySteamTables: A python wrapper for Freesteam steam tables
"""
import os
import sys
import subprocess
import re
from skbuild import setup
from skbuild.command.sdist import sdist

try:
    import configparser
except ImportError:
    import ConfigParser as configparser



DOCLINES = __doc__.split("\n")

CLASSIFIERS = """\
Development Status :: 4 - Beta
Intended Audience :: Science/Research
Intended Audience :: Developers
License :: OSI Approved :: GNU General Public License v2 (GPLv2)
Programming Language :: C
Programming Language :: Python :: 3
Programming Language :: Python :: 3.8
Programming Language :: Python :: 3.9
Programming Language :: Python :: 3.10
Programming Language :: Python :: 3.11
Programming Language :: Python :: 3.12
Programming Language :: Python :: 3.13
Programming Language :: Python :: 3.14
Topic :: Software Development
Topic :: Scientific/Engineering
Operating System :: Microsoft :: Windows
Operating System :: POSIX
Operating System :: Unix
Operating System :: MacOS
"""

# defaults
ISRELEASED = True
# assume a version set by conda, next update with git,
# otherwise count on default
VERSION = '0.0.1a1'


class GitError(RuntimeError):
    """Exception for git errors occurring in git_version"""
    pass


def git_version(srcdir=None):
    """Return the git version, revision and cycle"""
    def _minimal_ext_cmd(cmd, srcdir):
        env = {}
        for k in ['SYSTEMROOT', 'PATH']:
            v = os.environ.get(k)
            if v is not None:
                env[k] = v
        env['LANGUAGE'] = 'C'
        env['LANG'] = 'C'
        env['LC_ALL'] = 'C'
        proc = subprocess.Popen(
            cmd,
            cwd=srcdir,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            env=env)
        out, err = proc.communicate()
        if proc.returncode:
            errmsg = err.decode('ascii', errors='ignore').strip()
            raise GitError("git err; return code %d, error message:\n  '%s'"
                           % (proc.returncode, errmsg))
        return out

    GIT_VERSION = VERSION
    GIT_REVISION = 'Unknown'
    GIT_CYCLE = 0

    try:
        out = _minimal_ext_cmd(['git', 'rev-parse', 'HEAD'], srcdir)
        GIT_REVISION = out.strip().decode('ascii')
    except Exception:
        pass

    try:
        out = _minimal_ext_cmd(['git', 'tag'], srcdir)
        tags = [t.strip() for t in out.strip().decode('ascii').split('\n') if t.strip()]
        if tags:
            GIT_VERSION = tags[-1]
    except Exception:
        pass

    try:
        out = _minimal_ext_cmd(['git', 'describe', '--tags',
                                '--long', '--always'], srcdir)
        desc = out.strip().decode('ascii').split('-')
        if len(desc) >= 3:
            GIT_CYCLE = desc[-2]
            GIT_REVISION = desc[-1]
        elif len(desc) == 1 and desc[0]:
            GIT_CYCLE = 1
            GIT_REVISION = desc[0]
    except Exception:
        pass

    return GIT_VERSION, GIT_REVISION, GIT_CYCLE


# BEFORE importing distutils, remove MANIFEST. distutils doesn't properly
# update it when the contents of directories change.
if os.path.exists('MANIFEST'):
    os.remove('MANIFEST')


def rewrite_setup_cfg(version, gitrevision, release):
    cfg_in = 'setup.cfg.in'
    if not os.path.exists(cfg_in):
        return
    toreplace = dict(locals())
    data = ''.join(open(cfg_in, 'r').readlines()).split('@')
    for k, v in toreplace.items():
        if k in data:
            idx = data.index(k)
            data[idx] = str(v)
    cfg = open('setup.cfg', 'w')
    cfg.write(''.join(data))
    cfg.close()


def get_version_info(srcdir=None):
    global ISRELEASED
    GIT_CYCLE = 0

    git_dir = os.path.join(srcdir or '.', '.git')
    setup_cfg_path = os.path.join(srcdir or '.', 'setup.cfg')

    if os.environ.get('CONDA_BUILD', False):
        FULLVERSION = os.environ.get('PKG_VERSION', VERSION)
        GIT_REVISION = os.environ.get('GIT_DESCRIBE_HASH', '')
        ISRELEASED = True
        rewrite_setup_cfg(FULLVERSION, GIT_REVISION, 'yes')
    elif os.path.exists(git_dir):
        FULLVERSION, GIT_REVISION, GIT_CYCLE = git_version(srcdir)
        ISRELEASED = (str(GIT_CYCLE) == '0')
        if not FULLVERSION:
            FULLVERSION = VERSION
        rewrite_setup_cfg(FULLVERSION, GIT_REVISION,
                          (ISRELEASED and 'yes') or 'no')
    elif os.path.exists(setup_cfg_path):
        setupcfg = configparser.ConfigParser(allow_no_value=True)
        setupcfg.read(setup_cfg_path)
        try:
            FULLVERSION = setupcfg.get(section='metadata', option='version')
        except Exception:
            FULLVERSION = None

        if not FULLVERSION or FULLVERSION == "Unknown":
            FULLVERSION = VERSION

        try:
            GIT_REVISION = setupcfg.get(section='metadata', option='gitrevision')
        except Exception:
            GIT_REVISION = ""

        if GIT_REVISION is None:
            GIT_REVISION = ""

        return FULLVERSION, GIT_REVISION
    else:
        dname = os.getcwd().split(os.sep)[-1]
        m = re.search(r'[0-9.]+', dname)
        if m:
            FULLVERSION = m.group()
            GIT_REVISION = ''
        else:
            FULLVERSION = VERSION
            GIT_REVISION = "Unknown"

    if not FULLVERSION:
        FULLVERSION = VERSION

    if not ISRELEASED and str(GIT_CYCLE) != '0':
        FULLVERSION += '.dev' + str(GIT_CYCLE)

    return FULLVERSION, GIT_REVISION


class sdist_checked(sdist):
    """ check submodules on sdist to prevent incomplete tarballs """
    def run(self):
        sdist.run(self)

def setup_package():
    src_path = os.path.dirname(os.path.abspath(__file__))
    sys.path.insert(0, src_path)

    # Rewrite the version file everytime
    VERSION, gitrevision = get_version_info(src_path)

    metadata = dict(
        name='pysteamtables',
        packages=['pysteamtables', 'pysteamtables.tests'],
        cmake_languages=('C'),
        version=VERSION,
        maintainer="",
        maintainer_email="",
        description=DOCLINES[0],
        long_description=open('README.md').read(),
        url='',
        author='',
        license='GPL-2.0',
        classifiers=[_f for _f in CLASSIFIERS.split('\n') if _f],
        platforms=["Windows", "Linux", "Mac OS-X"],
        cmdclass={"sdist": sdist_checked},
        cmake_args=['-DPYSTEAMTABLES_VERSION:STRING=' + VERSION,
                    '-DGIT_REVISION:STRING=' + gitrevision,
                    '-DISRELEASE:STRING=' + str(ISRELEASED),
                    '-DFULL_VERSION=' + VERSION + '.git' + gitrevision[:7]],
        zip_safe=False,
        install_requires=[],
        python_requires=">=3.8"
    )

    try:
        setup(**metadata)
    finally:
        del sys.path[0]
    return


if __name__ == '__main__':
    setup_package()