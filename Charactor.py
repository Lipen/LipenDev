import math
from Point import Point
from Ship import Ship
from Events import CharactorSpawnEvent, CharactorUpdateEvent, CharactorDisplayEvent, KeyPressedEvent
from tkinter import LAST


class Charactor:

	"""<Ship with engines> adapter
	"""

	throws = []

	def __init__(self, eventManager):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)

		self.ship = Ship([Point(50, 0), Point(75, 0), Point(75, 25), Point(125, 25), Point(125, 0), Point(150, 0), Point(200, 100), Point(175, 150), Point(125, 150), Point(125, 125), Point(100, 100), Point(75, 125), Point(75, 150), Point(25, 150), Point(0, 100)], Point(300, 200), -90)
		# //Engines spawn
		self.ship.addEngine(Point(100, 100), 0, 5000000, btn_toggle='w', btn_stop='s')  # Main
		self.ship.addEngine(Point(50, 150), 5, 1000000, btn_toggle='d', btn_stop='s')  # Left (-->)
		self.ship.addEngine(Point(150, 150), -5, 1000000, btn_toggle='a', btn_stop='s')  # Right (<--)

		self.eventManager.Post(CharactorSpawnEvent(self))

	def render(self, canv):
		polygon = self.ship.polygon
		try:
			polygon.poly
		except:
			print('Exception::Charactor.render>>No defined poly yet.')
		else:
			canv.coords(polygon.poly, *polygon.getCoords())

		for throw in self.throws:
			canv.delete(throw)

		self.throws = []

		for id in list(self.ship.engines_working):  # make sure to copy
			engine = self.ship.engines[id]
			engine_r, engine_oa = engine[1]
			c = self.ship.polygon.getCenter()
			engine_pos = Point(c.x + engine_r * math.cos(engine_oa+math.radians(self.ship.angle)), c.y + engine_r * math.sin(engine_oa+math.radians(self.ship.angle)))
			L = 30
			throw_pos = Point(c.x + (engine_r+L) * math.cos(engine_oa+math.radians(self.ship.angle)), c.y + (engine_r+L) * math.sin(engine_oa+math.radians(self.ship.angle)))
			throw = canv.create_line(throw_pos.x, throw_pos.y, engine_pos.x, engine_pos.y, fill='red', width=2, arrow=LAST)
			self.throws.append(throw)

	def update(self, dt):
		self.ship.update(dt)

	def Notify(self, event):
		if isinstance(event, CharactorUpdateEvent):
			dt = event.dt
			self.update(dt)
		elif isinstance(event, CharactorDisplayEvent):
			self.render(event.canv)
		elif isinstance(event, KeyPressedEvent):
			keyname = event.keyname
			for id, engine in self.ship.engines.items():
				if engine[4][0] == keyname:
					self.ship.startEngine(id)
				if engine[4][1] == keyname:
					self.ship.stopEngine(id)
				if engine[4][2] == keyname:
					self.ship.toggleEngine(id)
