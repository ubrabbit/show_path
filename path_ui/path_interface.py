#coding: utf-8

"""
/*
 *  Copyright (C) 2011-2017 ubrabbit
 *  Author: ubrabbit <ubrabbit@gmail.com>
 *  Date: 2017-01-22 12:49:09
 *
 */

"""

import sys
import copy
import math
import re


from PyQt4 import QtCore, QtGui
from functools import partial

import console_log
import help_window

import c_path


VERSION=0.01


#显示中文
QtCore.QTextCodec.setCodecForTr(QtCore.QTextCodec.codecForName("utf8"))
QtCore.QTextCodec.setCodecForCStrings(QtCore.QTextCodec.codecForName("utf8"))
QtCore.QTextCodec.setCodecForLocale(QtCore.QTextCodec.codecForName("system"))



Grid_Color_List=[
        "",
        "#aaffff","#aaaaff","#00aa7f","#00aaff",
        "#aa55ff","#555500","#55557f","#00007f",
        "#aa0000",
]


Color_Enter="#00ff7f"
Color_Exit="#00ff00"
Color_Block="#000000"

Special_Colors=[Color_Enter,Color_Exit,Color_Block]


DEFINE_REDIRECT_STDOUT = 1


class CMyApp(QtGui.QMainWindow):

        def __init__(self):
                QtGui.QMainWindow.__init__(self)

                self.m_Grid=CGrid(self)
                self.m_Interface=CInterface(self)

                self.setWindowTitle(self.tr("寻路显示器"))

                screen = QtGui.QDesktopWidget().screenGeometry() 
                self.resize(screen.width(), screen.height()) 


        def __del__(self):
                console_log.Console_Free()


        def ImportTxtFile(self):
                sFilter="Text Files(*.txt)"
                fileName=QtGui.QFileDialog.getOpenFileName(self,self.tr("打开文件"),QtCore.QString(),self.tr(sFilter))
                fileName=unicode(fileName, 'utf8', 'ignore').encode("gbk")

                print "fileName is ",fileName

                if not fileName:
                        return

                with open(fileName,"r") as fobj:
                        sCode=fobj.read()
                        iRow,iCol,dPos=self.m_Grid.LoadTxt(sCode)

                        self.m_Interface.ResetTableGridSize(iRow,iCol)
                        for pos,iColor in dPos.iteritems():
                            row,col=pos
                            assert(self.m_Interface.SetTableColor(row,col,iColor)==1)


        def ExportToTxtFile(self):
                if len(self.m_Interface.m_ColorInputs) != len(self.m_Interface.m_ColorRatios):
                        QtGui.QMessageBox.information(self,"输入错误",self.tr("颜色权重未完全输入"))
                        return

                sFilter="Text Files(*.txt)"
                fileName=QtGui.QFileDialog.getOpenFileName(self,self.tr("打开文件"),QtCore.QString(),self.tr(sFilter))
                fileName=unicode(fileName, 'utf8', 'ignore').encode("gbk")

                print "fileName is ",fileName

                if not fileName:
                        return
                if fileName=="config":
                        QtGui.QMessageBox.information(self,"输入错误",self.tr("不能存入配置文件"))
                        return

                with open(fileName,"w+") as fobj:
                        sCode=self.m_Grid.SaveTxt()
                        fobj.write(sCode)
                QtGui.QMessageBox.information(self,"保存",self.tr("保存成功！"))


        def StartPainting(self):
                if len(self.m_Interface.m_ColorInputs) != len(self.m_Interface.m_ColorRatios):
                        QtGui.QMessageBox.information(self,"输入错误",self.tr("颜色权重未完全输入"))
                        return

                c_path.Reset_Path()

                dInfo=self.m_Grid.GetPainterInfo()
                self.m_CurWindow=CPainterPath(dInfo)
                self.m_CurWindow.show()


        def OpenHelpWindow(self):
                #需要保存为成员变量，不然会被释放
                self.m_CurWindow=help_window.Open()
                self.m_CurWindow.show()


        def OpenAboutWindow(self):
                QtGui.QMessageBox.about(self,self.tr("关于"),self.tr(self.GetAbout()))


        def GetAbout(self):
                sCode=\
"""
作者：ubrabbit
版本：%s
这是一个可以达成可视化寻路显示目的的界面程序
"""%self.GetVersion()
                return sCode


        def GetVersion(self):
                return "v%s"%VERSION


