from Point import Point
from Vector2 import Vector2
import math


class Polygon:

	"""Base 2D polygon class.
	"""

	vel = Vector2(0, 0)
	acc = Vector2(0, 0)
	angularVel = 0
	angularAcc = 0

	def __init__(self, points, pos, angle, fill, activefill='#99dd33'):
		# _points = Base points (not rotated, "pos"=0,0)
		# points = Real points ("on canvas")
		self.pos = pos
		self.angle = angle

		self.points = [Point(p.x+pos.x, p.y+pos.y) for p in points]
		# //Same:
		# for p in list(points):
		# 	self.points.append(Point(p.x+pos.x, p.y+pos.y))

		c = self.getCenter(points)
		self._points = []
		for p in points:
			r = Point.getDistanceBetweenPoints(c, p)
			a = Point.getAngleBetweenPoints(c, p) + math.radians(-angle)
			self._points.append(Point(c.x + r * math.cos(a), c.y + r * math.sin(a)))
		# self.updatePoints()  # init <points>

		self.color = fill
		self.activecolor = activefill

	def getArea(self, points=None):
		if points is None:
			points = self.points
		return 0.5 * abs(sum(points[i-1].x*points[i].y-points[i].x*points[i-1].y for i in range(len(points))))

	def getCenter(self, points=None):
		if points is None:
			points = self.points
		A = self.getArea()
		return Point(
			1/6/A*sum((points[i-1].x+points[i].x)*(points[i-1].x*points[i].y-points[i].x*points[i-1].y) for i in range(len(points))),
			1/6/A*sum((points[i-1].y+points[i].y)*(points[i-1].x*points[i].y-points[i].x*points[i-1].y) for i in range(len(points))))

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

	def updatePoints(self):
		"""List of points (offset by <pos> and <angle> from initial _points)
		"""
		c = self.getCenter(self._points)
		self.points = []
		for p in self._points:
			r = Point.getDistanceBetweenPoints(c, p)
			a = Point.getAngleBetweenPoints(c, p) + math.radians(self.angle)
			self.points.append(Point(c.x + r * math.cos(a) + self.pos.x, c.y + r * math.sin(a) + self.pos.y))

	def accel(self, dt):
		self.vel += self.acc * dt

	def move(self, dt):
		self.pos.addVector(self.vel * dt)
		# //Same:
		# for p in self.points:
		# 	p.addVector(self.vel * dt)

	def rotate(self, dt):
		self.angularVel += self.angularAcc * dt
		# da = self.angularVel * dt
		self.angle += math.degrees(self.angularVel * dt)

	def update(self, dt):
		self.accel(dt)
		self.move(dt)
		self.rotate(dt)
		self.updatePoints()

		# print('Angle={}'.format(self.angle))
		# print('v.x={:.1f},v.y={:.1f},a.x={:.1f},a.y={:.1f},aV={:.1f},aA={:.3f}'.format(self.vel.x, self.vel.y, self.acc.x, self.acc.y, self.angularVel, self.angularAcc))
