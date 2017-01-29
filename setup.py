#coding: utf-8

"""
"""

from distutils.core import setup, Extension
import sys

module = Extension('c_path', sources=[\
  'c_interface.c','c_list.c','c_map.c','c_hash.c','c_heap.c',\
  'c_container.c','algorithm/path_astar.c',\
])

setup(
      name = 'tool',
      version = '1.0',
      description = "pyd for test_path",
      ext_modules = [module],
      )