class CInterface(object):

        def __init__(self, parent):
                super( CInterface, self).__init__()
                self.m_Parent = parent

                self.m_ColorRatios={}
                self.m_ColorInputs={}
                self.m_CurSelectColor=0
                self.m_RowCnt=0
                self.m_ColCnt=0

                self.m_EntranceColor=0
                self.m_ExitColor=0
                self.m_BlockColor=0

                try:
                        self.Layout_Init()
                        self.InitGraphLayout()
                        self.StatusBar_Init()
                except Exception,e:
                        console_log.Console_Free()
                        import traceback
                        traceback.print_exc()
                        exit(-1)


        def Layout_Init(self):
                centralWidget=QtGui.QWidget(self.m_Parent)
                self.m_Parent.setCentralWidget(centralWidget)

                self.tab_Widget=QtGui.QTabWidget(self.m_Parent)
                self.mainLayout=QtGui.QHBoxLayout(centralWidget)


                self.listWidget=QtGui.QListWidget()
                self.showGraphStack=QtGui.QStackedWidget()

                self.listWidget.insertItem(0,self.m_Parent.tr("说明"))
                self.listWidget.insertItem(1,self.m_Parent.tr("画图"))
                self.m_Parent.connect(self.listWidget,QtCore.SIGNAL("currentRowChanged(int)"),self.showGraphStack,QtCore.SLOT("setCurrentIndex(int)"))

                self.mainLayout.addWidget(self.listWidget)
                self.mainLayout.addWidget(self.tab_Widget)
                #设置布局空间的比例
                self.mainLayout.setStretchFactor(self.listWidget,1)
                self.mainLayout.setStretchFactor(self.tab_Widget,29)


        def InitGraphLayout(self):
                if DEFINE_REDIRECT_STDOUT:
                        self.console_Edit,self.log_Edit=console_log.Console_Init(self)
                else:
                        console_Edit=QtGui.QTextEdit()
                        log_Edit=QtGui.QTextEdit()
                        self.console_Edit,self.log_Edit=console_Edit,log_Edit

                graphWidget=QtGui.QWidget(self.m_Parent)
                graphLayout=QtGui.QVBoxLayout(graphWidget)

                tipWidget=QtGui.QWidget(self.m_Parent)
                layout_Vertical=QtGui.QVBoxLayout(tipWidget)
                layout_Horizon=QtGui.QHBoxLayout()
                tips=QtGui.QTextEdit()
                sText=self.m_Parent.GetAbout()
                tips.setText(self.m_Parent.tr(sText))
                tips.setEnabled(False)
                layout_Vertical.addWidget(tips)
                layout_Vertical.addLayout(layout_Horizon)
                layout_Vertical.addStretch(5)

                self.showGraphStack.addWidget(tipWidget)
                self.showGraphStack.addWidget(graphWidget)

                self.tab_Widget.addTab(self.showGraphStack,self.m_Parent.tr("画图"))
                if DEFINE_REDIRECT_STDOUT:
                        self.tab_Widget.addTab(self.console_Edit,self.m_Parent.tr("控制台"))
                        self.tab_Widget.addTab(self.log_Edit,self.m_Parent.tr("日志"))

                layout_Vertical_Top=QtGui.QHBoxLayout()
                layout_Vertical_Bottom=QtGui.QHBoxLayout()

                #self.tableWidget=QtGui.QWidget(self.m_Parent)
                self.tableWidget=QtGui.QTableWidget(self.m_Parent)
                self.tableWidget.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)
                self.tableWidget.cellClicked.connect(partial(self.Table_CellClicked,"table_click_grid"))

                layout_right=QtGui.QVBoxLayout()
                layout_Vertical_Top.addWidget(self.tableWidget)
                layout_Vertical_Top.addLayout(layout_right)
                layout_Vertical_Top.setStretchFactor(self.tableWidget,19)
                layout_Vertical_Top.setStretchFactor(layout_right,1)

                layout_h1=QtGui.QHBoxLayout()
                layout_h2=QtGui.QHBoxLayout()
                layout_h3=QtGui.QHBoxLayout()

                label1=QtGui.QLabel(self.m_Parent.tr("长："))
                label2=QtGui.QLabel(self.m_Parent.tr("宽："))
                label1.setFont(QtGui.QFont('微软雅黑',10))
                label2.setFont(QtGui.QFont('微软雅黑',10))

                combo1=QtGui.QComboBox(self.m_Parent)
                combo2=QtGui.QComboBox(self.m_Parent)

                sizeList=(20,50,100,200,400,500,1000,)
                self.m_RowCnt=self.m_ColCnt=sizeList[0]
                for obj in (combo1,combo2):
                        obj.setCurrentIndex(0)
                        for value in sizeList:
                                obj.addItem(self.m_Parent.tr("%s"%value))
                self.ResetTableGridSize(self.m_RowCnt,self.m_ColCnt)

                self.m_Parent.connect(combo1,QtCore.SIGNAL('activated(QString)'),partial(self.OnComboActivated,"size_select1"))
                self.m_Parent.connect(combo2,QtCore.SIGNAL('activated(QString)'),partial(self.OnComboActivated,"size_select2"))

                layout_h1.addWidget(label1)
                layout_h1.addWidget(combo1)
                layout_h2.addWidget(label2)
                layout_h2.addWidget(combo2)

                layout_right.addLayout(layout_h1)
                layout_right.addLayout(layout_h2)

                label_title=QtGui.QLabel(self.m_Parent.tr("颜色与权重"))

                layout_right.addStretch(1)
                layout_right.addWidget(label_title)

                idx=0
                self.m_CurSelectColor=idx
                sList=Grid_Color_List
                doorLst=Special_Colors[:]
                sList.extend( doorLst )
                for sColor in sList:
                        self.m_ColorRatios[idx]=sColor
                        if sColor in doorLst:
                                if sColor==Color_Enter:
                                        sName="入口"
                                        self.m_EntranceColor=idx
                                elif sColor==Color_Exit:
                                        sName="出口"
                                        self.m_ExitColor=idx
                                elif sColor==Color_Block:
                                        sName="障碍物"
                                        self.m_BlockColor=idx
                                self.m_ColorInputs[idx]=0

                                oLabel=QtGui.QLabel(self.m_Parent.tr("%s"%sName))
                                oLabel.setFont(QtGui.QFont('微软雅黑',10))
                                oLabel.setAlignment(QtCore.Qt.AlignCenter)
                        else:
                                sName="%s"%idx
                                oLabel=QtGui.QLineEdit()
                                oLabel.connect(oLabel,QtCore.SIGNAL("textChanged(QString)"),partial(self.OnLineColorChanged,idx))
                                oLabel.setText(QtCore.QString("0"))

                        oButton=QtGui.QPushButton(self.m_Parent.tr(""))
                        oButton.setStyleSheet('QWidget {background-color:%s}'%sColor)
                        func=partial(self.OnButtonClicked,"Color_Label",idx)
                        oButton.clicked.connect(func)

                        idx+=1

                        layout=QtGui.QHBoxLayout()
                        layout.addWidget(oButton)
                        layout.addWidget(oLabel)
                        layout.setStretchFactor(oButton,2)
                        layout.setStretchFactor(oLabel,1)
                        layout.addStretch(1)

                        layout_right.addLayout(layout)

                layout_right.addStretch(1)
                oLabel1=QtGui.QLabel(self.m_Parent.tr("当前所选颜色："))
                oLabel1.setFont(QtGui.QFont('微软雅黑',10))
                self.curColorLabel=QtGui.QLabel(self.m_Parent.tr(""))
                layout=QtGui.QHBoxLayout()
                layout.addWidget(self.curColorLabel)
                layout.setStretchFactor(self.curColorLabel,1)
                layout.addStretch(1)

                layout_right.addWidget(oLabel1)
                layout_right.addLayout(layout)


                oButton=QtGui.QPushButton(self.m_Parent.tr("批量对鼠标所选区块上色"))
                func=partial(self.OnButtonClicked,"Set_Color")
                oButton.clicked.connect(func)
                layout_right.addWidget(oButton)

                layout_right.addStretch(1)
                oButton3=QtGui.QPushButton(self.m_Parent.tr("开始绘图"))
                oButton3.clicked.connect(partial(self.OnButtonClicked,"Painting"))
                oButton4=QtGui.QPushButton(self.m_Parent.tr("重置"))
                oButton4.clicked.connect(partial(self.OnButtonClicked,"ResetTable"))
                layout_right.addStretch(1)
                layout_right.addWidget(oButton3)
                layout_right.addWidget(oButton4)
                layout_right.setStretchFactor(oButton3,1)
                layout_right.setStretchFactor(oButton4,1)
                layout_right.addStretch(8)

                graphLayout.addLayout(layout_Vertical_Top)
                graphLayout.addLayout(layout_Vertical_Bottom)
                graphLayout.setStretchFactor(layout_Vertical_Top,9)
                graphLayout.setStretchFactor(layout_Vertical_Bottom,1)
                graphLayout.addStretch(1)

        def ResetTableGridSize(self,iTotalRow,iTotalCol):
                self.m_RowCnt=iTotalRow
                self.m_ColCnt=iTotalCol

                self.tableWidget.setColumnCount(iTotalCol)
                self.tableWidget.setRowCount(iTotalRow)

                iRowSize=iColSize=20
                for col in range(iTotalCol):
                        self.tableWidget.setColumnWidth(col,iColSize)
                for row in range(iTotalRow):
                        self.tableWidget.setRowHeight(row,iRowSize)

                self.m_Parent.m_Grid.ResetAllGrid(iTotalRow,iTotalCol)

                #self.tableWidget.verticalHeader().setVisible(False)
                #self.tableWidget.horizontalHeader().setVisible(False)


        def Table_CellClicked(self,sFlag,row,column):
                print "Table_CellClicked  ",sFlag,row,column
                self.SetTableColor(row,column,self.m_CurSelectColor)


        def SetTableColor(self,row,column,idx):
                if not self.m_Parent.m_Grid.SetGrid((row,column),idx):
                        QtGui.QMessageBox.information(self.m_Parent,"",self.m_Parent.tr("设置颜色失败"))
                        return 0

                sColor=self.m_ColorRatios[idx]
                #print "选择了颜色： %s "%str(sColor)
                owidget=QtGui.QWidget()
                owidget.setStyleSheet('QWidget {background-color:%s}'%sColor)
                self.tableWidget.setCellWidget(row,column,owidget)
                return 1


        def OnLineColorChanged(self,idx,sText):
                sText=unicode(sText, 'utf8', 'ignore').encode("utf8")

                print "OnLineEditChanged ",idx,sText

                sText=sText.strip(" ")
                sText=sText.strip("\t")
                sText=sText.strip("\n")
                if not sText.isdigit():
                        sCode="必须输入整数， 当前 输入: %s"%(sText)
                        QtGui.QMessageBox.information(self.m_Parent,"",self.m_Parent.tr(sCode))
                        return
                if not idx in self.m_ColorRatios:
                        sCode="严重错误！所选颜色不存在！！！！！  %s"%(sText)
                        QtGui.QMessageBox.critical(self.m_Parent,"",self.m_Parent.tr(sCode))
                        return
                self.m_ColorInputs[idx]=int(sText)


        def OnButtonClicked(self,sFlag,*param):
                print "OnButtonClicked ",sFlag,param
                if sFlag=="Color_Label":
                        idx=int(param[0])
                        if idx==self.m_CurSelectColor:
                                return
                        assert(idx in self.m_ColorRatios)
                        sColor=self.m_ColorRatios[idx]
                        self.m_CurSelectColor=idx

                        self.curColorLabel.setStyleSheet('QWidget {background-color:%s}'%sColor)
                elif sFlag=="Set_Color":
                        #oItems=self.tableWidget.selectedItems()
                        for oIndex in self.tableWidget.selectedIndexes():
                                    row,column=oIndex.row(),oIndex.column()
                                    if not self.SetTableColor(row,column,self.m_CurSelectColor):
                                                break
                elif sFlag=="Painting":
                        self.m_Parent.StartPainting()
                elif sFlag=="ResetTable":
                        self.tableWidget.clear()
                        self.ResetTableGridSize(10,10)


        def OnComboActivated(self,sFlag,sValue):
                sValue=str(sValue)
                print "OnComboActivated",sFlag,sValue
                if sFlag=="size_select1":
                    iRow=int(sValue)
                    self.ResetTableGridSize(iRow,self.m_ColCnt)
                elif sFlag=="size_select2":
                    iCol=int(sValue)
                    self.ResetTableGridSize(self.m_RowCnt,iCol)


        def StatusBar_Init(self):
                menubar=self.m_Parent.menuBar()
                fileMenu=menubar.addMenu(self.m_Parent.tr("文件"))
                helpMenu=menubar.addMenu(self.m_Parent.tr("帮助"))

                self.m_Parent.statusBar()

                #QtGui.QAction是关于菜单栏、工具栏或自定义快捷键动作的抽象。
                exitAction=QtGui.QAction(QtGui.QIcon(""),self.m_Parent.tr("退出"),self.m_Parent)
                #定义快捷键。
                exitAction.setShortcut("Ctrl+Q")
                #当鼠标停留在菜单上时，在状态栏显示该菜单的相关信息。
                exitAction.setStatusTip(self.m_Parent.tr("Ctrl+Q 退出程序"))
                #选定特定的动作，发出触发信号。该信号与QtGui.QApplication部件的quit()方法
                #相关联，这将会终止应用程序。
                exitAction.triggered.connect(QtGui.qApp.quit)

                importAction=QtGui.QAction(QtGui.QIcon(""),self.m_Parent.tr("导入文件"),self.m_Parent)
                importAction.setShortcut("Ctrl+I")
                importAction.setStatusTip(self.m_Parent.tr("Ctrl+I 导入文件程序"))
                importAction.triggered.connect(self.m_Parent.ImportTxtFile)

                exportAction=QtGui.QAction(QtGui.QIcon(""),self.m_Parent.tr("保存文件"),self.m_Parent)
                exportAction.setShortcut("Ctrl+S")
                exportAction.setStatusTip(self.m_Parent.tr("Ctrl+S 保存文件"))
                exportAction.triggered.connect(self.m_Parent.ExportToTxtFile)

                fileMenu.addAction(exitAction)
                fileMenu.addAction(importAction)
                fileMenu.addAction(exportAction)


                helpAction=QtGui.QAction(QtGui.QIcon(""),self.m_Parent.tr("说明"),self.m_Parent)
                helpAction.setShortcut("Ctrl+H")
                helpAction.setStatusTip(self.m_Parent.tr("弹出帮助窗口"))
                helpAction.triggered.connect(self.m_Parent.OpenHelpWindow)

                aboutAction=QtGui.QAction(QtGui.QIcon(""),self.m_Parent.tr("关于"),self.m_Parent)
                aboutAction.setStatusTip(self.m_Parent.tr("程序版本和作者信息"))
                aboutAction.triggered.connect(self.m_Parent.OpenAboutWindow)

                helpMenu.addAction(helpAction)
                helpMenu.addAction(aboutAction)

                toolBar=self.m_Parent.addToolBar(self.m_Parent.tr(""))
                toolBar.addAction(exitAction)
                toolBar.addAction(importAction)                
                toolBar.addAction(exportAction)


