#!/usr/bin/env python
from PyQt4 import Qt
import sys
from sensor import wiimote
import oscillo

if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)

    oscillo.Conf.REFRESH_RATE = 100
    oscillo.Conf.Y_SPAN = 256
    oscillo.Conf.X_SPAN = 2

    reader = wiimote.WiimoteReader()
    demo = oscillo.Scope(reader)
    demo.resize(500, 300)
    demo.show()

    ret = app.exec_()
    reader.terminate()
    sys.exit(ret)
