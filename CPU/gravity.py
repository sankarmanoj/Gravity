import pygame
from random import randint
SCREEN_WIDTH, SCREEN_HEIGHT = 600, 600
BG_COLOR = 150, 150, 80
pygame.init()
factor = 8
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), 0, 32)
clock = pygame.time.Clock()
ms = []
num =20

class mass:
	def __init__(self,x,y,vx,vy,mass,radius):
		self.m = mass
		self.x = x
		self.y = y
		self.vx = vx
		self.vy = vy
		self.rad=radius
	def draw(self):
		pygame.draw.circle(screen,(255,255,255),(int(self.x),int(self.y)),self.rad,0)
	def app_force(self,other):
		dx = self.x -other.x
		dy = self.y -other.y
		r = pow(dx**2+dy**2,0.5)
		if r>(other.rad+self.rad):
			self.vx -= other.m*factor*dx/(r**3)
			self.vy -= other.m*factor*dy/(r**3)
			other.vx += self.m*factor*dx/(r**3)
			other.vy += self.m*factor*dy/(r**3)
		else:
		
			new = mass(self.x,self.y,0,0,self.m+other.m,(self.rad+other.rad)*8/10)
			new.x= self.x*self.m + other.x*other.m
			new.y= self.y*self.m + other.y*other.m
			new.x/= new.m
			new.y/=new.m
			new.vx = self.vx*self.m + other.vx*other.m
			new.vy   = self.vy*self.m + other.vy*other.m
			new.vx /=new.m
			new.vy /=new.m
			global ms
			ms.remove(other)
			ms.remove(self)
			ms.append(new)
			num-=1
			
	def update(self):
		self.x +=self.vx
		#print self.vx
		self.y +=self.vy
		self.draw()


for i in range(0,num):
		ms.append(mass(randint(0,SCREEN_WIDTH),randint(0,SCREEN_HEIGHT),0,0,1,3))



while True:
	time_passed = clock.tick(80)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			exit_game()
# Redraw the background

	px =0
	py=0
	tm =0 
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
	pygame.draw.circle(screen,(0,0,0),(int(px/tm),int(py/tm)),4,0)
	pygame.display.update()
	
