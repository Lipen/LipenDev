class Event:

	"""Event baseclass
	"""

	def __init__(self):
		self.name = 'Generic Event'


class TickUpdateEvent(Event):

	def __init__(self, dt):
		self.name = 'Tick Update Event'
		self.dt = dt


class TickDisplayEvent(Event):

	def __init__(self, dt):
		self.name = 'Tick Display Event'
		self.dt = dt


class QuitEvent(Event):

	def __init__(self, root):
		self.name = 'Program Quit Event'
		self.root = root  # tk`s root


class MapBuiltEvent(Event):

	def __init__(self, map):
		self.name = 'Map Finished Building Event'
		self.map = map


class GameStartedEvent(Event):

	def __init__(self, game):
		self.name = 'Game Started Event'
		self.game = game


class CharactorMoveRequest(Event):

	def __init__(self, direction):
		self.name = 'Charactor Move Request'
		self.direction = direction


class CharactorPlaceEvent(Event):

	"""This event occurs when a Charactor is *placed* in a sector, ie it doesn't move there from an adjacent sector.
	"""

	def init__(self, charactor):
		self.name = 'Charactor Placement Event'
		self.charactor = charactor


class CharactorMoveEvent(Event):

	def __init__(self, charactor):
		self.name = 'Charactor Move Event'
		self.charactor = charactor


class KeyPressedEvent(Event):

	def __init__(self, keyname):
		self.name = 'Key Pressed Event'
		self.keyname = keyname
