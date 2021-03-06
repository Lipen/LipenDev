import math
from Point import Point
from Vector2 import Vector2
from Ship import Ship
from Events import CharactorSpawnEvent, CharactorUpdateEvent, CharactorDisplayEvent, KeyPressedEvent, CharactorCollisionEvent
from tkinter import LAST


class Charactor:

	"""<Ship with engines> adapter
	"""

	def __init__(self, eventManager, points, pos=None, angle=-90, color=None):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)

		self.ship = Ship(points, pos, angle, color)
		self.throws = []
		self.field = 0
		self.field_color = 'blue'

		self.eventManager.Post(CharactorSpawnEvent(self))

	def render(self, canv):
		polygon = self.ship.polygon
		canv.coords(polygon.poly, *polygon.getCoords())

		# try:
		# 	polygon.poly
		# except:
		# 	print('Exception::Charactor.render>>No defined poly yet.')
		# else:
		# 	canv.coords(polygon.poly, *polygon.getCoords())

		for throw in self.throws:
			canv.delete(throw)
		self.throws = []

		c = self.ship.polygon.getCenter()

		canv.delete(self.field)
		self.field = canv.create_oval(c.x-self.ship.radius, c.y-self.ship.radius, c.x+self.ship.radius, c.y+self.ship.radius, outline=self.field_color, width=1.5)

		for id in list(self.ship.engines_working):  # make sure to copy
			engine = self.ship.engines[id]
			engine_r = engine.offsetDist
			engine_oa = engine.offsetAngle
			engine_dir = engine.direction
			engine_f = engine.maxforce
			L = 15*engine_f/1E6

			engine_pos = Point(
				c.x + engine_r * math.cos(engine_oa+math.radians(self.ship.angle)),
				c.y + engine_r * math.sin(engine_oa+math.radians(self.ship.angle)))
			throw_pos = Point(
				engine_pos.x - L * math.cos(math.radians(engine_dir + self.ship.angle)),
				engine_pos.y - L * math.sin(math.radians(engine_dir + self.ship.angle)))

			throw = canv.create_line(throw_pos.x, throw_pos.y, engine_pos.x, engine_pos.y, fill='red', width=2, arrow=LAST)
			self.throws.append(throw)

	def collision(self, charactor):
		d = Point.getDistanceBetweenPoints(self.ship.polygon.getCenter(), charactor.ship.polygon.getCenter())

		if d <= self.ship.radius+charactor.ship.radius:
			# print('Collision! d={}'.format(d))
			charactor.collision_list.append(self)
			vc = charactor.ship.polygon.vel
			if abs(vc) > 0:
				r = Vector2.newFromPoints(charactor.ship.polygon.getCenter(), self.ship.polygon.getCenter())
				a = math.acos(vc*r/abs(vc)/abs(r))
				# vcy = vc*r/abs(r)
				vcx = vc*math.cos(a)
				vcy = vc*math.sin(a)
				vf = -vcx + vcy
				charactor.ship.polygon.vel = vf

	def update(self, dt):
		self.ship.update(dt)
		self.collision_list = []
		self.eventManager.Post(CharactorCollisionEvent(self))
		if self.collision_list:
			self.field_color = 'red'
		else:
			self.field_color = 'blue'

	def Notify(self, event):
		if isinstance(event, CharactorUpdateEvent):
			dt = event.dt
			self.update(dt)
		elif isinstance(event, CharactorDisplayEvent):
			self.render(event.canv)
		elif isinstance(event, KeyPressedEvent):
			keyname = event.keyname
			# print('Charactor::Caught <{}> keypress.'.format(keyname))
			for id in list(self.ship.engines.keys()):  # make sure to copy
				engine = self.ship.engines[id]
				if engine.btns[0] == keyname:
					self.ship.startEngine(id)
				if engine.btns[1] == keyname:
					self.ship.stopEngine(id)
				if engine.btns[2] == keyname:
					self.ship.toggleEngine(id)
		elif isinstance(event, CharactorCollisionEvent):
			if event.charactor is not self:
				self.collision(event.charactor)
