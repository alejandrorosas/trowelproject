#!/usr/bin/env python

from visual import *

import cwiid
import math


class Body:
	
	dt = 0.05
	
	def __init__(self):
		self.left1=box(pos=vector(-1,0,0),
           size=(1,3,1),color=color.white)
		self.right1=box(pos=vector(1,0,0),
           size=(1,3,1),color=color.white)

		self.left2=box(pos=vector(-1,3,0),
           size=(1,3,1),color=color.blue)
		self.right2=box(pos=vector(1,3,0),
           size=(1,3,1),color=color.blue)

		self.body=box(pos=vector(0,6,0),
           size=(3,3,2),color=color.red)
		self.head=box(pos=vector(0,8,0),
           size=(1,1,1),color=color.green)

		self.left_arm=box(pos=vector(-2,5,0),
           size=(1,5,1),color=color.yellow)
		self.right_arm=box(pos=vector(2,5,0),
           size=(1,5,1),color=color.yellow)

	def move_body(self, velocity):
		self.body.velocity = velocity
		self.body.pos = self.body.pos + self.body.velocity*self.dt
		self.move_head(velocity)
		self.move_left_arm(velocity)
		self.move_right_arm(velocity)
		self.move_right1(velocity)
		self.move_right2(velocity)
		self.move_left1(velocity)
		self.move_left2(velocity)
		
	def move_head(self, velocity):
		self.head.velocity = velocity
		self.head.pos = self.head.pos + self.head.velocity*self.dt
	
	def move_left_arm(self, velocity):
		self.left_arm.velocity = velocity
		self.left_arm.pos = self.left_arm.pos + self.left_arm.velocity*self.dt
		
	def move_right_arm(self, velocity):
		self.right_arm.velocity = velocity
		self.right_arm.pos = self.right_arm.pos + self.right_arm.velocity*self.dt
		
	def move_right1(self, velocity):
		self.right1.velocity = velocity
		self.right1.pos = self.right1.pos + self.right1.velocity*self.dt	
			
	def move_right2(self, velocity):
		self.right2.velocity = velocity
		self.right2.pos = self.right2.pos + self.right2.velocity*self.dt	
			
	def move_left1(self, velocity):
		self.left1.velocity = velocity
		self.left1.pos = self.left1.pos + self.left1.velocity*self.dt		
		
	def move_left2(self, velocity):
		self.left2.velocity = velocity
		self.left2.pos = self.left2.pos + self.left2.velocity*self.dt
				
	def rotate_left_arm_x(self,angle):
		origin = self.left_arm.pos + (self.left_arm.up * (self.left_arm.height /2 - 0.5))
		self.left_arm.rotate(angle=angle,axis=self.body.axis,origin=origin)
	
	def rotate_right_arm_x(self,angle):
		origin = self.right_arm.pos + (self.right_arm.up * (self.right_arm.height /2 - 0.5))
		self.right_arm.rotate(angle=angle,axis=self.body.axis,origin=origin)
	
	def rotate_left_arm_y(self,angle):
		origin = self.left_arm.pos + (self.left_arm.up * (self.left_arm.height /2 - 0.5))
		self.left_arm.rotate(angle=angle,axis=self.left_arm.up,origin=origin)
	
	def rotate_right_arm_y(self,angle):
		origin = self.right_arm.pos + (self.right_arm.up * (self.right_arm.height /2 - 0.5))
		self.right_arm.rotate(angle=angle,axis=self.right_arm.up,origin=origin)
		
	def rotate_left_arm_z(self,angle):
		axis = cross(self.body.up,self.body.axis)
		origin = self.left_arm.pos + (self.left_arm.up * (self.left_arm.height /2 - 0.5))
		self.left_arm.rotate(angle=angle,axis=axis,origin=origin)
	
	def rotate_right_arm_z(self,angle):
		axis = cross(self.body.up,self.body.axis)
		origin = self.right_arm.pos + (self.right_arm.up * (self.right_arm.height /2 - 0.5))
		self.right_arm.rotate(angle=angle,axis=axis,origin=origin)
	
	def rotate_body_y(self,angle):
		self.body.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		self.head.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		self.left2.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		self.left1.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		self.right2.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		self.right1.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		self.right_arm.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		self.left_arm.rotate(angle=angle,axis=(0,1,0),origin=self.body.pos)
		
	def rotate_head_y(self,angle):
		origin = self.head.pos - (self.head.up * (self.head.height /2 ))
		self.head.rotate(angle=angle,axis=self.body.up,origin=origin)
		
	def rotate_head_x(self,angle):
		origin = self.head.pos - (self.head.up * (self.head.height /2 ))
		self.head.rotate(angle=angle,axis=self.head.axis,origin=origin)
		
	def rotate_head_z(self,angle):
		axis = cross(self.head.up,self.head.axis)
		origin = self.head.pos - (self.head.up * (self.head.height /2 ))
		self.head.rotate(angle=angle,axis=axis,origin=origin)

