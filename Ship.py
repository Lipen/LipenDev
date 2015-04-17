import math
from Point import Point
from Vector2 import Vector2
from Polygon import Polygon
from common import getRandomColor


class Ship:

	"""Spaceship base class
	"""

	engines = {}
	engines_count = 0
	engines_working = set()

	@property
	def angle(self):
		return self.polygon.angle

	@angle.setter
	def angle(self, value):
		raise NotImplemented

	def __init__(self, points, pos=None, angle=-90, color=None):
		self.points = points
		# self.angle = angle

		if pos is None:
			pos = Point(0, 0)
		self.pos = pos

		if color is None:
			color = getRandomColor()

		self.polygon = Polygon(points, pos, angle, fill=color)

	def addEngine(self, pos, direction=0, force=0, btn_start='', btn_stop='', btn_toggle=''):
		# pos - offset from the ship postion
		# direction - angle, relative to ship.angle (forward -> 0)
			# TODO: //Vector in future?
		# force - float

		ID = self.engines_count
		if self.angle != 0:
			c = self.polygon.getCenter(self.polygon._points)
			r = Point.getDistanceBetweenPoints(c, pos)
			offsetAngle = Point.getAngleBetweenPoints(c, pos) - math.radians(self.angle)
			# print('{}: c={}, r={}, a_new={}'.format(ID, c, r, a))
			# pos = Point(r*math.cos(a), r*math.sin(a))
		self.engines[ID] = (ID, (r, offsetAngle), direction, force, (btn_start, btn_stop, btn_toggle))  # TODO: new Engine(..)
		print('Dbg: Created engine ID#{}'.format(ID))
		print('AT: {}. With: {}'.format(pos, self.engines[ID][1]))
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
				# self.engines_working.remove(id)
			else:
				self.startEngine(id)
				# self.engines_working.add(id)

	def update(self, dt):
		acc = Vector2(0, 0)
		aa = 0

		for id in list(self.engines_working):  # make sure to copy
			engine = self.engines[id]
			engine_r, engine_oa = engine[1]

			F = Vector2(
				engine[3]*math.cos(math.radians(engine[2]+self.angle)),
				engine[3]*math.sin(math.radians(engine[2]+self.angle)))
			c = self.polygon.getCenter()
			r = Vector2.newFromPoints(c, Point(c.x + engine_r * math.cos(engine_oa+math.radians(self.angle)), c.y + engine_r * math.sin(engine_oa+math.radians(self.angle))))
			M = r ** F
			J = 2E9  # 11656333333

			_MASS = 3000000
			acc += F*abs((F*r)/abs(F)/abs(r)) / _MASS
			aa += M / J

			print('#{}: F={}, r={}, M={}, Acc={}, aa={}'.format(id, F, r, round(M), acc, aa))

		self.polygon.setAccel(acc.x, acc.y)
		self.polygon.setAngularAccel(aa)
		self.polygon.update(dt)
