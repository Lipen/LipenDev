from tkinter import *
import random
import math
import time

_TITLE = 'shipsim'
_WIDTH = 800
_HEIGHT = 600
_FPS = 100.
_DELAY = round(1000. / _FPS)
_FPS = 1000./_DELAY
print('FPS = {}, DELAY = {}'.format(_FPS, _DELAY))

root = Tk()
root.title(_TITLE)
canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')
label_fps = Label(root, bg='#cccccc', fg='#000000', font='sans 20')


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

	def __add__(self, other):
		if isinstance(other, Vector2):
			return Vector2(self.x+other.x, self.y+other.y)
		else:
			print('type(other) == {}. isinstance(other, Vector2) == {}'.format(type(other), isinstance(other, Vector2)))
			return Vector2(self.x+other, self.y+other)

	def __sub__(self, other):
		if isinstance(other, Vector2):
			return Vector2(self.x-other.x, self.y-other.y)
		else:
			return Vector2(self.x-other, self.y-other)

	def __mul__(self, other):
		if isinstance(other, Vector2):
			return self.x*other.x + self.y*other.y
		else:
			return Vector2(self.x*other, self.y*other)

	def __truediv__(self, other):
		if isinstance(other, Vector2):
			return Vector2(self.x/other.x, self.y/other.y)
		else:
			return Vector2(self.x/other, self.y/other)

	def __pow__(self, other, signed=False):
		if isinstance(other, Vector2):
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

	def __str__(self):
		return '{'+', '.join(map(lambda q: format(q, '.3f').rstrip('0').rstrip('.'), [self.x, self.y]))+'}'

	def addVectors(v1, v2):
		return Vector2(v1.x+v2.x, v1.y+v2.y)


class Point:

	"""Base 2D point class.
	"""

	def __init__(self, x, y):
		self.x, self.y = x, y

	def __iter__(self):
		return iter([self.x, self.y])

	def __str__(self):
		return '('+', '.join(map(lambda q: format(q, '.3f').rstrip('0').rstrip('.'), [self.x, self.y]))+')'

	def addVector(self, v):
		self.x += v.x
		self.y += v.y

	def getDist(p1, p2):
		return math.hypot(p1.x - p2.x, p1.y-p2.y)

	def getAngle(p1, p2):
		# return math.atan2(p1.y-p2.y, p1.x-p2.x)
		return math.atan2(p2.y-p1.y, p2.x-p1.x)


class Polygon:

	"""Base 2D polygon class.
	* 4-point for now.
	"""

	vel = Vector2(0, 0)
	acc = Vector2(0, 0)
	angularVel = 0

	def __init__(self, p1, p2, p3, p4, fill='#5060BB', activefill='#99dd33'):
		self.points = [p1, p2, p3, p4]
		self.p1, self.p2, self.p3, self.p4 = self.points
		self.poly = canv.create_polygon(self.getCoords(), fill=fill, activefill=activefill)

	def getArea(self):
		return 0.5 * abs(sum(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y for i in range(len(self.points))))

	def getCenter(self):
		A = self.getArea()
		return Point(
			1/6/A*sum((self.points[i-1].x+self.points[i].x)*(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y) for i in range(len(self.points))),
			1/6/A*sum((self.points[i-1].y+self.points[i].y)*(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y) for i in range(len(self.points))))

	def setSpeed(self, vx, vy):
		"""Pixels per second
		"""
		self.vel.x = vx
		self.vel.y = vy

	def setAccel(self, ax, ay):
		"""Pixels per second**2
		"""
		self.acc.x = ax
		self.acc.y = ay

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

	def accel(self):
		self.vel += self.acc * (_DELAY/1000.)

	def move(self):
		for p in self.points:
			p.addVector(self.vel * (_DELAY/1000.))

	def rotate(self):
		c = self.getCenter()
		# print('A: {}, Center: {}'.format(self.getArea(), c))
		for p in self.points:
			r = Point.getDist(c, p)
			if self.angularVel != 0.0:
				a = Point.getAngle(c, p) + self.angularVel/_DELAY
				p.x = c.x + r * math.cos(a)
				p.y = c.y + r * math.sin(a)

	def update(self):
		self.accel()
		self.move()
		self.rotate()
		canv.coords(self.poly, self.p1.x, self.p1.y, self.p2.x, self.p2.y, self.p3.x, self.p3.y, self.p4.x, self.p4.y)  # TODO: reduce this line..
		print('A={}, vel={}, acc={}, Center={}'.format(round(self.getArea()), self.vel, self.acc, self.getCenter()))

	def newRandomPolygon():
		p1 = Point(random.uniform(0, _WIDTH), random.uniform(0, _HEIGHT))
		p2 = Point(random.uniform(0, _WIDTH), random.uniform(p1.y, _HEIGHT))
		p3 = Point(random.uniform(max(p1.x, p2.x), _WIDTH), random.uniform(0, _HEIGHT))
		p4 = Point(random.uniform(max(p1.x, p2.x), _WIDTH), random.uniform(p3.y, _HEIGHT))
		return Polygon(p1, p2, p3, p4, fill=getRandomColor())

# def PressedLeft(event=None):
# 	mypoly.setRotateSpeed(-0.1)
# def PressedRight(event=None):
# 	mypoly.setRotateSpeed(0.1)


def clock_yield():
	pt = time.time() - (_DELAY/1000.)
	while 1:
		t = time.time()
		yield time.time() - pt
		pt = t


def tick(event=None):
	mypoly.update()
	label_fps['text'] = round(1./next(clock))


def timer():
	tick()
	root.after(_DELAY, timer)  # repeat


def QuitDestroy(event=None):
	print('Quitting...')
	root.destroy()
	root.quit()
	print('Destroyed.')

###
clock = clock_yield()
mypoly = Polygon(Point(80, 250), Point(400, 270), Point(300, 390), Point(100, 400), fill=getRandomColor())
# mypoly.setSpeed(1, 1)
mypoly.setAccel(0, 1)
root.bind('z', tick)
root.bind('<Escape>', QuitDestroy)
root.bind('<Control-c>', QuitDestroy, '+')
root.bind('<Left>', lambda e: mypoly.setRotateSpeed(-0.1))
root.bind('<Right>', lambda e: mypoly.setRotateSpeed(0.1))
root.bind('<KeyRelease-Left>', lambda e: mypoly.setRotateSpeed(0),)
root.bind('<KeyRelease-Right>', lambda e: mypoly.setRotateSpeed(0), '+')
root.after(_DELAY, timer)  # start
###

canv.pack()
label_fps.place(x=16, y=16, width=64, height=64)
root.mainloop()
