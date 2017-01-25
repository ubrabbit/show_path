#-*- coding:utf-8 -*-
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

QTextCodec.setCodecForTr(QTextCodec.codecForName("utf-8"))

class StockDialog(QWidget):
	def __init__(self, parent=None):
		super(StockDialog, self).__init__(parent)
		self.setWindowTitle(self.tr("利用QPainter绘制各种图形"))

		mainSplitter = QSplitter(Qt.Horizontal)
		mainSplitter.setOpaqueResize(True)

		frame = QFrame(mainSplitter)
		mainLayout = QGridLayout(frame)
		mainLayout.setMargin(10)
		mainLayout.setSpacing(6)

		label1=QLabel(self.tr("形状："))
		label2=QLabel(self.tr("画笔线宽："))
		label3=QLabel(self.tr("画笔颜色："))
		label4=QLabel(self.tr("画笔风格："))
		label5=QLabel(self.tr("画笔顶端："))
		label6=QLabel(self.tr("画笔连接点："))
		label7=QLabel(self.tr("画刷风格："))
		label8=QLabel(self.tr("画刷颜色："))

		self.shapeComboBox = QComboBox()
		self.shapeComboBox.addItem(self.tr("Line"),"Line")
		self.shapeComboBox.addItem(self.tr("Rectangle"),"Rectangle")
		self.shapeComboBox.addItem(self.tr("Ellipse"),"Ellipse")
		self.widthSpinBox = QSpinBox()
		self.widthSpinBox.setRange(0,20)
		self.penColorFrame = QFrame()
		self.penColorFrame.setAutoFillBackground(True)
		self.penColorFrame.setPalette(QPalette(Qt.blue))
		self.penColorPushButton = QPushButton(self.tr("更改"))
		self.penStyleComboBox = QComboBox()
		self.penStyleComboBox.addItem(self.tr("Solid"),Qt.SolidLine)
		self.penCapComboBox = QComboBox()
		self.penCapComboBox.addItem(self.tr("Flat"),Qt.FlatCap)
		self.penJoinComboBox = QComboBox()
		self.penJoinComboBox.addItem(self.tr("Miter"),Qt.MiterJoin)
		self.brushStyleComboBox = QComboBox()
		self.brushStyleComboBox.addItem(self.tr("Linear Gradient"),Qt.LinearGradientPattern)
		self.brushColorFrame = QFrame()
		self.brushColorFrame.setAutoFillBackground(True)
		self.brushColorFrame.setPalette(QPalette(Qt.green))
		self.brushColorPushButton = QPushButton(self.tr("更改"))

		labelCol=0
		contentCol=1

		#建立布局
		mainLayout.addWidget(label1,1,labelCol)
		mainLayout.addWidget(self.shapeComboBox,1,contentCol)
		mainLayout.addWidget(label2,2,labelCol)
		mainLayout.addWidget(self.widthSpinBox,2,contentCol)
		mainLayout.addWidget(label3,4,labelCol)
		mainLayout.addWidget(self.penColorFrame,4,contentCol)
		mainLayout.addWidget(self.penColorPushButton,4,3)
		mainLayout.addWidget(label4,6,labelCol)
		mainLayout.addWidget(self.penStyleComboBox,6,contentCol)
		mainLayout.addWidget(label5,8,labelCol)
		mainLayout.addWidget(self.penCapComboBox,8,contentCol)
		mainLayout.addWidget(label6,10,labelCol)
		mainLayout.addWidget(self.penJoinComboBox,10,contentCol)
		mainLayout.addWidget(label7,12,labelCol)
		mainLayout.addWidget(self.brushStyleComboBox,12,contentCol)
		mainLayout.addWidget(label8,14,labelCol)
		mainLayout.addWidget(self.brushColorFrame,14,contentCol)
		mainLayout.addWidget(self.brushColorPushButton,14,3)
		mainSplitter1 = QSplitter(Qt.Horizontal)
		mainSplitter1.setOpaqueResize(True)

		stack1 = QStackedWidget()
		stack1.setFrameStyle(QFrame.Panel|QFrame.Raised)
		self.area = PaintArea()
		stack1.addWidget(self.area)
		frame1 = QFrame(mainSplitter1)
		mainLayout1 = QVBoxLayout(frame1)
		mainLayout1.setMargin(10)
		mainLayout1.setSpacing(6)
		mainLayout1.addWidget(stack1)

		layout = QGridLayout(self)
		layout.addWidget(mainSplitter1,0,0)
		layout.addWidget(mainSplitter,0,1)
		self.setLayout(layout)

		#信号和槽函数
		self.connect(self.shapeComboBox,SIGNAL("activated(int)"),self.slotShape)
		self.connect(self.widthSpinBox,SIGNAL("valueChanged(int)"),self.slotPenWidth)
		self.connect(self.penColorPushButton,SIGNAL("clicked()"),self.slotPenColor)
		self.connect(self.penStyleComboBox,SIGNAL("activated(int)"),self.slotPenStyle)
		self.connect(self.penCapComboBox,SIGNAL("activated(int)"),self.slotPenCap)
		self.connect(self.penJoinComboBox,SIGNAL("activated(int)"),self.slotPenJoin)
		self.connect(self.brushStyleComboBox,SIGNAL("activated(int)"),self.slotBrush)
		self.connect(self.brushColorPushButton,SIGNAL("clicked()"),self.slotBrushColor)

		self.slotShape(self.shapeComboBox.currentIndex())
		self.slotPenWidth(self.widthSpinBox.value())
		self.slotBrush(self.brushStyleComboBox.currentIndex())

	def slotShape(self,value):
		shape =  self.area.Shape[value]
		self.area.setShape(shape)

	def slotPenWidth(self,value):
		color = self.penColorFrame.palette().color(QPalette.Window)
		style = Qt.PenStyle((self.penStyleComboBox.itemData(self.penStyleComboBox.currentIndex(),Qt.UserRole).toInt())[0])
		cap = Qt.PenCapStyle((self.penCapComboBox.itemData(self.penCapComboBox.currentIndex(),Qt.UserRole).toInt())[0])
		join = Qt.PenJoinStyle((self.penJoinComboBox.itemData(self.penJoinComboBox.currentIndex(),Qt.UserRole).toInt())[0])
		self.area.setPen(QPen(color,value,style,cap,join))

	def slotPenStyle(self,value):
		self.slotPenWidth(value)

	def slotPenCap(self,value):
		self.slotPenWidth(value)

	def slotPenJoin(self,value):
		self.slotPenWidth(value)

	def slotPenColor(self):
		color = QColorDialog.getColor(Qt.blue)
		self.penColorFrame.setPalette(QPalette(color))
		self.area.setPen(QPen(color))

	def slotBrushColor(self):
		color = QColorDialog.getColor(Qt.blue)
		self.brushColorFrame.setPalette(QPalette(color))
		self.slotBrush(self.brushStyleComboBox.currentIndex())

	def slotBrush(self,value):
		color = self.brushColorFrame.palette().color(QPalette.Window)
		style = Qt.BrushStyle((self.brushStyleComboBox.itemData(value,Qt.UserRole).toInt())[0])

		if(style == Qt.LinearGradientPattern):
			linearGradient = QLinearGradient(0,0,400,400)
			linearGradient.setColorAt(0.0,Qt.white)
			linearGradient.setColorAt(0.2,color)
			linearGradient.setColorAt(1.0,Qt.black)
			self.area.setBrush(linearGradient)
		elif(style == Qt.ConicalGradientPattern):
			conicalGradient = QConicalGradient(200,200,30)
			conicalGradient.setColorAt(0.0,Qt.white)
			conicalGradient.setColorAt(0.2,color)
			conicalGradient.setColorAt(1.0,Qt.black)
			self.area.setBrush(conicalGradient)
		elif(style == Qt.TexturePattern):
			self.area.setBrush(QBrush(QPixmap("image/cheese.jpg")))
		else:
			self.area.setBrush(QBrush(color,style))


