#!/usr/bin/env python
from PyQt4 import Qt
import sys
import oscillo
import threading
import numpy
class CsvReader(oscillo.SensorReader):
    def __init__(self,file):
        self.init_cache()
        self.timers=[]
        f = open(file,  "r")
        for line in f.readlines():
            if not line == "t;raw ax;raw ay;raw az\n":
                [t,ax,ay,az] = line[:-1].split(";")
                tfloat = float(t.replace(',','.'))
                self.timers.append(threading.Timer(tfloat, self.update_cache, args=[tfloat,int(ax),int(ay),int(az)]))
        f.close()
        for timer in self.timers:
            timer.start()

    def update_cache(self,t,ax,ay,az):
        self.cache_t.append(t)
        self.cache_x.append(ax-128)
        self.cache_y.append(ay-128)
        self.cache_z.append(az-128)

    def init_cache(self):
        self.cache_t=[]
        self.cache_x=[]
        self.cache_y=[]
        self.cache_z=[]

    def get_new_data(self):
        data = {'t':numpy.array(self.cache_t),
                'x':numpy.array(self.cache_x),
                'y':numpy.array(self.cache_y),
                'z':numpy.array(self.cache_z)}

        self.init_cache()
        return data

    def terminate(self):
        pass

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
    reader.terminate()
    sys.exit(ret)
