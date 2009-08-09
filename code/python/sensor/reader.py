#import numpy

class ScopeReader:
    def __init__(self):
        self.count = 0
    def refresh(self):
        #v = numpy.sin(self.count/10.*numpy.pi) * 0.1
        #a = (numpy.array(self.count), numpy.array(-1+v), numpy.array(v), numpy.array(1+v))
        #self.count += 1
        #return a
        return (None,None,None,None)
