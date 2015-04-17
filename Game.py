from Player import Player
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
		self.players = [Player(eventManager)]

	def Start(self):
		self.state = self.STATE.RUNNING
		self.eventManager.Post(GameStartedEvent(self))

	def Notify(self, event):
		# weird and non-optimal.. :O
		if isinstance(event, TickUpdateEvent):
			if self.state is self.STATE.PREPARING:
				self.Start()
