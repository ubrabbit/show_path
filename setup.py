#coding: utf-8

"""
打包配置文件

编译时报错 No such file or directory: 'numpy-atlas.dll'
去 C:\Python27\Lib\site-packages\numpy\core 目录下找到复制过来即可
"""

from distutils.core import setup
#import py2exe
import sys


sys.path.append('path_ui')
sys.path.append('path_code')


#this allows to run it with a simple double click.
#sys.argv.append('py2exe')


def GetAllFiles():
        sList=[
        "main.py","__init__.py",
        "console_log.py","help_window.py","path_interface.py",
        ]
        return sList


py2exe_options = {
        "includes": ["sip"],
        "dll_excludes": ["MSVCP90.dll",],
        "compressed": 1,
        "optimize": 2,
        "ascii": 0,
        #"bundle_files": 1,
        }

from distutils.core import setup, Extension

# define the extension module
module = Extension('c_path', sources=[\
  'c_interface.c','path_code/c_list.c','path_code/c_map.c',
  'path_code/c_container.c','path_code/path_astar.c'\
  ])

# run the setup
setup(ext_modules=[module])

setup(
      name = 'tool',
      version = '1.0',
      description = "pyd for test_path",
      ext_modules = [module],
      )
