#!/usr/bin/env python
from sensor import zizi
import sys,time

Z = zizi.Zizi()

def callback(dat):
    print dat

Z.set_callback(callback)

Z.start()

time.sleep(3)

Z.terminate()
