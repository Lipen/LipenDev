from tkinter import *
import random
import math
import time

# TODO: (_DELAY/1000.) ===> "_dt" from clock
# TODO: Force -+-> angular momentum

_TITLE = 'shipsim'
_WIDTH = 800
_HEIGHT = 600
_FPS = 25.
_DELAY = round(1000. / _FPS)
_FPS = 1000./_DELAY
print('FPS = {}, DELAY = {}'.format(_FPS, _DELAY))
_WATERLINE = 250
_SHIPWIDTH = 5
_MASS = 2000000  # kg
_DENSITY = 50  # 113; 22.6
_GRAVACCEL = 9.8
_FRICTION = 1.0

root = Tk()
root.title(_TITLE)
canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')
label_fps = Label(root, bg='#cccccc', fg='#000000', font='sans 20')


def getRandomColor():
	return '#{:06X}'.format(random.randint(0, 16777215))


def sign(x):
	return 1 if x > 0 else -1 if x < 0 else 0


def fround(f, precision=3):
	return format(f, '.{}f'.format(precision)).rstrip('0').rstrip('.')


def crossSegmentWithHorizontalLine(A, B, c):
	if B.y != A.y and ((A.y < c and B.y > c) or (A.y > c and B.y < c)):
		return Point((A.x*(B.y-c) + B.x*(c-A.y))/(B.y-A.y), c)


class Vector2:

	"""Base 2D vector class
	* Implemented basic arithmetics operation:
		- Addiction/Subtraction (with vector or with scalar)
		- Multiplication (simple or scalar product of vectors)
		- Pow (simple or vector product of vectors)
		- Abs (modulo==length)
	"""

	def __init__(self, x, y):
		self.x, self.y = x, y

	def __add__(self, other):
		if isinstance(other, Vector2):
			return Vector2(self.x+other.x, self.y+other.y)
		else:
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
		return '{'+', '.join(map(fround, [self.x, self.y]))+'}'

	def newFromPoints(p1, p2):
		return Vector2(p2.x-p1.x, p2.y-p1.y)

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
		return '('+', '.join(map(fround, [self.x, self.y]))+')'

	def addVector(self, v):
		self.x += v.x
		self.y += v.y

	def getDist(p1, p2):
		return math.hypot(p1.x - p2.x, p1.y-p2.y)

	def getAngle(p1, p2):
		return math.atan2(p2.y-p1.y, p2.x-p1.x)


class Polygon:

	"""Base 2D polygon class.
	"""

	vel = Vector2(0, 0)
	acc = Vector2(0, 0)
	angularVel = 0
	angularAcc = 0

	def __init__(self, points, fill='#5060BB', activefill='#99dd33', spawn=False):
		self.points = list(points)  # is it necessary?..
		if spawn:
			self.poly = canv.create_polygon(self.getCoords(), fill=fill, activefill=activefill)

	def getArea(self):
		return 0.5 * abs(sum(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y for i in range(len(self.points))))

	def getCenter(self):
		A = self.getArea()
		return Point(
			1/6/A*sum((self.points[i-1].x+self.points[i].x)*(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y) for i in range(len(self.points))),
			1/6/A*sum((self.points[i-1].y+self.points[i].y)*(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y) for i in range(len(self.points))))

	# def getInertiaMoment():
	# 	return _MASS*()/12

	def setSpeed(self, vx, vy):
		"""Pixels per second
		"""
		self.vel.x = vx
		self.vel.y = vy

	def setAccel(self, ax=None, ay=None):
		"""Pixels per second^2
		"""
		if ax is not None:
			self.acc.x = ax
		if ay is not None:
			self.acc.y = ay

	def setAngularSpeed(self, av):
		"""Radians per second
		"""
		self.angularVel = av

	def setAngularAccel(self, aa):
		"""Radians per second^2
		"""
		self.angularAcc = aa

	def getCoords(self):
		"""List of coords of all polygon's points
		"""
		return [c for p in self.points for c in p]

	def accel(self):
		self.vel += self.acc * (_DELAY/1000.)

	def move(self):
		for p in self.points:
			p.addVector(self.vel * (_DELAY/1000.))

	def rotate(self):
		self.angularVel += self.angularAcc * (_DELAY/1000.)
		c = self.getCenter()
		for p in self.points:
			r = Point.getDist(c, p)
			if self.angularVel != 0:
				a = Point.getAngle(c, p) + self.angularVel/_DELAY
				p.x = c.x + r * math.cos(a)
				p.y = c.y + r * math.sin(a)

	def update(self):
		self.accel()
		self.move()
		self.rotate()
		canv.coords(self.poly, *self.getCoords())

		print('v.x={:.1f},v.y={:.1f},a.x={:.1f},a.y={:.1f},aV={:.1f},aA={:.1f}'.format(self.vel.x, self.vel.y, self.acc.x, self.acc.y, self.angularVel, self.angularAcc))


