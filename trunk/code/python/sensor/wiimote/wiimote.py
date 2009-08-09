#!/usr/bin/env python

import cwiid


class MyWiimote (cwiid.Wiimote):

    def __init__(self):
        cwiid.Wiimote.__init__(self)
        self.wm_acc_cal = self.get_acc_cal(cwiid.EXT_NONE)
        self.rpt_mode = cwiid.RPT_ACC

    def set_msg_callback(self,callback):
        self.enable(cwiid.FLAG_MESG_IFC)
        self.mesg_callback=callback

    def get_accel_tuple(self):
        a_x = (float(self.state['acc'][cwiid.X]) - float(self.wm_acc_cal[0][cwiid.X]) ) / (float(self.wm_acc_cal[1][cwiid.X])-float(self.wm_acc_cal[0][cwiid.X]))
        a_y = (float(self.state['acc'][cwiid.Y]) - float(self.wm_acc_cal[0][cwiid.Y]) ) / (float(self.wm_acc_cal[1][cwiid.Y])-float(self.wm_acc_cal[0][cwiid.Y]))
        a_z = (float(self.state['acc'][cwiid.Z]) - float(self.wm_acc_cal[0][cwiid.Z]) ) / (float(self.wm_acc_cal[1][cwiid.Z])-float(self.wm_acc_cal[0][cwiid.Z]))
        return (a_x,a_y,a_z)

    def get_raw_accel_tuple(self):
        return (self.state['acc'][cwiid.X],self.state['acc'][cwiid.Y],self.state['acc'][cwiid.Z])

    def get_roll(self):
        a_x,a_y,a_z = self.get_accel_tuple()
        if a_z==0:
            a_z=0.0000000000000000001

        roll = math.atan(a_x/a_z)
        if a_z <= 0.0:
            if a_x > 0.0:
                factor = 1
            else:
                factor = -1
            roll += math.pi * factor

        roll *= -1
        return roll

    def get_pitch(self):
        a_x,a_y,a_z = self.get_accel_tuple()
        if a_z==0:
            a_z=0.0000000000000000001
        ##return atan(a_y/a_z*math.cos(self.get_roll()))
        return math.atan(a_y/a_z)

    def get_yaw(self):
        a_x,a_y,a_z = self.get_accel_tuple()
        if a_y==0:
            a_y=0.0000000000000000001
        return math.atan(a_x/a_y)

def giveMeAWiimote():
    print 'Put Wiimote in discoverable mode now (press 1+2)...'
    wiimote = MyWiimote()
    print 'wiimote connected'
    return wiimote