class MyWiimote (cwiid.Wiimote):
	
	def __init__(self):
		cwiid.Wiimote.__init__(self)
		self.wm_acc_cal = self.get_acc_cal(cwiid.EXT_NONE)
	
	def get_accel_tuple(self):
		a_x = (float(self.state['acc'][cwiid.X]) - float(self.wm_acc_cal[0][cwiid.X]) ) / (float(self.wm_acc_cal[1][cwiid.X])-float(self.wm_acc_cal[0][cwiid.X]))
		a_y = (float(self.state['acc'][cwiid.Y]) - float(self.wm_acc_cal[0][cwiid.Y]) ) / (float(self.wm_acc_cal[1][cwiid.Y])-float(self.wm_acc_cal[0][cwiid.Y]))
		a_z = (float(self.state['acc'][cwiid.Z]) - float(self.wm_acc_cal[0][cwiid.Z]) ) / (float(self.wm_acc_cal[1][cwiid.Z])-float(self.wm_acc_cal[0][cwiid.Z]))
		return (a_x,a_y,a_z)
		
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

print 'Put Wiimote in discoverable mode now (press 1+2)...'
wiimote = MyWiimote()
print 'wiimote connected'

wiimote.rpt_mode = cwiid.RPT_ACC
wm_acc_cal = wiimote.get_acc_cal(cwiid.EXT_NONE)

body = Body()

##wallR = box(pos=(0,0,20), size=(20,20,0.2), color=color.green)
##wallL = box(pos=(0,0,-20), size=(20,20,0.2), color=color.green)

ball1 = sphere(pos=(0,0,0), radius=0.5)
ball2 = sphere(pos=(0,0,20), radius=0.5)
ball3 = sphere(pos=(0,0,-20), radius=0.5)

total_roll = 0
total_pitch = 0
total_yaw = 0

velocity = vector(0,0,2)
while (1==1):
	rate(100)
		  
	#body.rotate_left_arm_x(pi*0.01)
	#body.rotate_left_arm_y(pi*0.01)
	#body.rotate_left_arm_z(pi*0.01)
	
	body.rotate_right_arm_x(wiimote.get_pitch()-total_pitch)
	total_pitch=wiimote.get_pitch()
	body.rotate_right_arm_y(wiimote.get_roll()-total_roll)
	total_roll= wiimote.get_roll()
	#body.rotate_right_arm_z(wiimote.get_yaw()-total_yaw)
	#total_yaw= wiimote.get_yaw()
	
	#body.rotate_head_y(-pi*0.01)
	#body.rotate_head_x(-pi*0.02)
	#body.rotate_head_z(-pi*0.02)
	
	#body.rotate_body_y(pi*0.01)
	#body.move_body(velocity)
	
	if body.body.z > 20:
		body.body.velocity.z = -body.body.velocity.z
	if body.body.z < -20:
		body.body.velocity.z = -body.body.velocity.z

wiimote.close()