class World:

	"""Physics simulation
	"""

	def gravForce(mass):
		return Vector2(0, mass * _GRAVACCEL)

	def archForce(density, volume):
		return Vector2(0, -density * _GRAVACCEL * volume)

	def fricForce(density, vel: Vector2, coef, poly: Polygon):
		VD = 0
		for i in range(len(mypoly.points)):
			for j in range(len(mypoly.points)):
				VA = mypoly.points[i]
				VC = mypoly.points[j]
				VV = vel
				VBy = (VA.x*VV.x+VA.y*VV.y-VC.x*VV.x+VC.y*VV.x**2/VV.y)/(VV.x**2/VV.y+VV.y)
				VBx = (VBy*VV.x+VC.x*VV.y-VC.y*VV.x)/VV.y
				VDtmp = math.hypot(VA.x-VBx, VA.y-VBy)
				if VDtmp > VD:
					VD = VDtmp
		VS = VD * _SHIPWIDTH
		v = abs(vel)
		f = 0.5*density*v**2*coef*VS
		return -Vector2(f*vel.x/v, f*vel.y/v)


def clock_yield():
	pt = 0
	while 1:
		t = time.time()
		yield time.time() - pt
		pt = t


def tick(event=None):
	mypoly.update()
	label_fps['text'] = round(1./next(clock))

	if max(mypoly.getCoords()[1::2]) > _WATERLINE:
		# print('water!')
		if min(mypoly.getCoords()[1::2]) > _WATERLINE:
			Area = mypoly.getArea()
			Volume = Area * _SHIPWIDTH
			forceGrav = World.gravForce(_MASS)
			forceArch = World.archForce(_DENSITY, Volume)
			forceFric = World.fricForce(_DENSITY, mypoly.vel, _FRICTION, mypoly)
			F = forceGrav + forceArch + forceFric
			acc = F / _MASS
			print('VolUnderWater={:.1f}, acc.x={:.1f}, acc.y={:.1f}'.format(Volume, acc.x, acc.y))
			mypoly.setAccel(acc.x, acc.y)
		else:
			intersections = []
			for i in range(len(mypoly.points)):
				A = mypoly.points[i-1]
				B = mypoly.points[i]
				t = crossSegmentWithHorizontalLine(A, B, _WATERLINE)
				if t is not None:
					intersections.append([t, A, B])
			if len(intersections) == 2:
				# TODO: Refactor this mess :c
				A, B, C, D = intersections[0][1:]+intersections[1][1:]
				if A.y < B.y:
					b = True
					W = []
					for i in range(len(mypoly.points)):
						X = mypoly.points[i]
						if X is D:
							b = False
							W.append(intersections[1][0])
						if X is A:
							b = True
							W.append(intersections[0][0])
						if b and X.y > _WATERLINE:
							W.append(X)
				else:
					b = True
					W = []
					for i in range(len(mypoly.points)):
						X = mypoly.points[i]
						if X is B:
							b = False
							W.append(intersections[0][0])
						if X is C:
							b = True
							W.append(intersections[1][0])
						if b and X.y > _WATERLINE:
							W.append(X)
				under = Polygon(W)  # Polygon under water
				Area = under.getArea()
				Volume = Area * _SHIPWIDTH

				forceGrav = World.gravForce(_MASS)
				forceArch = World.archForce(_DENSITY, Volume)
				forceFric = World.fricForce(_DENSITY, mypoly.vel, _FRICTION, under)
				F = forceArch + forceFric

				Cu = under.getCenter()
				C0 = mypoly.getCenter()
				s = Vector2.newFromPoints(Cu, C0)  # directing

				Fll = F * s / abs(s)  # projection # ForceLine-Length
				Flx = Fll / math.sqrt((s.y/s.x)**2+1)  # relative
				Fly = Flx * s.y/s.x
