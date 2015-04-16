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

# TODO: MVC
# TODO: (_DELAY/1000.) ===> "_dt" from clock
# TODO: <Link> class for linking some immutable objects with bases

_TITLE = 'shipsim'
_WIDTH = 800
_HEIGHT = 600
_FPS = 50
_DELAY = round(1000. / _FPS)
# _FPS = 1000./_DELAY
# print('FPS = {}, DELAY = {}'.format(_FPS, _DELAY))
_MASS = 3000000  # kg


class Polygon:

	"""Base 2D polygon class.
	"""

	angle = 0
	vel = Vector2(0, 0)
	acc = Vector2(0, 0)
	angularVel = 0
	angularAcc = 0

	def __init__(self, points, fill='#5060BB', activefill='#99dd33', spawn=False):
		self.points = list(points)  # is it necessary?..
		if spawn:
			self.poly = canv.create_polygon(self.getCoords(), fill=fill, activefill=activefill)

	def getArea(self):
		return 0.5 * abs(sum(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y for i in range(len(self.points))))

	def getCenter(self):
		A = self.getArea()
		return Point(
			1/6/A*sum((self.points[i-1].x+self.points[i].x)*(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y) for i in range(len(self.points))),
			1/6/A*sum((self.points[i-1].y+self.points[i].y)*(self.points[i-1].x*self.points[i].y-self.points[i].x*self.points[i-1].y) for i in range(len(self.points))))

	def setSpeed(self, vx, vy):
		"""Pixels per second
		"""
		self.vel.x = vx
		self.vel.y = vy

	def setAccel(self, ax=None, ay=None):
		"""Pixels per second^2
		"""
		if ax is not None:
			self.acc.x = ax
		if ay is not None:
			self.acc.y = ay

	def setAngularSpeed(self, av):
		"""Radians per second
		"""
		self.angularVel = av

	def setAngularAccel(self, aa):
		"""Radians per second^2
		"""
		self.angularAcc = aa

	def getCoords(self):
		"""List of coords of all polygon's points
		"""
		return [c for p in self.points for c in p]

	def accel(self):
		self.vel += self.acc * (_DELAY/1000.)

	def move(self):
		for p in self.points:
			p.addVector(self.vel * (_DELAY/1000.))

	def rotate(self):
		self.angularVel += self.angularAcc * (_DELAY/1000.)
		# da = self.angularVel/_DELAY
		da = self.angularVel * (_DELAY/1000.)
		if da != 0:
			self.angle += math.degrees(da)
			c = self.getCenter()
			for p in self.points:
				r = Point.getDistanceBetweenPoints(c, p)
				a = Point.getAngleBetweenPoints(c, p) + da
				p.x = c.x + r * math.cos(a)
				p.y = c.y + r * math.sin(a)

	def update(self):
		self.accel()
		self.move()
		self.rotate()
		canv.coords(self.poly, *self.getCoords())

		print('Angle={}'.format(self.angle))
		# print('v.x={:.1f},v.y={:.1f},a.x={:.1f},a.y={:.1f},aV={:.1f},aA={:.3f}'.format(self.vel.x, self.vel.y, self.acc.x, self.acc.y, self.angularVel, self.angularAcc))


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

	game = Game(eventManager)
	view = View(eventManager)
	controller = Controller(eventManager)

	# //STUFF INIT
	root = Tk()
	root.title(_TITLE)
	canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')
	label_fps = Label(root, bg='#cccccc', fg='#000000', font='sans 20')

	clock = clock_yield()
	# ship = Ship([Point(50, 0), Point(75, 0), Point(75, 25), Point(125, 25), Point(125, 0), Point(150, 0), Point(200, 100), Point(175, 150), Point(125, 150), Point(125, 125), Point(100, 100), Point(75, 125), Point(75, 150), Point(25, 150), Point(0, 100)], Point(300, 200))
	# ship.addEngine(Point(100, 100), 0, 5000000)  # Main
	# Wird signs... Should be vise-versa (left+35, right-35)..
	# ship.addEngine(Point(50, 150), -35, 1000000)  # Left (-->)
	# ship.addEngine(Point(150, 150), 35, 1000000)  # Right (<--)

	# -------

	# def QuitDestroy(event=None):
	# 	print('Quitting...')
	# 	eventManager.Post(QuitEvent(root))
	# 	# root.destroy()
	# 	# root.quit()

	# keyboardController = KeyboardController(eventManager)
	# root.bind('z', tick)
	# root.bind('<Escape>', QuitDestroy)
	# root.bind('<w>', lambda e: ship.toggleEngine(0))
	# root.bind('<s>', lambda e: ship.stopEngine(0))
	# root.bind('<a>', lambda e: ship.toggleEngine(2))
	# root.bind('<d>', lambda e: ship.toggleEngine(1))
	# root.bind('<space>', lambda e: ship.stopEngine((0, 1, 2)))
	# # root.bind('<q>', lambda e: ship.setAngularAccel(-0.2))
	# # root.bind('<e>', lambda e: ship.setAngularAccel(0.2))
	# # root.bind('<KeyRelease-a>', lambda e: ship.setAccel(ax=0))
	root.bind('<w>', lambda e: eventManager.Post(KeyPressedEvent('w')))
	root.bind('<Escape>', lambda e: eventManager.Post(QuitEvent(root)))
	root.bind('<Control-c>', lambda e: eventManager.Post(QuitEvent(root)))

	# -------
	# timeController = TimeController(eventManager)

	def tick(event=None):
		# # ship.update()
		# ev = TickUp
		# eventManager.Post()
		label_fps['text'] = round(1./next(clock))

	def timer():
		tick()
		root.after(_DELAY, timer)  # repeat

	root.after(_DELAY, timer)  # start
	# timeController.Run(_FPS)

	# controller.Run(_FPS)
	# root.after(_DELAY, lambda: controller.Run(_FPS))
	# thread = Thread(target=controller.Run, args=(_FPS,))
	# thread.start()
	Thread(target=controller.Run, args=(_FPS,)).start()

	# -------
	canv.pack()
	label_fps.place(x=16, y=16, width=64, height=64)
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
