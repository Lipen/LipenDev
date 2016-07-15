import math
from Point import Point
from Vector2 import Vector2
from Polygon import Polygon
from Engine import Engine
from common import getRandomColor


class Ship:

	"""Spaceship base class
	"""

	@property
	def angle(self):
		return self.polygon.angle

	@angle.setter
	def angle(self, value):
		raise NotImplemented

	def __init__(self, points, pos=None, angle=-90, color=None):
		self.points = points
		self.engines = {}
		self.engines_count = 0
		self.engines_working = set()

		if pos is None:
			pos = Point(0, 0)
		self.pos = pos

		if color is None:
			color = getRandomColor()

		self.polygon = Polygon(points, pos, angle, fill=color)

		c = self.polygon.getCenter()
		self.radius = max(Point.getDistanceBetweenPoints(c, p) for p in self.polygon.points) + 5

	def addEngine(self, pos, direction=0, force=0, btn_start='', btn_stop='', btn_toggle=''):
		ID = self.engines_count
		c = self.polygon.getCenter(self.polygon._points)
		r = Point.getDistanceBetweenPoints(c, pos)
		offsetAngle = Point.getAngleBetweenPoints(c, pos) - math.radians(self.angle)
		self.engines[ID] = Engine(id=ID, offsetDist=r, offsetAngle=offsetAngle, direction=direction, maxforce=force, btns=(btn_start, btn_stop, btn_toggle))
		print('Dbg: Created engine ID#{}'.format(ID))
		self.engines_count += 1

	def startEngine(self, id):
		try:
			for i in id:
				self.startEngine(i)
		except:
			print('DBG: startEngine #{}'.format(id))
			self.engines_working.add(id)

	def stopEngine(self, id):
		try:
			for i in id:
				self.stopEngine(i)
		except:
			print('DBG: stopEngine #{}'.format(id))
			if id in self.engines_working:
				self.engines_working.remove(id)

	def toggleEngine(self, id):
		try:  # if <is iterable>
			for i in id:
				self.toggleEngine(i)
		except:
			print('DBG: toggleEngine #{}'.format(id))
			if id in self.engines_working:
				self.stopEngine(id)
			else:
				self.startEngine(id)

	def update(self, dt):
		acc = Vector2(0, 0)
		aa = 0

		for id in list(self.engines_working):  # make sure to copy
			engine = self.engines[id]
			engine_f = engine.maxforce
			engine_r = engine.offsetDist
			engine_oa = engine.offsetAngle

			F = Vector2(
				engine_f*math.cos(math.radians(engine.direction+self.angle)),
				engine_f*math.sin(math.radians(engine.direction+self.angle)))
			c = self.polygon.getCenter()
			r = Vector2.newFromPoints(c, Point(
				c.x + engine_r * math.cos(engine_oa+math.radians(self.angle)),
				c.y + engine_r * math.sin(engine_oa+math.radians(self.angle))))
			M = r ** F
			J = 2E9  # 11656333333

			_MASS = 3000000
			acc += F*abs((F*r)/abs(F)/abs(r)) / _MASS
			aa += M / J

			# print('#{}: F={}, r={}, M={}, Acc={}, aa={}'.format(id, F, r, round(M), acc, aa))

		self.polygon.setAccel(acc.x, acc.y)
		self.polygon.setAngularAccel(aa)
		self.polygon.update(dt)
