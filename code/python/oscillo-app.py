#!/usr/bin/env python

from oscillo import oscillo
import serial, re, sys
from PyQt4 import Qt

def make(reader):
    demo = oscillo.Scope(reader)
    demo.resize(500, 300)
    demo.show()
    return demo

def main(args):
    print args
    if "wiimote" in args:
        from sensor.wiimote import WiimoteReader
        reader = WiimoteReader()
    elif "serial" in args:
        from sensor.zizi import SerialReader
        reader = SerialReader()
    else:
        print "Usage :\n    python oscillo-app.py wiimote\n    python oscillo-app.py serial\n"
        sys.exit(0)

    app = Qt.QApplication(args)
    demo = make(reader)
    sys.exit(app.exec_())

if __name__ == '__main__':
    main(sys.argv)
