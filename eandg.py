import pygame
from math import *
from random import *
SCREEN_WIDTH, SCREEN_HEIGHT = 600,600
BG_COLOR = 150, 150, 80
pygame.init()
fg = 0.1
damp = 0.0009
fe = 200
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), 0, 32)
clock = pygame.time.Clock()
ms = []
debug = False
frame_rate = 50
critical_factor = 0.08
num = 10
class mass:
	def __init__(self,x,y,vx,vy,radius,charge):
		self.x = x
		if charge =="+":
			self.charge = 1
			self.m = 10
		elif charge =="-":
			self.charge = -1
			self.m = 1
		else:
			print "Error in charge"
		self.y = y
		self.vx = vx
		self.vy = vy
		self.rad=radius
	def draw(self):
		if self.charge ==1:
			pygame.draw.circle(screen,(255,0,0),(int(self.x),int(self.y)),self.rad,0)
		elif self.charge ==-1:
			pygame.draw.circle(screen,(0,0,255),(int(self.x),int(self.y)),self.rad,0)
		else :
			pygame.draw.circle(screen,(0,0,0),(int(self.x),int(self.y)),self.rad,0)
	def app_force(self,other):
		dx = self.x -other.x
		dy = self.y -other.y
		r = pow(dx**2+dy**2,0.5)
		fxs = -(fg*self.m*other.m-self.charge*other.charge*fe)*dx/(r**3*self.m)
		fys = -(fg*self.m*other.m-self.charge*other.charge*fe)*dy/(r**3*self.m)
		fxo	=(fg*self.m*other.m-self.charge*other.charge*fe)*dx/(r**3*other.m)
		fyo=(fg*self.m*other.m-self.charge*other.charge*fe)*dy/(r**3*other.m)
		if r>(10):
			self.vx+=fxs
			self.vy+=fys
			other.vx+=fxo
			other.vy+=fyo
		elif r>5 :
			self.vx+=fxs*critical_factor*r**2
			self.vy+=fys*critical_factor*r**2
			other.vx+=fxo*critical_factor*r**2
			other.vy+=fyo*critical_factor*r**2	
			self.vx -= damp*(self.vx-other.vx)*abs(self.vx)*abs(self.charge-other.charge)*abs(self.x-other.x)
			self.vy -= damp*(self.vy-other.vy)*abs(self.vy)*abs(self.charge-other.charge)*abs(self.y-other.y)
			other.vx -=damp*(other.vx-self.vx)*abs(other.vx)*abs(self.charge-other.charge)*abs(self.x-other.x)
			other.vy -=damp*(other.vy-self.vy)*abs(other.vy)*abs(self.charge-other.charge)*abs(self.y-other.y)
	def update(self):
		self.x +=self.vx
		#print self.vx
		self.y +=self.vy
		if not 1<self.x<SCREEN_WIDTH:
			self.vx=(-self.vx)
		if not 1<self.y<SCREEN_HEIGHT:
			self.vy=(-self.vy)
		self.draw()
if debug == True:
	ms.append(mass(400,300,-1.2,0.9,3,"-"))
	ms.append(mass(200,300,0,0,3,"+"))
	num =2 
	frame_rate=1000
else :
	
	for i in range(0,num/2):
			ms.append(mass(randint(0,SCREEN_WIDTH),randint(0,SCREEN_HEIGHT),0,0,3,"-"))

	for i in range(num/2,num):
			ms.append(mass(randint(0,SCREEN_WIDTH),randint(0,SCREEN_HEIGHT),0,0,3,"+"))

	
while True:
	time_passed = clock.tick(frame_rate)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			exit_game()
# Redraw the background

	px =0
	py=0
	tm =0 
	#if not debug:
	screen.fill(BG_COLOR) 
	for i in range(0,num):
		
		for ci in range(i+1,num):
			try:
				ms[i].app_force(ms[ci])
			except:
				pass
		try:
			px +=ms[i].x*ms[i].m
			py +=ms[i].y*ms[i].m
			tm +=ms[i].m
			ms[i].update()
		except:
			pass
			
	pygame.draw.circle(screen,(0,0,0),(int(px/tm),int(py/tm)),1,0)
	pygame.display.update()
	if pygame.key.get_focused():
		press = pygame.key.get_pressed()
		if press[273]==1:
			for x in xrange(0,num):
				ms[x].y-=1*num/2
		if press[274]==1:
			for x in xrange(0,num):
				ms[x].y+=1*num/2
		if press[275]==1:
			for x in xrange(0,num):
				ms[x].x+=1*num/2			
		if press[276]==1:
			for x in xrange(0,num):
				ms[x].x-=1*num/2
			

	