#!/usr/bin/python

from distutils.core import setup, Extension

module1 = Extension('rotary',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    include_dirs = ['/usr/include'],
                    libraries = ['wiringPi'],
                    library_dirs = ['/usr/lib'],
                    sources = ['pyrotary.c'])

setup (name = 'RotaryEncoder',
       version = '1.0',
       description = 'Simple rotary encoder using wiringPy',
       author = 'Martin v. Loewis',
       author_email = 'miroslav.talasek@seznam.cz',
       url = 'wiki.talasek.sk',
       long_description = '''
Simple rotary encoder.
''',
       ext_modules = [module1])
