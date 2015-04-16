from Events import TickUpdateEvent, TickDisplayEvent


class EventManager:

	"""Base event manager.
	"""

	def __init__(self, ):
		from weakref import WeakKeyDictionary
		self.listeners = WeakKeyDictionary()
		self.eventQueue = []

	def RegisterListener(self, listener):
		self.listeners[listener] = 1

	def UnregisterListener(self, listener):
		if listener in self.listeners.keys():
			del self.listeners[listener]

	def Post(self, event):
		# if not isinstance(event, TickDisplayEvent):
		if not isinstance(event, TickUpdateEvent) and not isinstance(event, TickDisplayEvent):
			print(event.name)

		for listener in self.listeners.keys():
			listener.Notify(event)
