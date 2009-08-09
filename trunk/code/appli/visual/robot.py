#!/usr/bin/env python

from visual import *

from wiimote import MyWiimote
import math

class Body:
	
	dt = 0.05
	
	def __init__(self):
		#self.left1=box(pos=vector(-1,0,0),
        #   size=(1,3,1),color=color.white)
		#self.right1=box(pos=vector(1,0,0),
        #   size=(1,3,1),color=color.white)

		#self.left2=box(pos=vector(-1,3,0),
        #   size=(1,3,1),color=color.blue)
		#self.right2=box(pos=vector(1,3,0),
        #   size=(1,3,1),color=color.blue)

		self.left_leg=box(pos=vector(-1,3,0),
           size=(1,6,1),color=color.blue)
		self.right_leg=box(pos=vector(1,3,0),
           size=(1,6,1),color=color.blue)

		self.body=box(pos=vector(0,7.5,0),
           size=(3,3,2),color=color.red)
		self.head=box(pos=vector(0,9.5,0),
           size=(1,1,1),color=color.green)

		self.left_arm=box(pos=vector(-2,6.5,0),
           size=(1,5,1),color=color.yellow)
		self.right_arm=box(pos=vector(2,6.5,0),
           size=(1,5,1),color=color.yellow)
		   
		self.next_walk = "left"

	def move_body(self, vector):
		
		self.body.pos = self.body.pos + vector
		self.move_head(vector)
		self.move_left_arm(vector)
		self.move_right_arm(vector)
		#self.move_right1(velocity)
		#self.move_right2(velocity)
		#self.move_left1(velocity)
		#self.move_left2(velocity)
		self.move_right_leg(vector)
		self.move_left_leg(vector)
		
		
	def move_head(self, vector):
		self.head.pos = self.head.pos + vector
	
	def move_left_arm(self, vector):
		self.left_arm.pos = self.left_arm.pos + vector
		
	def move_right_arm(self, vector):
		self.right_arm.pos = self.right_arm.pos + vector
		
#	def move_right1(self, velocity):
#		self.right1.velocity = velocity
#		self.right1.pos = self.right1.pos + self.right1.velocity*self.dt	
			
#	def move_right2(self, velocity):
#		self.right2.velocity = velocity
#		self.right2.pos = self.right2.pos + self.right2.velocity*self.dt	
			
#	def move_left1(self, velocity):
#		self.left1.velocity = velocity
#		self.left1.pos = self.left1.pos + self.left1.velocity*self.dt		

			
	def move_right_leg(self, vector):
		self.right_leg.pos = self.right_leg.pos + vector	
			
	def move_left_leg(self, vector):
		self.left_leg.pos = self.left_leg.pos + vector		

		
