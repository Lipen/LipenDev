from Point import Point
from Charactor import Charactor
from Events import GameStartedEvent, TickUpdateEvent


class Game:

	"""//Model
	"""

	from enum import Enum
	STATE = Enum('GameState', 'PREPARING RUNNING PAUSED')

	def __init__(self, eventManager):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)

		self.state = self.STATE.PREPARING

		char1 = Charactor(eventManager, points=[Point(50, 0), Point(75, 0), Point(75, 25), Point(125, 25), Point(125, 0), Point(150, 0), Point(200, 100), Point(175, 150), Point(125, 150), Point(125, 125), Point(100, 100), Point(75, 125), Point(75, 150), Point(25, 150), Point(0, 100)], pos=Point(150, 200), angle=-90)
		char1.ship.addEngine(Point(100, 100), 0, 5E6, btn_toggle='w', btn_stop='s')  # Main
		char1.ship.addEngine(Point(50, 150), 5, 1E6, btn_toggle='d', btn_stop='s')  # Left (-->)
		char1.ship.addEngine(Point(150, 150), -5, 1E6, btn_toggle='a', btn_stop='s')  # Right (<--)
		char1.ship.addEngine(Point(25, 50), 140, 2E6, btn_toggle='x', btn_stop='s')  # Left-front (-ccw-V>)
		char1.ship.addEngine(Point(175, 50), -140, 2E6, btn_toggle='z', btn_stop='s')  # Right-front (<V-cw--)

		char2 = Charactor(eventManager, points=[Point(0, 75), Point(75, 50), Point(75, 0), Point(100, 25), Point(100, 50), Point(150, 75), Point(175, 75), Point(175, 100), Point(150, 125), Point(125, 125), Point(125, 150), Point(150, 150), Point(175, 175), Point(175, 200), Point(150, 200), Point(100, 225), Point(100, 250), Point(75, 275), Point(75, 225), Point(0, 200), Point(0, 175), Point(50, 150), Point(50, 125), Point(0, 100)], pos=Point(400, 100), angle=-180)
		char2.ship.addEngine(Point(125, 137.5), 0, 4E6, btn_toggle='u', btn_stop='j')  # Main
		char2.ship.addEngine(Point(175, 187.5), 5, 3E6, btn_toggle='k', btn_stop='j')  # Left (-->)
		char2.ship.addEngine(Point(175, 87.5), -5, 3E6, btn_toggle='h', btn_stop='j')  # Right (<--)
		char2.ship.addEngine(Point(75, 250), 150, 1E6, btn_toggle='m', btn_stop='j')  # Left-front (-ccw-V>)
		char2.ship.addEngine(Point(75, 25), -150, 1E6, btn_toggle='n', btn_stop='j')  # Right-front (<V-cw--)

		self.chars = [char1, char2]

	def Start(self):
		self.state = self.STATE.RUNNING
		self.eventManager.Post(GameStartedEvent(self))

	def Notify(self, event):
		# weird and non-optimal.. :O
		if isinstance(event, TickUpdateEvent):
			if self.state is self.STATE.PREPARING:
				self.Start()