//				Fl = Vector2(Flx, Fly)  #ForceLine
				# Fl = Vector2(Cu.x+Flx, Cu.y+Fly)  #ForceLine

				Fml = math.sqrt(abs(F)**2 - abs(Fl)**2)  # ForceMomentum-Length
				Fmx = Fml / math.sqrt((s.y/s.x)**2+1)  # relative
				Fmy = -Fmx * s.y/s.x
				Fm = Vector2(Fmx, Fmy)  # ForceMomentum
				# Fm = Vector2(Cu.x+Fmx, Cu.x+Fmy)  # ForceMomentum
				print('Fll = {}, Fl = {}, Fm = {}, s = {}'.format(Fll, Fl, Fm, s))
				# M = abs(Fm) * abs(s)  # Momentum
//				M = F ** s  # == -s x F
				# J = mypoly.getInertiaMoment()
				# J = 11656333333
				J = 48000000000

				Fl += forceGrav
				acc = Fl / _MASS  # accel
//				# aa = pow(Fm, s, True) * M / J  # angularAccel
//				aa = M / J
				print('VUW={:.1f},ax={:.1f},ay={:.1f},aa={:.2f},Fmx={:.1f},Fmy={:.1f},M={:.0f}'.format(Volume, acc.x, acc.y, aa, Fm.x, Fm.y, M, J))
				mypoly.setAccel(acc.x, acc.y)
				mypoly.setAngularAccel(aa)
			elif len(intersections) > 2:
				print('WEIRD')
	else:
		F = World.gravForce(_MASS)
		acc = F / _MASS
		mypoly.setAccel(acc.x, acc.y)
		mypoly.setAngularAccel(0)


def timer():
	tick()
	root.after(_DELAY, timer)  # repeat


def QuitDestroy(event=None):
	print('Quitting...')
	root.destroy()
	root.quit()
	print('Destroyed.')

clock = clock_yield()
mypoly = Polygon([Point(200, 50), Point(560, 30), Point(585, 160), Point(210, 190)], fill=getRandomColor(), spawn=True)
mypoly.setAccel(0, _GRAVACCEL)
root.bind('z', tick)
root.bind('<Escape>', QuitDestroy)
root.bind('<Control-c>', QuitDestroy)
root.bind('<a>', lambda e: mypoly.setAccel(ax=-20))
root.bind('<d>', lambda e: mypoly.setAccel(ax=20))
root.bind('<w>', lambda e: mypoly.setAccel(ay=-20))
root.bind('<s>', lambda e: mypoly.setAccel(ay=20))
root.bind('<q>', lambda e: mypoly.setAngularAccel(-0.2))
root.bind('<e>', lambda e: mypoly.setAngularAccel(0.2))
root.bind('<KeyRelease-a>', lambda e: mypoly.setAccel(ax=0))
root.bind('<KeyRelease-d>', lambda e: mypoly.setAccel(ax=0))
root.bind('<KeyRelease-w>', lambda e: mypoly.setAccel(ay=_GRAVACCEL))
root.bind('<KeyRelease-s>', lambda e: mypoly.setAccel(ay=_GRAVACCEL))
root.bind('<KeyRelease-q>', lambda e: mypoly.setAngularAccel(0))
root.bind('<KeyRelease-e>', lambda e: mypoly.setAngularAccel(0))
root.after(_DELAY, timer)  # start

canv.pack()
canv.create_line(0, _WATERLINE, _WIDTH, _WATERLINE, width=1., fill='red', dash=(2, 2, 2))
label_fps.place(x=16, y=16, width=64, height=64)
root.mainloop()
