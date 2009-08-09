from sensor import ScopeReader

from sensor.wiimote import *

import time

class WiimoteReader(ScopeReader):
    def __init__(self):
        self.wiimote = giveMeAWiimote()
        self.wiimote.set_msg_callback(self.callback)
        self.time_init = time.time()
        self.data = [None,None,None,None]

    def refresh(self):
        return self.data

    def callback(self,msgs):
        for msg in msgs:
            if msg[0]==cwiid.MESG_ACC:
                self.data= [time.time()-self.time_init,msg[1][0],msg[1][1],msg[1][2]]
