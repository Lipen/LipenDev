from Charactor import Charactor


class Player:

	"""Player class
	"""

	def __init__(self, eventManager):
		self.eventManager = eventManager
		# self.eventManager.RegisterListener(self)

		self.charactors = [Charactor(eventManager)]