class CGrid(object):

        m_ReFindSize=re.compile("\$size\((\d+),(\d+)\)")
        m_ReFineVersion=re.compile("\$version\((.+)\)")

        m_EnterFlag="${Enter}"
        m_ExitFlag="${Exit}"
        m_BlockFlag="${Block}"

        def __init__(self,parent):
                self.m_Parent=parent

                self.m_Row=0
                self.m_Col=0
                self.m_Pos_Entrance=()
                self.m_Pos_Export=()
                self.m_BlockList=[]

                self.m_GridPos={}


        def ResetAllGrid(self,iTotalRow,iTotalCol):
                self.m_Row=iTotalRow
                self.m_Col=iTotalCol

                self.m_Pos_Entrance=()
                self.m_Pos_Export=()

                self.m_GridPos={}
                for pos in [(i,j) for i in range(iTotalRow) for j in range(iTotalCol)]:
                        self.m_GridPos[pos]=0


        def SetGrid(self,pos,idx):
                if not self.ValidSetGrid(pos,idx):
                        return 0
                if pos==self.m_Pos_Entrance:
                        self.m_Pos_Entrance=()
                if pos==self.m_Pos_Export:
                        self.m_Pos_Export=()

                if pos in self.m_BlockList:
                        self.m_BlockList.remove(pos)

                if idx==self.m_Parent.m_Interface.m_EntranceColor:
                        self.m_Pos_Entrance=pos
                if idx==self.m_Parent.m_Interface.m_ExitColor:
                        self.m_Pos_Export=pos

                if idx==self.m_Parent.m_Interface.m_BlockColor:
                        if not pos in self.m_BlockList:
                                self.m_BlockList.append( pos )
                else:
                        self.m_GridPos[pos]=idx
                return 1


        def ValidSetGrid(self,pos,iColor):
                if iColor==self.m_Parent.m_Interface.m_EntranceColor:
                        if self.m_Pos_Entrance and pos!=self.m_Pos_Entrance:
                                return False
                if iColor==self.m_Parent.m_Interface.m_ExitColor:
                        if self.m_Pos_Export and pos!=self.m_Pos_Export:
                                return False
                return True


        def GetPainterInfo(self):
                dInfo={}
                dWeight={}
                for pos,idx in self.m_GridPos.iteritems():
                        weight=self.m_Parent.m_Interface.m_ColorInputs[idx]
                        dWeight[pos]=weight

                dInfo["Size"]=(self.m_Row,self.m_Col)
                dInfo["Color"]=copy.copy(self.m_Parent.m_Interface.m_ColorInputs)
                dInfo["Enter"]=self.m_Pos_Entrance
                dInfo["Exit"]=self.m_Pos_Export
                dInfo["Block"]=self.m_BlockList
                dInfo["Grid"]=copy.copy(self.m_GridPos)
                dInfo["Weight"]=dWeight
                dInfo["EnterColor"]=self.m_Parent.m_Interface.m_EntranceColor
                dInfo["ExitColor"]=self.m_Parent.m_Interface.m_ExitColor

                #print "GetPainterInfo  ",dInfo
                return dInfo


        def SaveTxt(self):
                sFile=\
