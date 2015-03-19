from common import *
from Entity import Entity
from Point import Point
import pygame

CHANGE_INTENSITY_CAP = 0.009


class Cell(Entity):

	"""Base cell class
	"""

	@property
	def x(self):
		return self.pos.x

	@x.setter
	def x(self, value):
		self.pos.x = value

	@property
	def y(self):
		return self.pos.y

	@y.setter
	def y(self, value):
		self.pos.y = value

	@property
	def intensity(self):
		return self._intensity

	@intensity.setter
	def intensity(self, value):
		# self.changeIntensity(value)
		self.__intensity = value

	def __init__(self, field, x, y, width, height, color=None, activecolor=(255, 255, 221, 255)):
		if color is None:
			self.color = getRandomColor()
		else:
			self.color = color
		self.field = field
		self.pos = Point(x, y)
		self.width, self.height = width, height
		self.previousColor = self.color
		self.activecolor = activecolor
		self.active = False
		self._intensity = 0  # Current real intensity
		self.__intensity = 0  # Final intensity
		self.sources = []
		self.surface = pygame.Surface((width, height))
		# self.surface.fill(Color('black'))  # is this line neccessary?..

	def isContainsPoint(self, p):
		return (self.x < p.x <= self.x+self.width) and (self.y < p.y <= self.y+self.height)

	def getCenter(self):
		return Point(self.x + self.width/2, self.y + self.height/2)

	def getNumber(self):
		return (round(self.x/self.width), round(self.y/self.height))

	def getDistanceBetweenCells(c1, c2):
		return Point.getDistanceBetweenPoints(c1.getCenter(), c2.getCenter())

	def changeIntensity(self, value):
		if self._intensity != self.__intensity:
			dI = value - self.intensity
			t = dI
			dI = sign(dI)*min(CHANGE_INTENSITY_CAP, abs(dI))
			# Exponential:
			COEF = math.log(CHANGE_INTENSITY_CAP+1)/CHANGE_INTENSITY_CAP
			tt = sign(dI) * abs(math.exp(COEF*dI) - 1)
			self._intensity += tt
			# print('DBG changeIntensity(): {:.2f} -> {:.2f}'.format(t, tt))
			# Linear:
			# self._intensity += sign(dI)*dI**4 * CHANGE_INTENSITY_CAP**(1-4)

	def recalculate(self):
		if self.active:
			# pass
			self.intensity = 1
		else:
			if len(self.sources) > 0:
				I = 0
				for source in self.sources:
					r = Cell.getDistanceBetweenCells(self, source)
					if r > 0:
						I += Cell.calculateIntensity(r)
				self.intensity = I
			else:
				self.intensity = 0

	def linkSource(self, source):
		self.sources.append(source)
		self.recalculate()

	def unlinkSource(self, source):
		self.sources.remove(source)
		self.recalculate()

	def click(self):
		if self.active:
			self.active = False
			self.recalculate()
			for cell in self.field.cells:
				if cell is not self:
					cell.unlinkSource(self)
		else:
			self.active = True
			self.recalculate()
			for cell in self.field.cells:
				if cell is not self:
					cell.linkSource(self)

	def update(self):
		# self.recalculate()
		self.changeIntensity(self.__intensity)

		# if self.active:
		if False:
			if self.activecolor != self.previousColor:
				self.surface.fill(self.activecolor)
				self.previousColor = self.activecolor
		else:
			newcolor = tuple(map(lambda z: max(0, min(255, z*self.intensity)), self.color[:3])) + (255, )
			if newcolor != self.previousColor:
				self.surface.fill(newcolor)
				self.previousColor = newcolor
