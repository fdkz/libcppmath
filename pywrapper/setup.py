from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import sys


libraries = []
extra_compile_args = []
extra_link_args = []


ext_gltext = Extension(
    "cppmath",
    language="c++",
    #pyrex_cplus=1,
    sources=["cppmath.pyx",],
    include_dirs=["../include"],
    library_dirs=[],
    libraries=libraries,
    #define_macros=[("WIN32", "1")],
    extra_link_args=extra_link_args,
    extra_compile_args=extra_compile_args,
    #depends=[],
    )


setup(
    name         = "cppmath",
    version      = "0.1",
    license      = "MIT",
    description  = "python wrappers for some 3d cpp math objects",
    long_description  = """nah""",
    keywords     = "",
    author       = "",
    author_email = "",
    url          = "",
    classifiers  = [],
    ext_modules  = [ext_gltext],
    cmdclass={
        'build_ext': build_ext,
        }
    )


# the setup.py "clean" command is not working completely for cython. we'll do it ourselves.

if "clean" in sys.argv:
    import os
    import shutil
    for p in ["cppmath.cpp"]:
        print "deleting", p
        try:    os.remove(p)
        except: pass
    shutil.rmtree("build", True)

