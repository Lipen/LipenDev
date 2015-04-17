from Events import TickDisplayEvent, CharactorDisplayEvent, CharactorSpawnEvent
from tkinter import Label


class View:

	"""//View
	"""

	def __init__(self, eventManager, canv):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)
		self.canv = canv
		self.label_fps = Label(canv, bg='#cccccc', fg='#000000', font='Meslo 20')
		self.label_fps.place(x=16, y=16, width=64, height=64)

	def Notify(self, event):
		if isinstance(event, TickDisplayEvent):
			dt = event.dt
			# print('dt =', dt)
			if dt > 0:
				print('FPS: {}'.format(1./dt))
				self.label_fps['text'] = round(1./dt)
			else:
				print('FPS: XX')
				self.label_fps['text'] = "XX"
			self.eventManager.Post(CharactorDisplayEvent(self.canv))
		elif isinstance(event, CharactorSpawnEvent):
			polygon = event.charactor.ship.polygon
			polygon.poly = self.canv.create_polygon(polygon.getCoords(), fill=polygon.color, activefill=polygon.activecolor)
