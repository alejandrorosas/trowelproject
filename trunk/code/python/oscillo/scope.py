# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'test.ui'
#
# Created: Tue Aug 11 12:59:13 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_Frame(object):
    def setupUi(self, Frame):
        Frame.setObjectName("Frame")
        Frame.resize(652, 351)
        Frame.setFrameShape(QtGui.QFrame.StyledPanel)
        Frame.setFrameShadow(QtGui.QFrame.Raised)
        self.horizontalLayoutWidget = QtGui.QWidget(Frame)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(25, 27, 591, 271))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtGui.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setSizeConstraint(QtGui.QLayout.SetMaximumSize)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.qwtPlot = QwtPlot(self.horizontalLayoutWidget)
        self.qwtPlot.setObjectName("qwtPlot")
        self.horizontalLayout.addWidget(self.qwtPlot)
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        spacerItem = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem)
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.label = QtGui.QLabel(self.horizontalLayoutWidget)
        self.label.setLineWidth(1)
        self.label.setMargin(2)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.Slider = QwtSlider(self.horizontalLayoutWidget)
        self.Slider.setObjectName("Slider")
        self.gridLayout.addWidget(self.Slider, 0, 1, 1, 1)
        self.lcdNumber_2 = QtGui.QLCDNumber(self.horizontalLayoutWidget)
        self.lcdNumber_2.setObjectName("lcdNumber_2")
        self.gridLayout.addWidget(self.lcdNumber_2, 0, 2, 1, 1)
        self.label_4 = QtGui.QLabel(self.horizontalLayoutWidget)
        self.label_4.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_4.setMargin(2)
        self.label_4.setObjectName("label_4")
        self.gridLayout.addWidget(self.label_4, 0, 3, 1, 1)
        self.label_2 = QtGui.QLabel(self.horizontalLayoutWidget)
        self.label_2.setMargin(2)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)
        self.Slider_2 = QwtSlider(self.horizontalLayoutWidget)
        self.Slider_2.setObjectName("Slider_2")
        self.gridLayout.addWidget(self.Slider_2, 1, 1, 1, 1)
        self.lcdNumber_3 = QtGui.QLCDNumber(self.horizontalLayoutWidget)
        self.lcdNumber_3.setObjectName("lcdNumber_3")
        self.gridLayout.addWidget(self.lcdNumber_3, 1, 2, 1, 1)
        self.label_5 = QtGui.QLabel(self.horizontalLayoutWidget)
        self.label_5.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_5.setMargin(2)
        self.label_5.setObjectName("label_5")
        self.gridLayout.addWidget(self.label_5, 1, 3, 1, 1)
        self.label_3 = QtGui.QLabel(self.horizontalLayoutWidget)
        self.label_3.setMargin(2)
        self.label_3.setObjectName("label_3")
        self.gridLayout.addWidget(self.label_3, 2, 0, 1, 1)
        self.Slider_3 = QwtSlider(self.horizontalLayoutWidget)
        self.Slider_3.setObjectName("Slider_3")
        self.gridLayout.addWidget(self.Slider_3, 2, 1, 1, 1)
        self.lcdNumber = QtGui.QLCDNumber(self.horizontalLayoutWidget)
        self.lcdNumber.setObjectName("lcdNumber")
        self.gridLayout.addWidget(self.lcdNumber, 2, 2, 1, 1)
        self.label_6 = QtGui.QLabel(self.horizontalLayoutWidget)
        self.label_6.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_6.setMargin(2)
        self.label_6.setObjectName("label_6")
        self.gridLayout.addWidget(self.label_6, 2, 3, 1, 1)
        self.verticalLayout.addLayout(self.gridLayout)
        self.horizontalLayout.addLayout(self.verticalLayout)

        self.retranslateUi(Frame)
        QtCore.QMetaObject.connectSlotsByName(Frame)

    def retranslateUi(self, Frame):
        Frame.setWindowTitle(QtGui.QApplication.translate("Frame", "Frame", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Frame", "TimeDiv", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("Frame", "s/div", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Frame", "SigDiv", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("Frame", "g/div", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("Frame", "SigOff", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("Frame", "g", None, QtGui.QApplication.UnicodeUTF8))

from PyQt4.Qwt5 import QwtPlot, QwtSlider
