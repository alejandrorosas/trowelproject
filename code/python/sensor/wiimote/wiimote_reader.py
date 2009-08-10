##from sensor import ScopeReader

import oscillo

from sensor.wiimote import *

import time
import numpy

class WiimoteReader(oscillo.SensorReader):
    def __init__(self):
        self.wiimote = giveMeAWiimote()
        self.wiimote.set_msg_callback(self.callback)

        self.time_init = time.time()

        self.init_cache()

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

    def callback(self,msgs):
        for msg in msgs:
            if msg[0]==cwiid.MESG_ACC:
                self.cache_t.append(time.time()-self.time_init)
                self.cache_x.append(msg[1][0]-128)
                self.cache_y.append(msg[1][1]-128)
                self.cache_z.append(msg[1][2]-128)

    def terminate(self):
        self.wiimote.close()
