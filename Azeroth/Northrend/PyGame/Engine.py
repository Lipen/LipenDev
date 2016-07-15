class Engine:

	"""Engine class
	"""

	def __init__(self, id, offsetDist, offsetAngle, direction, maxforce, btns):
		self.id = id
		self.offsetDist = offsetDist
		self.offsetAngle = offsetAngle
		self.direction = direction
		self.maxforce = maxforce
		self.btns = btns  # FIXME: redo
		# self.isWorking = False

	# def turnOn
