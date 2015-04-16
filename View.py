from Events import TickDisplayEvent


class View:

	"""//View
	"""

	def __init__(self, eventManager):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)

		# <STUFF INIT> goes here
		# MAYBE

		# ship = Ship()
		# NO

	def update(self):
		pass
		# TODO: //for char in chars: char.update()

	def Notify(self, event):
		if isinstance(event, TickDisplayEvent):
			self.update()
