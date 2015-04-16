class Charactor:

	"""//There must be <Ship> class..
	"""

	def __init__(self, eventManager):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)

	def Notify(self, event):
		pass