class PaintArea(QWidget):
	def __init__(self):
		super(PaintArea,self).__init__()
		self.Shape = ["Line","Rectangle"]
		self.setPalette(QPalette(Qt.white))
		self.setAutoFillBackground(True)
		self.setMinimumSize(400,400)
		self.pen = QPen()
		self.brush = QBrush()

	def setShape(self,s):
		self.shape = s
		self.update()

	def setPen(self,p):
		self.pen = p
		self.update()

	def setBrush(self,b):
		self.brush = b
		self.update()

	def paintEvent(self,QPaintEvent):
		print "paintEvent"
		p = QPainter(self)
		p.setPen(self.pen)
		p.setBrush(self.brush)

		rect = QRect(50,100,300,200)
		points = [QPoint(150,100),QPoint(300,150),QPoint(350,250),QPoint(100,300)]
		startAngle = 30 * 16
		spanAngle = 120 * 16

		path = QPainterPath();
		path.addRect(150,150,100,100)
		path.moveTo(100,100)
		path.cubicTo(300,100,200,200,300,300)
		path.cubicTo(100,300,200,200,100,100)

		if self.shape == "Line":
			p.drawLine(rect.topLeft(),rect.bottomRight())
		elif self.shape == "Rectangle":
			p.drawRect(rect)
		elif self.shape == "RoundRect":
			p.drawRoundRect(rect)
		elif self.shape == "Ellipse":
			p.drawEllipse(rect)
		elif self.shape == "Polygon":
			p.drawPolygon(points,4)
		elif self.shape == "Polyline":
			p.drawPolyline(points,4)
		elif self.shape == "Points":
			p.drawPoints(points,4)
		elif self.shape == "Arc":
			p.drawArc(rect,startAngle,spanAngle)
		elif self.shape == "Path":
			p.drawPath(path)
		elif self.shape == "Text":
			p.drawText(rect,Qt.AlignCenter,tr("Hello Qt!"))
		elif self.shape == "Pixmap":
			p.drawPixmap(150,150,QPixmap("image/butterfly.png"))

if __name__=='__main__':

	app = QApplication(sys.argv)
	form = StockDialog()
	form.show()
	app.exec_()
