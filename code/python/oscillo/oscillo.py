#!/usr/bin/env python

import math
import sys
from PyQt4 import Qt
import PyQt4.Qwt5 as Qwt
import numpy
import time

class Conf:
    REFRESH_RATE = 100
    X_SPAN = 10
    Y_SPAN = 10
    Y_CENTER = 0

class SensorReader:
    new_data = {'t':numpy.array([]),
                'x':numpy.array([]),
                'y':numpy.array([]),
                'z':numpy.array([]) }
    
    def get_new_data(self):
        return self.new_data

class DefaultReader(SensorReader):
    def __init__(self):
        self.count = 0
    
    def get_new_data(self):
        t = self.count * 1.0/Conf.REFRESH_RATE
        x = 2*numpy.cos(2*numpy.pi*5*t) + numpy.random.randn()
        y = numpy.sin(2*numpy.pi*2*t)
        k = t-int(t)
        if k<0.5:
            z = 0
        else:
            z = 1
        self.count += 1
        return {'t':t, 'x':x, 'y':y, 'z':z}

# class Reader

class AccelData:
    data = {'t':numpy.array([]),
            'x':numpy.array([]),
            'y':numpy.array([]),
            'z':numpy.array([]) }
    
    def __init__(self, reader):
        # store the reader
        self.reader = reader
        
    def refresh(self):
        # add data from reader
        new_data = self.reader.get_new_data()
        #~ print "new data=", new_data
        for k in self.data.keys():
            self.data[k] = numpy.append(self.data[k], new_data[k])
        
        #~ print "AccelData.data:", self.data
        # remove old data
        old = numpy.max(self.data['t'])-Conf.X_SPAN
        mask = self.data['t'] > old
        
        for k in self.data.keys():
            self.data[k] = self.data[k][mask]

class TimeScope(Qwt.QwtPlot):
    
    def __init__(self, accel_data, *args):
        Qwt.QwtPlot.__init__(self, *args)
        
         # register the data reader
        self.accel_data = accel_data
        
        # set the plot title
        self.setTitle('Time Measure');
        
        # make a QwtPlot widget
        self.insertLegend(Qwt.QwtLegend(), Qwt.QwtPlot.RightLegend)

        self.plotLayout().setAlignCanvasToScales(True)
        grid = Qwt.QwtPlotGrid()
        grid.attach(self)
        grid.setPen(Qt.QPen(Qt.Qt.black, 0, Qt.Qt.DotLine))
        
        # set axis titles
        self.setAxisTitle(Qwt.QwtPlot.xBottom, 't (s)')
        self.setAxisTitle(Qwt.QwtPlot.yLeft, 'a (g)')
        
        # set axis scales
        self.setAxisScale(Qwt.QwtPlot.xBottom, 0, Conf.X_SPAN)
        self.setAxisScale(Qwt.QwtPlot.yLeft, Conf.Y_CENTER-(Conf.Y_SPAN/2.), Conf.Y_CENTER+(Conf.Y_SPAN/2.))
        
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
        
        # add a Timer
        timer = Qt.QTimer(self)
        timer.connect(timer, Qt.SIGNAL('timeout()'), self.refresh)
        timer.start(1000./Conf.REFRESH_RATE)
        
        # replot
        self.replot()

    # __init__()
    
    def refresh(self):
        # update the data
        self.accel_data.refresh()
        
        # plot the data
        self.a_x.setData(self.accel_data.data['t'], self.accel_data.data['x'])
        self.a_y.setData(self.accel_data.data['t'], self.accel_data.data['y'])
        self.a_z.setData(self.accel_data.data['t'], self.accel_data.data['z'])
        
        # update the X scale
        if len(self.accel_data.data['t']) == 0:
            max = 0
        else:
            max = numpy.max(self.accel_data.data['t'])
        
        if max > Conf.X_SPAN:
            self.setAxisScale(Qwt.QwtPlot.xBottom, max-Conf.X_SPAN, max)
        
        self.replot()

# class TimePlot


