import time
from common import clock_yield
from Events import TickUpdateEvent, TickDisplayEvent, KeyPressedEvent, QuitEvent


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

		while self.flowing:
			loops = 0

			while time.time() > nt and loops < max_skip:
				dt = clock_yield()
				ev = TickUpdateEvent(dt)
				self.eventManager.Post(ev)
				nt += skip
				loops += 1
				# print('dbg: Controller>> loops={}, nt={}, skip={}'.format(loops, nt, skip))

			ev = TickDisplayEvent(dt)
			self.eventManager.Post(ev)

	def Notify(self, event):
		if isinstance(event, TickUpdateEvent):
			pass  # TODO: some kind of update call?..
		elif isinstance(event, QuitEvent):
			self.flowing = False
			if event.root:  # is not None
				print('Destroying root...')
				event.root.destroy()
				event.root.quit()
				print('Root destroyed.')
		elif isinstance(event, KeyPressedEvent):
			print('Pressed <{}>.'.format(event.keyname))
