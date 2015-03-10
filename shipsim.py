from tkinter import *
import random

title = 'shipsim'
w = 800
h = 600
_FPS = 50.
_DELAY = round(1000. / _FPS)
_FPS = 1000./_DELAY
print('FPS = {}, DELAY = {}'.format(_FPS, _DELAY))

root = Tk()
root.title(title)
canv = Canvas(root, width=w, height=h, bg='#505050')


# class Vector2:
# eh


class Point:

	def __init__(self, x, y):
		self.x, self.y = x, y

	def __iter__(self):
		return iter([self.x, self.y])

	def applySpeed(self, vx, vy):
		self.x += vx
		self.y += vy


class Polygon:
	vx = 0
	vy = 0

	def __init__(self, p1, p2, p3, p4, fill='#5060BB', activefill='#A060BB'):
		self.points = [p1, p2, p3, p4]
		self.p1, self.p2, self.p3, self.p4 = self.points
		self.poly = canv.create_polygon(self.getCoords(), fill=fill, activefill=activefill)

	def setSpeed(self, vx, vy):
		'''
		pixels per second
		'''
		self.vx, self.vy = vx, vy

	def getCoords(self):
		'''
		list of coords of all polygon's points:
				[ p1.x, p1.y, ..., p4.x, p4.y ] #for 2-dim 4-point polygon
		'''
		# coords = []
		# for p in self.points:
		# 	coords.extend(p)
		# return coords
		return [c for p in self.points for c in p]

	def move(self):
		for p in self.points:
			p.applySpeed(self.vx/_DELAY, self.vy/_DELAY)
			# p.addVector(self.speedVector)

	def update(self):
		self.move()
		canv.coords(self.poly, self.p1.x, self.p1.y, self.p2.x, self.p2.y, self.p3.x, self.p3.y, self.p4.x, self.p4.y)

	def newRandomPolygon():
		p1 = Point(random.uniform(0, w), random.uniform(0, h))
		p2 = Point(random.uniform(0, w), random.uniform(p1.y, h))
		p3 = Point(random.uniform(max(p1.x, p2.x), w), random.uniform(0, h))
		p4 = Point(random.uniform(max(p1.x, p2.x), w), random.uniform(p3.y, h))
		return Polygon(p1, p2, p3, p4, fill='#{:X}{:X}{:X}'.format(random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)))


def tick(event=None):
	mypoly.update()


def timer():
	tick()
	# print('timer <3')
	#
	root.after(_DELAY, timer)  # repeat

###
mypoly = Polygon.newRandomPolygon()
mypoly.setSpeed(1, 1)
root.bind('z', tick)
root.after(_DELAY, timer)  # start
# Polygon.getCoords()  # eh???
###

canv.pack()
root.mainloop()
