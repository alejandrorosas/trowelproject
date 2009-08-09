
import threading
import serial, re

class Zizi(threading.Thread):
    ser_data = ''
    t_init = None
    
    t_a = 0
    a_x = 0
    a_y = 0
    a_z = 0
    
    callback = None
    
    def __init__(self, serialPort="/dev/ttyUSB1"):
        threading.Thread.__init__(self)
        
        self.ser = serial.Serial(serialPort, baudrate=115200, timeout=1)
        self.ptrn = re.compile("t=(?P<t>[0-9]+), x=(?P<x>-?[0-9]+), y=(?P<y>-?[0-9]+), z=(?P<z>-?[0-9]+)\n")
        
        self.keep_running = True
        
    def run(self):
        print "Zizi Started"
        
        while self.keep_running:
            self.ser_data += self.ser.read()
            while '\n' in self.ser_data:
                ix = self.ser_data.index('\n')+1
                line = self.ser_data[:ix]
                self.ser_data = self.ser_data[ix:]

                m = self.ptrn.match(line)

                if m is not None:
                    self.t_a = float(m.group('t'))/1000
                    if self.t_init is None:
                        self.t_init = self.t_a
                    self.t_a -= self.t_init
                    self.a_x = float(m.group('x'))/50
                    self.a_y = float(m.group('y'))/50
                    self.a_z = float(m.group('z'))/50
                    
                    if self.callback is not None:
                        self.callback({'t':self.t_a, 'x':self.a_x, 'y':self.a_y, 'z':self.a_z})
                else:
                    print "no match"
        print "Zizi Stopped"
    
    def set_callback(self, callback):
        self.callback = callback
    
    def get_data(self):
        return {'t':self.t_a, 'x':self.a_x, 'y':self.a_y, 'z':self.a_z}
        
    def terminate(self):
        self.keep_running = False
        self.ser.close()
