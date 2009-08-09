
import threading
import serial, re

class Zizi(threading.Thread):
	ser_data = ''
	
	t_a = 0
	a_x = 0
	a_y = 0
	a_z = 0
	
	callback = None
	
	def __init__(self, serialPort="/dev/ttyUSB1"):
		threading.Thread.__init__(self)
		
		self.ser = serial.Serial(serialPort, baudrate=115200, timeout=0)
		self.ptrn = re.compile("(?P<t>[- ][0-9]+), (?P<x>[- ][0-9]{3}), (?P<y>[- ][0-9]{3}), (?P<z>[- ][0-9]{3})\n")
		
		self.start()
    
	def run():
		
		while True:
			self.ser_data += self.ser.read()

			while '\n' in self.ser_data:
				ix = self.ser_data.index('\n')+1
				line = self.ser_data[:ix]
				self.ser_data = self.ser_data[ix:]

				m = self.ptrn.match(line)

				if m is not None:
					self.t_a = float(m.group('t'))
					self.a_x = float(m.group('x'))/50
					self.a_y = float(m.group('y'))/50
					self.a_z = float(m.group('z'))/50
					
					if self.callback is not None:
						self.callback((self.t_a, self.a_x, self.a_y, self.a_z))
	
	def set_callback(self, callback):
		self.callback = callback
