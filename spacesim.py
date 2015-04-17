from tkinter import *
from common import *
from Vector2 import Vector2
from Point import Point
from Events import *
from EventManager import EventManager
from Game import Game  # from Model import Model
from View import View
from Controller import Controller
from threading import Thread

_TITLE = 'shipsim'
_WIDTH = 800
_HEIGHT = 600
_FPS = 60
_DELAY = round(1000. / _FPS)


class Ship:

	"""Spaceship base class
	"""

	engines = {}
	engines_count = 0
	engines_working = set()

	@property
	def angle(self):
		return self.polygon.angle

	@angle.setter
	def angle(self, value):
		raise NotImplemented

	def __init__(self, points, pos=None, angle=-90, color=None):
		# pos - ship position. (Not center of mass)
		if pos is None:
			pos = Point(0, 0)
		self.pos = pos  # Initial position offset
		if color is None:
			color = getRandomColor()
		self.polygon = Polygon([Point(p.x+pos.x, p.y+pos.y) for p in points], fill=color, spawn=True)
		self.polygon.angle = angle

	def addEngine(self, pos=None, direction=0, force=0):
		# pos - offset from the ship postion
		# direction - angle, relative to ship.angle (forward -> 0)
			# TODO: //Vector in future?
		# force - float
		if pos is None:
			pos = Point(0, 0)
		assert isinstance(pos, Point)
		ID = self.engines_count
		self.engines[ID] = [ID, pos, direction, force]  # TODO: new Engine(..)
		print('Dbg: Created engine ID#{}'.format(ID))
		self.engines_count += 1

	def startEngine(self, id):
		try:
			for i in id:
				self.startEngine(i)
		except:
			print('DBG: startEngine #{}'.format(id))
			self.engines_working.add(id)

	def stopEngine(self, id):
		try:
			for i in id:
				self.stopEngine(i)
		except:
			print('DBG: stopEngine #{}'.format(id))
			if id in self.engines_working:
				self.engines_working.remove(id)

	def toggleEngine(self, id):
		try:
			for i in id:
				self.toggleEngine(i)
		except:
			print('DBG: toggleEngine #{}'.format(id))
			if id in self.engines_working:
				self.engines_working.remove(id)
			else:
				self.engines_working.add(id)

	def update(self):
		# //ship calculations:
		acc = Vector2(0, 0)
		aa = 0
		for id in self.engines_working:
			engine = self.engines[id]

			F = Vector2(engine[3]*math.cos(math.radians(engine[2]+self.angle)), engine[3]*math.sin(math.radians(engine[2]+self.angle)))
			print('DBG: Center = {}'.format(self.polygon.getCenter()))
			r = Vector2.newFromPoints(self.polygon.getCenter(), Point(engine[1].x+self.pos.x, engine[1].y+self.pos.y))
			M = r ** F
			J = 11656333333

			acc += F / _MASS
			aa += M / J

			print('#{}: F={}, r={}, M={}, Acc={}, aa={}'.format(id, F, r, round(M), acc, aa))

		self.polygon.setAccel(acc.x, acc.y)
		self.polygon.setAngularAccel(aa)
		self.polygon.update()


def main():
	eventManager = EventManager()

	# //STUFF INIT
	root = Tk()
	root.title(_TITLE)
	canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')
	canv.pack()

	controller = Controller(eventManager)
	view = View(eventManager, canv)
	game = Game(eventManager)

	for c in 'qwertyuiopasdfghjklzxcvbnm':
		root.bind('<{}>'.format(c), lambda e, c=c: eventManager.Post(KeyPressedEvent(c)))  # c=c IS IMPORTANT!
	root.bind('<Escape>', lambda e: eventManager.Post(QuitEvent(root)))
	root.bind('<Control-c>', lambda e: eventManager.Post(QuitEvent(root)))

	thread = Thread(target=controller.Run, args=(_FPS,))
	thread.daemon = True
	thread.start()

	print('Going tk.mainloop...')
	root.mainloop()
	eventManager.Post(QuitEvent(None))

main()

# try:
# 	main()
# except Exception as ex:
# 	print('Quit with exception:\n\t{}'.format(ex))
# else:
# 	print('Successfully quit.')
