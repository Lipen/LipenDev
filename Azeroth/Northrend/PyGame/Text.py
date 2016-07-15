from common import *
from Entity import Entity


class Text(Entity):

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

	def __init__(self, pos=None, text='<DefaultString>', color=(255, 255, 255), colorBackground=None, font=None):
		# TODO: <Text class> Implement isVisible
		if pos is None:
			self.pos = Point(0, 0)
		else:
			self.pos = pos
		if font is None:
			self.font = LightSim.font_monospace
		else:
			self.font = font
		self.text = str(text)
		self.color = color
		self.colorBackground = colorBackground
		self.update()

	def setText(self, text):
		self.text = str(text)
		self.update()

	def centerTowardsEntity(self, ent, isX=True, isY=True):
		# FIXME: Move to <Entity> class?
		R = ent.getRect()
		r = self.getRect()
		if isX:
			self.x += R.centerx - r.centerx
		if isY:
			self.y += R.centery - r.centery

	def update(self):
		self.surface = self.font.render(self.text, True, self.color, self.colorBackground)
