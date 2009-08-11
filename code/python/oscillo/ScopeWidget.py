import sys, numpy, PyQt4.Qwt5 as Qwt, time
from PyQt4 import QtGui, Qt, QtCore
from PyQt4 import QtCore

class SensorReader:
    def get_new_data(self):
        """ returns a tuple containing the new time and samples since last call""" 
        return ([], [])

class DummyReader(SensorReader):
    def __init__(self):
        self.t_i = time.time()
    def get_new_data(self):
        t = time.time()-self.t_i
        return ([t], [numpy.cos(2*numpy.pi*t)])

class Scope(QtGui.QWidget):    
    
    datat = numpy.array([])
    datax = numpy.array([])
    
    def __init__(self, parent=None, reader=None):
        QtGui.QWidget.__init__(self, parent)
        
        # store the reader
        self.reader = reader
        if self.reader is None:
            self.reader = DummyReader()
        
        # create a HBox
        self.scopeHbox = QtGui.QHBoxLayout(self)
        self.scopeHbox.setSizeConstraint(QtGui.QLayout.SetMaximumSize)
        self.scopeHbox.setObjectName("scopeHbox")
        
        # create THE plot
        self.plot = Qwt.QwtPlot(self)
        self.plot.setObjectName("plot")
        self.plot.setTitle("Acceleration")
        # insert it in the HBox
        self.scopeHbox.addWidget(self.plot)
        
        # configure the plot
        self.plot.insertLegend(Qwt.QwtLegend(), Qwt.QwtPlot.RightLegend)
        self.plot.plotLayout().setAlignCanvasToScales(True)
        self.plot.setAutoReplot(True)
        self.plotgrid = Qwt.QwtPlotGrid()
        self.plotgrid.attach(self.plot)
        self.plotgrid.setPen(Qt.QPen(Qt.Qt.black, 0, Qt.Qt.DotLine))
        # set axis titles
        self.plot.setAxisTitle(Qwt.QwtPlot.xBottom, 't (s)')
        self.plot.setAxisTitle(Qwt.QwtPlot.yLeft, 'a (g)')
        
        # insert the curve
        self.curvex = Qwt.QwtPlotCurve('a_x')
        self.curvex.setPen(Qt.QPen(Qt.Qt.red))
        self.curvex.attach(self.plot)
        self.curvex.setData(self.datat, self.datax)
        
        #create a timer
        self.timer = Qt.QTimer(self)
        self.connect(self.timer, Qt.SIGNAL('timeout()'), self.get_new_data)
        self.timer.refreshRate = 10
        self.timer.start(1000/self.timer.refreshRate)
        
        # create the right side panel menu
        self.menu = QtGui.QVBoxLayout()
        self.menu.setObjectName("menu")
        # insert it in the HBox
        self.scopeHbox.addLayout(self.menu)
        
        # add a stretcher
        self.menu.addStretch(1)
        # add a grid
        self.menugrid = QtGui.QGridLayout()
        self.menugrid.setObjectName("menugrid")
        self.menu.addLayout(self.menugrid)
        
        # place different components
        self.timediv = QtGui.QLabel("Time Div.:", self)
        self.menugrid.addWidget(self.timediv, 0, 0, 1, 1)
        self.timediv.setLineWidth(1)
        self.timediv.setMargin(2)
        self.timediv.setObjectName("timediv")
        
        self.timedivslider = Qwt.QwtSlider(self)
        self.menugrid.addWidget(self.timedivslider, 0, 1, 1, 1)
        self.timedivslider.setObjectName("timedivslider")
        self.timedivslider.possibleValues = [0.05, 0.1, 0.2, 0.5, 1, 2]
        self.timedivslider.actualValue = 2
        self.timedivslider.setRange(0, len(self.timedivslider.possibleValues)-1, 1, 1)
        self.connect(self.timedivslider, Qt.SIGNAL("valueChanged(double)"), self.timediv_changed)
        
        self.timedivunit = QtGui.QLabel("s/div", self)
        self.timedivunit.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.timedivunit.setMargin(2)
        self.timedivunit.setObjectName("timedivunit")
        self.menugrid.addWidget(self.timedivunit, 0, 2, 1, 1)
        
        
        self.sigdiv = QtGui.QLabel("Sig. Div.:", self)
        self.menugrid.addWidget(self.sigdiv, 1, 0, 1, 1)
        self.sigdiv.setMargin(2)
        self.sigdiv.setObjectName("sigdiv")
        
        self.sigdivslider = Qwt.QwtSlider(self)
        self.menugrid.addWidget(self.sigdivslider, 1, 1, 1, 1)
        self.sigdivslider.setObjectName("sigdivslider")
        self.sigdivslider.possibleValues = [0.5, 1, 2, 4, 16, 64, 128, 256, 1024]
        self.sigdivslider.actualValue = 4
        self.sigdivslider.setRange(0, len(self.sigdivslider.possibleValues)-1, 1, 1)
        self.connect(self.sigdivslider, Qt.SIGNAL("valueChanged(double)"), self.sigdiv_changed)
        
        self.sigdivunit = QtGui.QLabel("g/div", self)
        self.menugrid.addWidget(self.sigdivunit, 1, 2, 1, 1)
        self.sigdivunit.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.sigdivunit.setMargin(2)
        self.sigdivunit.setObjectName("sigdivunit")
        
        self.sigoff = QtGui.QLabel("Sig. Off.:", self)
        self.menugrid.addWidget(self.sigoff, 2, 0, 1, 1)
        self.sigoff.setMargin(2)
        self.sigoff.setObjectName("sigoff")
        
        self.sigoffslider = Qwt.QwtSlider(self)
        self.menugrid.addWidget(self.sigoffslider, 2, 1, 1, 1)
        self.sigoffslider.setObjectName("sigoffslider")
        self.sigoffslider.possibleValues = range(-5, 6, 1)
        self.sigoffslider.actualValue = 0
        self.sigoffslider.setRange(0, len(self.sigoffslider.possibleValues)-1, 1, 1)
        self.connect(self.sigoffslider, Qt.SIGNAL("valueChanged(double)"), self.sigoff_changed)
        
        self.sigoffunit = QtGui.QLabel("g", self)
        self.menugrid.addWidget(self.sigoffunit, 2, 2, 1, 1)
        self.sigoffunit.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.sigoffunit.setMargin(2)
        self.sigoffunit.setObjectName("sigoffunit")
        
        
        #set the default values
        self.timedivslider.setValue(self.timedivslider.possibleValues.index(2))
        self.sigdivslider.setValue(self.sigdivslider.possibleValues.index(4))
        self.sigoffslider.setValue(self.sigoffslider.possibleValues.index(0))
        
    def timediv_changed(self, value):
        value = int(value)
        timediv = self.timedivslider.possibleValues[value]
        self.timedivslider.actualValue = timediv
        self.timedivunit.setText("%1.1f s/div"%timediv)
        
        # update refresh rate: 50 times per screen
        #self.timer.refreshRate =  50./(10*self.timedivslider.actualValue)
        #self.timer.setInterval(1000/self.timer.refreshRate)
        
        # update axis scales
        if len(self.datat)>0:
            high = self.datat[-1]
        else:
            high = 0
        low = high - self.timedivslider.actualValue*10
        self.plot.setAxisScale(Qwt.QwtPlot.xBottom, low, high)
        
        
    def sigdiv_changed(self, value):
        value = int(value)
        sigdiv = self.sigdivslider.possibleValues[value]
        self.sigdivslider.actualValue = sigdiv
        self.sigdivunit.setText("%1.1f g/div"%sigdiv)
        
        # update sigoff with the same value
        self.sigoff_changed(len(self.sigoffslider.possibleValues)/2)
        
        
    def sigoff_changed(self, value):
        value = int(value)
        sigoff = self.sigoffslider.possibleValues[value] * self.sigdivslider.actualValue
        self.sigoffslider.actualValue = sigoff
        self.sigoffunit.setText("%1.1f g"%sigoff)
        
        # set axis scales
        low = -self.sigoffslider.actualValue - self.sigdivslider.actualValue*4
        high = -self.sigoffslider.actualValue + self.sigdivslider.actualValue*4
        self.plot.setAxisScale(Qwt.QwtPlot.yLeft, low, high)
        
    def get_new_data(self):
        t, a = self.reader.get_new_data()
        
        if len(t) != len(a):
            print "set_new_data lengths don't match"
            m = min(len(t), len(a))
        else:
            m = len(t)
        
        self.datat = numpy.append(self.datat, t[:m])
        self.datax = numpy.append(self.datax, a[:m])
        
        # we keep data since left of the screen minus 2 screens
        t_right = max(self.datat)
        t_left  = t_right-(10*self.timedivslider.actualValue) 
        t_limit = t_left - 2 * (t_right-t_left)
        
        limit_ix = numpy.where(self.datat > t_limit)[0][0]
        self.datat = self.datat[limit_ix:]
        self.datax = self.datax[limit_ix:]
        
        self.curvex.setData(self.datat, self.datax)
        
        # update axis scales
        if len(self.datat)>0:
            high = self.datat[-1]
        else:
            high = 0
        low = high - self.timedivslider.actualValue*10
        self.plot.setAxisScale(Qwt.QwtPlot.xBottom, low, high)
        
        
        
# Admire
if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)
    
    scope = Scope()
    scope.resize(1500, 300)
    scope.show()
    
    sys.exit(app.exec_())
