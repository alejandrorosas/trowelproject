#!/usr/bin/env python
from PyQt4 import Qt
import sys
from sensor import zizi
import oscillo

class ZiziReader(oscillo.SensorReader):
    def __init__(self):
        self.zizi = zizi.Zizi("/dev/ttyUSB0")
        self.zizi.start()
        
    def get_new_data(self):
        return self.zizi.get_data()
        
    def terminate(self):
        self.zizi.terminate()

if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)
    
    oscillo.Conf.REFRESH_RATE = 100
    oscillo.Conf.Y_SPAN = 5
    oscillo.Conf.X_SPAN = 2
    
    reader = ZiziReader()
    demo = oscillo.Scope(reader)
    demo.resize(500, 300)
    demo.show()
    
    ret = app.exec_()
    reader.terminate()
    sys.exit(ret)
