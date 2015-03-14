from common import *


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

	def getDistanceBetweenPoints(p1, p2):
		return math.hypot(p1.x - p2.x, p1.y-p2.y)

	def getAngleBetweenPoints(p1, p2):
		return math.atan2(p2.y-p1.y, p2.x-p1.x)
