#!/usr/bin/env python

import math
import sys
from PyQt4 import Qt
import PyQt4.Qwt5 as Qwt
import numpy

REFRESH_RATE = 10
X_SPAN = 10
X_RIGHT_FREE = 2
Y_SPAN = 3
Y_CENTER = 0

class Reader:
    dt = 1./REFRESH_RATE
    t = numpy.array([])
    data = t*0
    count = 0
    
    def __init__(self):
        pass
    
    def update(self):
        
        self.t = numpy.append(self.t, self.count*self.dt)
        self.data = numpy.append(self.data, numpy.random.randn(1))
        self.count += 1
        
        if self.t[-1] > X_SPAN:
            self.t = self.t[-int(X_SPAN/self.dt):]
            self.data = self.data[-int(X_SPAN/self.dt):]
        
        return True

# class Reader


class Scope(Qwt.QwtPlot):
    
    def __init__(self, reader=None, *args):
        Qwt.QwtPlot.__init__(self, *args)
        
        if reader is None:
            self.reader = Reader()
        else:
            self.reader = reader
                
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
        self.curve = Qwt.QwtPlotCurve('a_x')
        self.curve.setPen(Qt.QPen(Qt.Qt.red))
        self.curve.attach(self)
        
        # initialize the data
        self.curve.setData(self.reader.t, self.reader.data)

        # insert a horizontal marker at y = 0
        mY = Qwt.QwtPlotMarker()
        mY.setLineStyle(Qwt.QwtPlotMarker.HLine)
        mY.setYValue(0.0)
        mY.attach(self)
        
        # add a Timer
        timer = Qt.QTimer(self)
        timer.connect(timer,
                      Qt.SIGNAL('timeout()'),
                      self.refreshData)
        timer.start(REFRESH_RATE)
        
        # replot
        self.replot()

    # __init__()
    
    def refreshData(self):
        # update the data
        if self.reader.update():
            #plot it
            self.curve.setData(self.reader.t, self.reader.data)
            
            self.setAxisScale(Qwt.QwtPlot.xBottom, numpy.max(self.reader.t)-X_SPAN+X_RIGHT_FREE, numpy.max(self.reader.t)+X_RIGHT_FREE)
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



