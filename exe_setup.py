#coding: utf-8

"""
/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-30 02:02:48
 *
 */

SET VS90COMNTOOLS=%VS110COMNTOOLS%
python exe_setup.py
 """

from distutils.core import setup
import py2exe
import sys

sys.path.append('path_ui')
sys.argv.append('py2exe')


#打包配置文件
def GetAllFiles():
        sList=[
        "main.py","init.py",
        "path_ui/__init__.py",
        "path_ui/console_log.py","path_ui/help_window.py","path_ui/path_interface.py",
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

setup(
      name = 'show_path',
      version = '1.0',
      windows = GetAllFiles(),
      zipfile = None,
      options = {'py2exe': py2exe_options}
      )
