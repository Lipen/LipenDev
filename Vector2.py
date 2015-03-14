from common import *


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

	def __radd__(self, other):
		return Vector2(self.x+other, self.y+other)

	def __sub__(self, other):
		if isinstance(other, Vector2):
			return Vector2(self.x-other.x, self.y-other.y)
		else:
			return Vector2(self.x-other, self.y-other)

	def __rsub__(self, other):
		return Vector2(self.x-other, self.y-other)

	def __mul__(self, other):
		if isinstance(other, Vector2):
			return self.x*other.x + self.y*other.y
		else:
			return Vector2(self.x*other, self.y*other)

	def __rmul__(self, other):
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