#	def move_left2(self, velocity):
#		self.left2.velocity = velocity
#		self.left2.pos = self.left2.pos + self.left2.velocity*self.dt
				
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
	
	def rotate_left_leg_x(self,angle):
		origin = self.left_leg.pos + (self.left_leg.up * (self.left_leg.height /2 - 0.5))
		self.left_leg.rotate(angle=angle,axis=self.body.axis,origin=origin)
		
	def rotate_right_leg_x(self,angle):
		origin = self.right_leg.pos + (self.right_leg.up * (self.right_leg.height /2 - 0.5))
		self.right_leg.rotate(angle=angle,axis=self.body.axis,origin=origin)
		
	def walk(self):
		if self.next_walk == "left":
			self.walk_left()
			self.switch_next_walk()
		elif self.next_walk == "right":
			self.walk_right()
			self.switch_next_walk()
	
	def switch_next_walk(self):
		if self.next_walk == "left":
			self.next_walk = "right"
		elif self.next_walk == "right":
			self.next_walk = "left"
		
	def walk_left(self):
		left_current_angle = 0.0
		deltah_current = 0.0
		deltad_current = 0.0
		while left_current_angle > -pi/6 :
			rate(50)
			
			angle = -pi*0.01
			left_current_angle+=angle
			body.rotate_left_leg_x(angle)
			body.rotate_right_leg_x(-angle)
			
			l=self.left_leg.height
			deltah = l - l * math.cos(left_current_angle)
			deltad = l * math.sin(left_current_angle)
			
			body.move_body(vector(0,deltah_current-deltah,deltad_current-deltad))
			deltah_current=deltah
			deltad_current=deltad
			
			
		angle *= -1
		#deltad_current = 0.0
		while left_current_angle < 0 :
			rate(50)
			
			left_current_angle+=angle
			body.rotate_left_leg_x(angle)
			body.rotate_right_leg_x(-angle)
			
			l=self.left_leg.height
			deltah = l - l * math.cos(left_current_angle)
			deltad = l * math.sin(left_current_angle)
			
			body.move_body(vector(0,deltah_current-deltah,deltad-deltad_current))
			deltah_current=deltah
			deltad_current=deltad
		#body.rotate_right_leg_x(-pi*0.01)
		body.rotate_left_leg_x(-left_current_angle)
		body.rotate_right_leg_x(left_current_angle)

	def walk_right(self):
		left_current_angle = 0.0
		deltah_current = 0.0
		deltad_current = 0.0
		while left_current_angle < pi/6 :
			rate(50)
			angle = pi*0.01
			left_current_angle+=angle
			body.rotate_left_leg_x(angle)
			body.rotate_right_leg_x(-angle)
			
			l=self.left_leg.height
			deltah = l - l * math.cos(left_current_angle)
			deltad = -l * math.sin(left_current_angle)
			
			body.move_body(vector(0,deltah_current-deltah,deltad_current-deltad))
			deltah_current=deltah
			deltad_current=deltad

		angle *= -1
		#deltad_current = 0.0
		while left_current_angle > 0 :
			rate(50)
			
			left_current_angle+=angle
			body.rotate_left_leg_x(angle)
			body.rotate_right_leg_x(-angle)
			
			l=self.left_leg.height
			deltah = l - l * math.cos(left_current_angle)
			deltad = -l * math.sin(left_current_angle)
			
			body.move_body(vector(0,deltah_current-deltah,deltad-deltad_current))
			deltah_current=deltah
			deltad_current=deltad
		#body.rotate_right_leg_x(-pi*0.01)
		body.rotate_left_leg_x(-left_current_angle)
		body.rotate_right_leg_x(left_current_angle)



print 'Put Wiimote in discoverable mode now (press 1+2)...'
wiimote = MyWiimote()
print 'wiimote connected'

body = Body()

ball1 = sphere(pos=(0,0,0), radius=0.5)
ball2 = sphere(pos=(0,0,20), radius=0.5)
ball3 = sphere(pos=(0,0,-20), radius=0.5)

scene.forward=(-1,-1,-1)

#total_roll = 0
#total_pitch = 0
#total_yaw = 0

#velocity = vector(0,0,2)

dir (wiimote)

while true:
	rate(100)
	#print wiimote.get_accel_tuple()[2]
	if wiimote.get_accel_tuple()[2]<0.4:
		body.walk()
		  
	#body.rotate_left_arm_x(pi*0.01)
	#body.rotate_left_arm_y(pi*0.01)
	#body.rotate_left_arm_z(pi*0.01)
	
	#body.rotate_right_arm_x(wiimote.get_pitch()-total_pitch)
	#total_pitch=wiimote.get_pitch()
	#body.rotate_right_arm_y(wiimote.get_roll()-total_roll)
	#total_roll= wiimote.get_roll()
	
	#body.rotate_right_arm_z(wiimote.get_yaw()-total_yaw)
	#total_yaw= wiimote.get_yaw()
	
	#body.rotate_head_y(-pi*0.01)
	#body.rotate_head_x(-pi*0.02)
	#body.rotate_head_z(-pi*0.02)
	
	#body.rotate_body_y(pi*0.01)
	#body.move_body(velocity)
	
	#body.rotate_left_leg_x(pi*0.01)
	#body.rotate_right_leg_x(-pi*0.01)
	
	
	#if body.body.z > 20:
#		body.body.velocity.z = -body.body.velocity.z
	#if body.body.z < -20:
	#	body.body.velocity.z = -body.body.velocity.z



wiimote.close()
