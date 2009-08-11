#!/usr/bin/env python
from PyQt4 import Qt
import sys, time
import oscillo
import threading
import numpy

class CsvReader(oscillo.SensorReader):
    cache_t=[]
    cache_x=[]
    cache_y=[]
    cache_z=[]
    
    def __init__(self,file):
        f = open(file,  "r")
        for line in f.readlines():
            if not line == "t;raw ax;raw ay;raw az\n":
                [t,ax,ay,az] = line[:-1].split(";")
                tfloat = float(t.replace(',','.'))
                self.cache_t.append(tfloat)
                self.cache_x.append(int(ax)-128)
                self.cache_y.append(int(ay)-128)
                self.cache_z.append(int(az)-128)
        f.close()
        
        self.t_init = time.time()

    def get_new_data(self):
        tmax = time.time()-self.t_init
        ix = 0
        for t in self.cache_t:
            if t > tmax:
                ix = self.cache_t.index(t)
                break
        
        data = {'t':numpy.array(self.cache_t[:ix], float),
                'x':numpy.array(self.cache_x[:ix], float),
                'y':numpy.array(self.cache_y[:ix], float),
                'z':numpy.array(self.cache_z[:ix], float)}
        
        self.cache_t = self.cache_t[ix:]
        self.cache_x = self.cache_x[ix:]
        self.cache_y = self.cache_y[ix:]
        self.cache_z = self.cache_z[ix:]
        
        return data

if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)

    oscillo.Conf.REFRESH_RATE = 100
    oscillo.Conf.Y_SPAN = 256
    oscillo.Conf.X_SPAN = 2

    reader =CsvReader("result.csv")
    demo = oscillo.Scope(reader)
    demo.resize(500, 300)
    demo.show()

    ret = app.exec_()
    sys.exit(ret)
