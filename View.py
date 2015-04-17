from Events import TickDisplayEvent, CharactorDisplayEvent, CharactorSpawnEvent


class View:

	"""//View
	"""

	def __init__(self, eventManager, canv):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)
		self.canv = canv

		# <STUFF INIT> goes here
		# MAYBE

	def Notify(self, event):
		if isinstance(event, TickDisplayEvent):
			# ::self.update()::
			self.eventManager.Post(CharactorDisplayEvent(self.canv))
		elif isinstance(event, CharactorSpawnEvent):
			print('VIEW: DEFINING POLY...')
			polygon = event.charactor.ship.polygon
			polygon.poly = self.canv.create_polygon(polygon.getCoords(), fill=polygon.color, activefill=polygon.activecolor)
			print('VIEW: POLY DEFINED')
