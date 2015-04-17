from Point import Point
from Ship import Ship
from Events import CharactorSpawnEvent, CharactorUpdateEvent, CharactorDisplayEvent, KeyPressedEvent


class Charactor:

	"""<Ship with engines> adapter
	"""

	def __init__(self, eventManager):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)

		self.ship = Ship([Point(50, 0), Point(75, 0), Point(75, 25), Point(125, 25), Point(125, 0), Point(150, 0), Point(200, 100), Point(175, 150), Point(125, 150), Point(125, 125), Point(100, 100), Point(75, 125), Point(75, 150), Point(25, 150), Point(0, 100)], Point(300, 200), -90)
		# //Engines spawn
		self.ship.addEngine(Point(100, 100), 0, 5000000, btn_toggle='w', btn_stop='s')  # Main
		# Weird signs... Should be vise-versa (left +35, right -35)..
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