class FreqScope(Qwt.QwtPlot):
    
    """
    Power spectrum display widget
    """
    def __init__(self, accel_data, *args):
        apply(Qwt.QwtPlot.__init__, (self,) + args)
        
         # register the data reader
        self.accel_data = accel_data

        self.setTitle('Power spectrum');
        self.setCanvasBackground(Qt.Qt.white)

        # grid 
        self.grid = Qwt.QwtPlotGrid()
        self.grid.enableXMin(True)
        self.grid.setMajPen(Qt.QPen(Qt.Qt.gray, 0, Qt.Qt.SolidLine));
        self.grid.attach(self)

        # axes
        self.setAxisTitle(Qwt.QwtPlot.xBottom, 'Frequency [Hz]');
        self.setAxisTitle(Qwt.QwtPlot.yLeft, 'Power [dB]');
        self.setAxisMaxMajor(Qwt.QwtPlot.xBottom, 10);
        self.setAxisMaxMinor(Qwt.QwtPlot.xBottom, 0);
        self.setAxisMaxMajor(Qwt.QwtPlot.yLeft, 10);
        self.setAxisMaxMinor(Qwt.QwtPlot.yLeft, 0);

        # legend
        self.insertLegend(Qwt.QwtLegend(), Qwt.QwtPlot.RightLegend)
        
        # curves
        self.a_x = Qwt.QwtPlotCurve('a_x')
        self.a_x.setPen(Qt.QPen(Qt.Qt.red,2))
        self.a_x.setYAxis(Qwt.QwtPlot.yLeft)
        self.a_x.attach(self)
        
        self.a_y = Qwt.QwtPlotCurve('a_y')
        self.a_y.setPen(Qt.QPen(Qt.Qt.blue,2))
        self.a_y.setYAxis(Qwt.QwtPlot.yLeft)
        self.a_y.attach(self)
        
        self.a_z = Qwt.QwtPlotCurve('a_z')
        self.a_z.setPen(Qt.QPen(Qt.Qt.green,2))
        self.a_z.setYAxis(Qwt.QwtPlot.yLeft)
        self.a_z.attach(self)
        
        # add a Timer
        timer = Qt.QTimer(self)
        timer.connect(timer, Qt.SIGNAL('timeout()'), self.refresh)
        timer.start(10*1000./Conf.REFRESH_RATE)
        
    def refresh(self):
        # TODO
        sig = self.accel_data.data['x']
        t = self.accel_data.data['t']
        dt = numpy.average(t[1:]-t[:-1])
        
        freq = numpy.fft.fftfreq(len(sig), dt)
        freq = numpy.concatenate((freq[len(freq)/2:], freq[:len(freq)/2-1]))
        sigF = abs(numpy.fft.fft(sig))
        sigF /= numpy.max(sigF)
        sigF = numpy.concatenate((sigF[len(sigF)/2:], sigF[:len(sigF)/2-1]))
        self.a_x.setData(freq, sigF)
        
        sig = self.accel_data.data['y']
        sigF = abs(numpy.fft.fft(sig))
        sigF /= numpy.max(sigF)
        sigF = numpy.concatenate((sigF[len(sigF)/2:], sigF[:len(sigF)/2-1]))
        self.a_y.setData(freq, sigF)
        
        sig = self.accel_data.data['z']
        sigF = abs(numpy.fft.fft(sig))
        sigF /= numpy.max(sigF)
        sigF = numpy.concatenate((sigF[len(sigF)/2:], sigF[:len(sigF)/2-1]))
        self.a_z.setData(freq, sigF)
        
        
        self.replot()
        
# class FreqPlot


class Scope(Qt.QWidget):
    def __init__(self, reader=None, *args):
        Qt.QMainWindow.__init__(self)
        self.setWindowTitle("Accelerometer Oscilloscope")
        self.resize(640, 480)
        
         # register the reader
        if reader is None:
            self.reader = DefaultReader()
        else:
            self.reader = reader
        
        accel_data = AccelData(self.reader)
        
        vbox = Qt.QVBoxLayout()
        timescope = TimeScope(accel_data)
        vbox.addWidget(timescope)
        
        freqscope = FreqScope(accel_data)
        vbox.addWidget(freqscope)
        
        self.setLayout(vbox)

# Admire
if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)
    
    demo = Scope()
    demo.show()
    
    sys.exit(app.exec_())

# Local Variables: ***
# mode: python ***
# End: ***



