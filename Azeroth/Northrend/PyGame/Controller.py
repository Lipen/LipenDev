import time
from common import clock_yield
from Events import TickUpdateEvent, TickDisplayEvent, KeyPressedEvent, QuitEvent, CharactorUpdateEvent


class Controller:

	"""//Controller
	"""

	def __init__(self, eventManager):
		self.eventManager = eventManager
		self.eventManager.RegisterListener(self)

		self.flowing = True

	def Run(self, fps):
		"""timeController coroutine
		"""

		clockUpdate = clock_yield()
		clockDisplay = clock_yield()

		while self.flowing:
			nt = time.time() + 1./fps

			while time.time() < nt:
				dt = next(clockUpdate)
				self.eventManager.Post(TickUpdateEvent(dt))

			dt = next(clockDisplay)
			self.eventManager.Post(TickDisplayEvent(dt))

	def Notify(self, event):
		if isinstance(event, TickUpdateEvent):
			dt = event.dt
			self.eventManager.Post(CharactorUpdateEvent(dt))
		elif isinstance(event, QuitEvent):
			self.flowing = False
			if event.root:  # is not None
				print('Destroying root...')
				event.root.destroy()
				event.root.quit()
				print('Root destroyed.')
		elif isinstance(event, KeyPressedEvent):
			print('Controller::Pressed <{}>.'.format(event.keyname))
