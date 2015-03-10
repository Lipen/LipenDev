from tkinter import *
import random
import math

_TITLE = 'shipsim'
_WIDTH = 800
_HEIGHT = 600
_FPS = 50.
_DELAY = round(1000. / _FPS)
_FPS = 1000./_DELAY
print('FPS = {}, DELAY = {}'.format(_FPS, _DELAY))

root = Tk()
root.title(_TITLE)
canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')


def getRandomColor():
	return '#{:06X}'.format(random.randint(0, 16777215))


def sign(x):
	return 1 if x > 0 else -1 if x < 0 else 0


class Vector2:

	"""Base 2D vector class
	* Implemented basic arithmetics operation:
		- Addiction/Subtraction (with vector or with scalar)
		- Multiplication (simple or scalar product of vectors)
		- Pow (simple or vector product of vectors)
		- Abs (modulo)
	"""

	def __init__(self, x, y):
		self.x, self.y = x, y

	def addVectors(v1, v2):
		return Vector2(v1.x+v2.x, v1.y+v2.y)

	def __mul__(self, other):
		if other is Vector2:
			return self.x*other.x + self.y*other.y
		else:
			return Vector2(self.x*other, self.y*other)

	def __add__(self, other):
		if other is Vector2:
			return Vector2(self.x+other.x, self.y+other.y)
		else:
			return Vector2(self.x+other, self.y+other)

	def __sub__(self, other):
		if other is Vector2:
			return Vector2(self.x-other.x, self.y-other.y)
		else:
			return Vector2(self.x-other, self.y-other)

	def __pow__(self, other, signed=False):
		if other is Vector2:
			t = self.x*other.y - self.y*other.x
			return sign(t) if signed else t
		else:
			return Vector2(self.x**other, self.y**other)

	def __neg__(self):
		return Vector2(-self.x, -self.y)

	def __pos__(self):
		return self

	def __abs__(self):
		return (self.x**2 + self.y**2) ** 0.5


class Point:

	"""Base 2D point class.
	"""

	def __init__(self, x, y):
		self.x, self.y = x, y

	def __iter__(self):
		return iter([self.x, self.y])

	def addVector(self, v):
		self.x += v.x
		self.y += v.y

	def getDist(p1, p2):
		return math.hypot(p1.x - p2.x, p1.y-p2.y)


class Polygon:

	"""Base 2D polygon class.
	* 4-point for now.
	"""

	vel = Vector2(0, 0)
	angularVel = 0
	angle = 0

	def __init__(self, p1, p2, p3, p4, fill='#5060BB', activefill='#99dd33'):
		self.points = [p1, p2, p3, p4]
		self.p1, self.p2, self.p3, self.p4 = self.points
		self.poly = canv.create_polygon(self.getCoords(), fill=fill, activefill=activefill)

	def getArea(self):
		return 0.5 * sum(self.points[i].x*self.points[i+1].y-self.points[i+1]*self.points[i].y for i in range(len(self.points)-1))

	def getCenter(self):
		A = self.getArea()
		return Point(
			1/6/A*sum((self.points[i].x+self.points[i+1].x)*(self.points[i].x*self.points[i+1].y-self.points[i+1]*self.points[i].y) for i in range(len(self.points)-1)),
			1/6/A*sum((self.points[i].y+self.points[i+1].y)*(self.points[i].x*self.points[i+1].y-self.points[i+1]*self.points[i].y) for i in range(len(self.points)-1)))

	def setSpeed(self, vx, vy):
		"""Pixels per second
		"""
		self.vel.x = vx
		self.vel.y = vy

	def setRotateSpeed(self, av):
		"""Radians per second
		"""
		self.angularVel = av

	def getCoords(self):
		"""List of coords of all polygon's points:
			[ p1.x, p1.y, ..., p4.x, p4.y ] #for 2-dim 4-point polygon
		"""
		# coords = []
		# for p in self.points:
		# 	coords.extend(p)
		# return coords
		return [c for p in self.points for c in p]

	def move(self):
		for p in self.points:
			p.addVector(self.vel / _DELAY)

	def rotate(self):
		c = self.getCenter()
		for p in self.points:
			r = Point.getDist(p, c)


	def update(self):
		self.move()
		self.rotate()
		canv.coords(self.poly, self.p1.x, self.p1.y, self.p2.x, self.p2.y, self.p3.x, self.p3.y, self.p4.x, self.p4.y)  # TODO: reduce this line..

	def newRandomPolygon():
		p1 = Point(random.uniform(0, _WIDTH), random.uniform(0, _HEIGHT))
		p2 = Point(random.uniform(0, _WIDTH), random.uniform(p1.y, _HEIGHT))
		p3 = Point(random.uniform(max(p1.x, p2.x), _WIDTH), random.uniform(0, _HEIGHT))
		p4 = Point(random.uniform(max(p1.x, p2.x), _WIDTH), random.uniform(p3.y, _HEIGHT))
		return Polygon(p1, p2, p3, p4, fill=getRandomColor())


def tick(event=None):
	mypoly.update()


def timer():
	tick()
	# print('timer <3')
	#
	root.after(_DELAY, timer)  # repeat


def QuitDestroy(event=None):
	print('<Q> pressed.')
	root.destroy()
	root.quit()
	print('Destroyed.')

###
# mypoly = Polygon.newRandomPolygon()
mypoly = Polygon(Point(100, 400), Point(80, 250), Point(400, 270), Point(300, 390), fill=getRandomColor())
mypoly.setSpeed(1, 1)
root.bind('z', tick)
root.bind('q', QuitDestroy)
root.bind('<Left>', lambda e: mypoly.setRotateSpeed(-1))
root.bind('<Right>', lambda e: mypoly.setRotateSpeed(1))
root.bind('<ButtonRelease-Left>', lambda e: mypoly.setRotateSpeed(0),)
root.bind('<ButtonRelease-Right>', lambda e: mypoly.setRotateSpeed(0), '+')
root.after(_DELAY, timer)  # start
###

canv.pack()
root.mainloop()
