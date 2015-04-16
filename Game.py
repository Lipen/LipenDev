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

		# ship -> init
		# NO
		# map, etc -> init
		# MAYBE

	def Start(self):
		# self.map.build() # // :O
		self.state = self.STATE.RUNNING
		ev = GameStartedEvent(self)
		self.eventManager.Post(ev)

	def Notify(self, event):
		# weird and non-optimal.. :O
		if isinstance(event, TickUpdateEvent):
			if self.state is self.STATE.PREPARING:
				self.Start()
