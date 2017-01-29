#coding: utf-8

"""
/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-22 12:47:21
 *
 */

 """
import sys

sys.path.append('path_ui')

from PyQt4 import QtCore, QtGui

import path_interface

import c_path

def start():
	c_path.Init_Path()
	app = QtGui.QApplication([])
	window = path_interface.CMyApp()
	window.show()
	app.installEventFilter(window)
	sys.exit(app.exec_())


if __name__ == "__main__":
        start()
