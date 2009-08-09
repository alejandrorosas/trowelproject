#!/usr/bin/env python

import math
import sys
from PyQt4 import Qt
import PyQt4.Qwt5 as Qwt
import numpy

REFRESH_RATE = 10
X_SPAN = 10
Y_SPAN = 3
Y_CENTER = 0

class ScopeReader:
    def __init__(self):
        self.count = 0
    def refresh(self):
        v = numpy.sin(self.count/10.*numpy.pi) * 0.1
        a = (numpy.array(self.count), numpy.array(-1+v), numpy.array(v), numpy.array(1+v))
        self.count += 1
        return a

# class Reader

class ScopeData:
    def __init__(self, reader=None):
        self.t   = numpy.array([])
        self.a_x = numpy.array([])
        self.a_y = numpy.array([])
        self.a_z = numpy.array([])
        self.magn = numpy.array([])
        
        if reader is None:
            self.reader = ScopeReader()
        else:
            self.reader = reader
        
    def refresh(self):
        (t, x, y, z) = self.reader.refresh()
        self.t   = numpy.append(self.t, t)
        self.a_x = numpy.append(self.a_x, x)
        self.a_y = numpy.append(self.a_y, y)
        self.a_z = numpy.append(self.a_z, z)
        
        self.magn = numpy.append(self.magn, numpy.sqrt(numpy.square(x)+numpy.square(y)+numpy.square(z)))
    

class Scope(Qwt.QwtPlot):
    
    def __init__(self, reader=None, *args):
        Qwt.QwtPlot.__init__(self, *args)
        
        self.data = ScopeData(reader)
        
        # make a QwtPlot widget
        self.insertLegend(Qwt.QwtLegend(), Qwt.QwtPlot.RightLegend)

        # a variation on the C++ example
        self.plotLayout().setAlignCanvasToScales(True)
        grid = Qwt.QwtPlotGrid()
        grid.attach(self)
        grid.setPen(Qt.QPen(Qt.Qt.black, 0, Qt.Qt.DotLine))
        
        # set axis titles
        self.setAxisTitle(Qwt.QwtPlot.xBottom, 't (s)')
        self.setAxisTitle(Qwt.QwtPlot.yLeft, 'a (g)')
        
        # set axis scales
        self.setAxisScale(Qwt.QwtPlot.xBottom, 0, X_SPAN)
        self.setAxisScale(Qwt.QwtPlot.yLeft, Y_CENTER-(Y_SPAN/2.), Y_CENTER+(Y_SPAN/2.))
        
        # insert a few curves
        self.a_x = Qwt.QwtPlotCurve('a_x')
        self.a_x.setPen(Qt.QPen(Qt.Qt.red))
        self.a_x.attach(self)
        self.a_y = Qwt.QwtPlotCurve('a_y')
        self.a_y.setPen(Qt.QPen(Qt.Qt.blue))
        self.a_y.attach(self)
        self.a_z = Qwt.QwtPlotCurve('a_z')
        self.a_z.setPen(Qt.QPen(Qt.Qt.green))
        self.a_z.attach(self)
        
        # magn
        self.magn = Qwt.QwtPlotCurve('magn')
        self.magn.setPen(Qt.QPen(Qt.Qt.black))
        self.magn.attach(self)
        
        # insert a horizontal marker at y = 0
        mY = Qwt.QwtPlotMarker()
        mY.setLineStyle(Qwt.QwtPlotMarker.HLine)
        mY.setYValue(0.0)
        mY.attach(self)
        
        # add a Timer
        timer = Qt.QTimer(self)
        timer.connect(timer,
                      Qt.SIGNAL('timeout()'),
                      self.refresh)
        timer.start(REFRESH_RATE)
        
        # replot
        self.replot()

    # __init__()
    
    def refresh(self):
        # update the data
        self.data.refresh()
        
        # plot the data
        self.a_x.setData(self.data.t, self.data.a_x)
        self.a_y.setData(self.data.t, self.data.a_y)
        self.a_z.setData(self.data.t, self.data.a_z)
        
        self.magn.setData(self.data.t, self.data.magn)
        
        # update the X scale
        if len(self.data.t) == 0:
            max = 0
        else:
            max = numpy.max(self.data.t)
        
        if max > X_SPAN:
            self.setAxisScale(Qwt.QwtPlot.xBottom, max-X_SPAN, max)
        self.replot()

# class Plot


def make():
    demo = Scope()
    demo.resize(500, 300)
    demo.show()
    return demo

# make()


def main(args):
    app = Qt.QApplication(args)
    demo = make()
    sys.exit(app.exec_())

# main()


# Admire
if __name__ == '__main__':
    main(sys.argv)

# Local Variables: ***
# mode: python ***
# End: ***