"""$version(%s)
$size(%s,%s)
%s
"""

                sContent=[]
                for i in xrange(self.m_Row):
                        sList=[]
                        for j in xrange(self.m_Col):
                                pos=(i,j)
                                if pos==self.m_Pos_Entrance:
                                        sKey=self.m_EnterFlag
                                elif pos==self.m_Pos_Export:
                                        sKey=self.m_ExitFlag
                                elif pos in self.m_BlockList:
                                        sKey=self.m_BlockFlag
                                else:
                                        iColor=self.m_GridPos[pos]
                                        sKey="%s"%iColor
                                sList.append(sKey)
                        sContent.append( " ".join(sList) )

                sFile=sFile%(VERSION,self.m_Row,self.m_Col,"\n".join(sContent))

                print "SaveTxt ",sFile
                return sFile


        def LoadTxt(self,sFile):
                sFile=sFile.strip("\t")
                sFile=sFile.strip(" ")
                sCodeLst=sFile.split("\n")
                sHead=""
                while(sHead==""):
                    sHead=sCodeLst.pop(0)

                oGroup=self.m_ReFineVersion.match(sHead)
                if not oGroup or oGroup.group(1).strip(" ")!="%s"%VERSION:
                        QtGui.QMessageBox.information(self.m_Parent,"导入错误",self.m_Parent.tr("文件版本已经不一致"))
                        return 0,0,{}

                sHead=sCodeLst.pop(0)
                oGroup=self.m_ReFindSize.match(sHead)
                iRow,iCol=int(oGroup.group(1)),int(oGroup.group(2))
                dPos={}

                for i in xrange(iRow):
                        sList=sCodeLst.pop(0)
                        sList=sList.split(" ")
                        for j in xrange(iCol):
                                pos=(i,j)
                                if sList:
                                        sKey=sList.pop(0)
                                        if sKey==self.m_EnterFlag:
                                                idx=self.m_Parent.m_Interface.m_EntranceColor
                                        elif sKey==self.m_ExitFlag:
                                                idx=self.m_Parent.m_Interface.m_ExitColor
                                        elif sKey==self.m_BlockFlag:
                                                idx=self.m_Parent.m_Interface.m_BlockColor
                                        else:
                                                idx=int(sKey)
                                        iColor=idx
                                else:
                                        iColor=0
                                dPos[pos]=iColor
                                #assert(self.SetGrid(pos,iColor)==1)
                return iRow,iCol,dPos


