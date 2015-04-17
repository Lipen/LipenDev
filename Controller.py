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

		skip = 1. / fps
		max_skip = 5

		dt = 0
		nt = time.time()  # "next tick"
		clock = clock_yield()

		while self.flowing:
			loops = 0

			while time.time() > nt and loops < max_skip:
				dt = next(clock)
				self.eventManager.Post(TickUpdateEvent(dt))
				nt += skip
				loops += 1
				# print('dbg: timeController>> loops={}, nt={}, skip={}'.format(loops, nt, skip))

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
			print('Pressed <{}>.'.format(event.keyname))