class CPainterPath(QtGui.QWidget):

        m_Path_Color="#55ff7f"

        def __init__(self,dInfo,parent=None):
                super(CPainterPath,self).__init__(parent)

                self.m_Row,self.m_Col=dInfo["Size"]
                self.m_Color=dInfo["Color"]
                self.m_Entrance=dInfo["Enter"]
                self.m_Exit=dInfo["Exit"]
                self.m_BlockList=dInfo["Block"]
                self.m_Pos=dInfo["Grid"]
                self.m_Weight=dInfo["Weight"]

                #剩余路径列表
                self.m_PathList=[]
                self.m_PassList_Bak=[]
                #已经经过的路径列表，初始为出口
                self.m_PassList=[]

                self.m_Color_Bak={}

                self.setWindowTitle(self.tr("显示窗口"))

                mainLayout=QtGui.QHBoxLayout(self)
                leftLayout=QtGui.QVBoxLayout()
                rightLayout=QtGui.QVBoxLayout()

                screen = QtGui.QDesktopWidget().screenGeometry() 
                self.m_WindowSize_Col=screen.width()
                self.m_WindowSize_Row=screen.height()
                self.PaintMap()

                bottomLayout=QtGui.QHBoxLayout()
                leftLayout.addWidget(self.mapWidget)
                leftLayout.addLayout(bottomLayout)
                leftLayout.setStretchFactor(self.mapWidget,19)
                leftLayout.setStretchFactor(bottomLayout,1)

                mainLayout.addLayout(leftLayout)
                mainLayout.addLayout(rightLayout)
                mainLayout.setStretchFactor(leftLayout,19)
                mainLayout.setStretchFactor(rightLayout,1)

                sList=[
                ("开始","Paint_Start"),("上一步","Paint_Last"),
                ("下一步","Paint_Next"),("全部执行","Paint_All"),
                ("重新开始","Paint_Restart"),
                ]
                oList=[]
                for sName,sKey in sList:
                        oButton=QtGui.QPushButton(self.tr(sName))
                        func=partial(self.OnButtonClicked,sKey)
                        oButton.clicked.connect(func)
                        oList.append(oButton)
                        setattr(self,"m_Button_%s"%sKey,oButton)
                        if sKey!="Paint_Start":
                                oButton.setEnabled(False)

                rightLayout.addStretch(1)
                for oButton in oList:
                        rightLayout.addWidget(oButton)
                for oButton in oList:
                        rightLayout.setStretchFactor(oButton,1)

                layout_cost=QtGui.QHBoxLayout()
                label1=QtGui.QLabel(self.tr("c层寻路开销："))
                label2=QtGui.QLabel(self.tr("0"))
                label3=QtGui.QLabel(self.tr(" ms"))
                self.costLabel=label2;
                label1.setFont(QtGui.QFont('微软雅黑',10))
                label2.setFont(QtGui.QFont('微软雅黑',10))
                label3.setFont(QtGui.QFont('微软雅黑',10))
                layout_cost.addWidget(label1)
                layout_cost.addWidget(label2)
                layout_cost.addWidget(label3)
                rightLayout.addLayout(layout_cost)

                rightLayout.addStretch(5)

                self.move_all_timer=QtCore.QTimer()
                #设置窗口计时器
                self.move_all_timer.timeout.connect(self.Move_All_Next)

                self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)
                self.resize(screen.width(), screen.height()) 
                self.setFixedSize(screen.width(),screen.height())


        def OnButtonClicked(self,sFlag):
                print "OnButtonClicked ",sFlag
                if sFlag=="Paint_Start":
                        self.Move_Start()
                elif sFlag=="Paint_Last":
                        self.Move_LastStep()
                elif sFlag=="Paint_Next":
                        self.Move_NextStep()
                elif sFlag=="Paint_All":
                        self.Move_All()
                elif sFlag=="Paint_Restart":
                        self.Move_Restart()


        def PaintMap(self):
                self.mapWidget=QtGui.QTableWidget(self)
                self.mapWidget.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)
                self.PaintTables()
                self.RegistMap()


        def PaintTables(self):
                self.mapWidget.setColumnCount(self.m_Row)
                self.mapWidget.setRowCount(self.m_Col)

                iRowSize=max(15,min(20,self.m_WindowSize_Row/self.m_Row))
                iColSize=max(15,min(20,self.m_WindowSize_Col/self.m_Col))
                for col in range(self.m_Col):
                        self.mapWidget.setColumnWidth(col,iColSize)
                for row in range(self.m_Row):
                        self.mapWidget.setRowHeight(row,iRowSize)

                for pos,idx in self.m_Pos.iteritems():
                        row,col=pos
                        sColor=Grid_Color_List[idx]
                        assert(self.SetColor(row,col,sColor)==1)
                        self.m_Color_Bak[pos]=sColor
                for pos in self.m_BlockList:
                        row,col=pos
                        sColor=Color_Block
                        assert(self.SetColor(row,col,sColor)==1)
                        self.m_Color_Bak[pos]=sColor


        def RegistMap(self):

                posList=[]
                for pos,weight in self.m_Weight.iteritems():
                        i,j=pos
                        posList.append( (i,j,weight) )
                print self.m_Col,self.m_Row,self.m_Entrance,self.m_Exit,posList

                try:
                        iret=c_path.Regist_Map(self.m_Col,self.m_Row,self.m_Entrance,self.m_Exit,posList,self.m_BlockList)
                        assert(iret==1)
                except Exception,e:
                        import traceback
                        traceback.print_exc()
                        QtGui.QMessageBox.information(self,"出错",self.tr("注册地图失败！"))


        def SetColor(self,row,col,sColor):
                owidget=QtGui.QWidget()
                owidget.setStyleSheet('QWidget {background-color:%s}'%sColor)
                self.mapWidget.setCellWidget(row,col,owidget)
                return 1


        def Move_Start(self):
                self.m_Start=1
                self.m_Button_Paint_Start.setEnabled(False)
                self.m_Button_Paint_Last.setEnabled(True)
                self.m_Button_Paint_Next.setEnabled(True)
                self.m_Button_Paint_All.setEnabled(True)
                self.m_Button_Paint_Restart.setEnabled(True)

                try:
                        self.move_all_timer.stop()
                        iCost,pTuple=c_path.Path_Start()
                        #print "Move_Start!!! ",iCost,pTuple
                        pList=list(pTuple)
                except:
                        import traceback
                        traceback.print_exc()
                        QtGui.QMessageBox.information(self,"出错",self.tr("寻路失败！"))
                        return

                if not pList:
                        row_enter,col_enter=self.m_Entrance
                        row_exit,col_exit=self.m_Exit
                        if( abs(row_enter-row_exit) + abs(col_enter-col_exit) <=2 ):
                                QtGui.QMessageBox.information(self,"结果",self.tr("入口就在出口旁边"))
                                return
                        QtGui.QMessageBox.information(self,"无结果",self.tr("入口到出口无可达路径"))
                        return
                self.costLabel.setText("%s"%iCost )

                while (pList):
                        i,j=pList.pop(0),pList.pop(0)
                        self.m_PathList.append( (i,j) )
                self.m_PathList.reverse()


        def Move_Restart(self):
                if not getattr(self,"m_Start",0):
                        return
                self.m_Button_Paint_Start.setEnabled(True)
                self.m_Button_Paint_Last.setEnabled(False)
                self.m_Button_Paint_Next.setEnabled(False)
                self.m_Button_Paint_All.setEnabled(False)
                self.m_Button_Paint_Restart.setEnabled(False)
                self.move_all_timer.stop()

                self.m_PassList=self.m_PassList_Bak[:]
                del self.m_Start
                self.PaintTables()


        def Move_LastStep(self):
                if not getattr(self,"m_Start",0):
                    return

                if len(self.m_PassList)<=0:
                        print "It has last"
                        return 0
                row,col=self.m_PassList.pop(-1)
                self.m_PathList.insert(0,(row,col))

                sColor=self.m_Color_Bak.get( (row,col), "" )
                self.SetColor(row,col,sColor)
                return 1


        def Move_NextStep(self):
                if not getattr(self,"m_Start",0):
                        return 0

                if not self.m_PathList:
                        print "no pass list"
                        return 0
                row,col=self.m_PathList.pop(0)
                self.m_PassList.append( (row,col) )
                self.SetColor(row,col,self.m_Path_Color)
                return 1


        def Move_All(self):
                import time
                if not getattr(self,"m_Start",0):
                        return
                self.m_Button_Paint_Start.setEnabled(False)
                self.m_Button_Paint_Last.setEnabled(False)
                self.m_Button_Paint_Next.setEnabled(False)
                self.m_Button_Paint_All.setEnabled(False)
                self.m_Button_Paint_Restart.setEnabled(True)

                self.Move_All_Next()


        def Move_All_Next(self):
                self.move_all_timer.stop()
                if( self.Move_NextStep() ):
                        self.move_all_timer.start(100)
      
                self.update()


if __name__ == "__main__":
        app = QtGui.QApplication(sys.argv)

        window=CPainterPath(dInfo)
        window.m_PathList=[ (1,1),(1,2),(1,3),(0,3),(0,2),(0,1) ]

        window.show()
        sys.exit(app.exec_())

