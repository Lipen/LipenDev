from tkinter import *
from common import *
from Vector2 import Vector2
from Point import Point

# TODO: Replace Label inside cell to canvas's polygons (rects to start with)
# FIXME: Mouse class doesn't need anymore..

_TITLE = 'testField'
_CELL_WIDTH = 48  # 64
_CELL_HEIGHT = 48  # 64
_CELL_AMOUNT_X = 16  # 12
_CELL_AMOUNT_Y = 12  # 9
_WIDTH = _CELL_WIDTH * _CELL_AMOUNT_X
_HEIGHT = _CELL_HEIGHT * _CELL_AMOUNT_Y
_LEFT = 420
_TOP = 60
_FPS = 25
_DELAY = 1/_FPS  # seconds
_STARTTIME = time.time()


class Mouse:

	pos = Point(-1, -1)


class Cell:

	"""Base cell class
	"""

	@property
	def x(self):
		return self.pos.x

	@x.setter
	def x(self, value):
		self.pos.x = value

	@property
	def y(self):
		return self.pos.y

	@y.setter
	def y(self, value):
		self.pos.y = value

	def __init__(self, x, y, width, height, color=None, activecolor='#00FF33'):
		if color is None:
			color = getRandomColor()
		self.pos = Point(x, y)
		self.width, self.height = width, height
		self.color = color
		self.previousColor = color
		self.activecolor = activecolor
		self.label = Label(root, width=width, height=height, background=color)
		self.label.place(x=x, y=y)
		self.active = False
		self.intensity = 0
		# self.label.bind('<Enter>', self.mouseEnter())
		# self.label.bind('<Leave>', self.mouseLeave())

	def isContainsPoint(self, p):
		return (self.pos.x <= p.x <= self.pos.x+self.width) and (self.pos.y <= p.y <= self.pos.y+self.height)

	def getCenter(self):
		return Point(self.x + self.width/2, self.y + self.height/2)

	def getNumber(self):
		return '{}:{}'.format(round(self.pos.x/self.width), round(self.pos.y/self.height))

	def mouseClick(self, event=None):
		pass

	def mouseEnter(self, event=None):
		if not self.active:
			# self.label['background'] = self.activecolor
			self.active = True
			self.intensity = 1
			for cell in field.cells:
				if cell is not self:
					r = Cell.getDistanceBetweenCells(cell, self)
					if r > 0:
						cell.intensity += calculateIntensity(r)
		else:
			print('Double enter.. weird')

	def mouseLeave(self, event=None):
		if self.active:
			# self.label['background'] = self.color
			for cell in field.cells:
				if cell is not self:
					r = Cell.getDistanceBetweenCells(cell, self)
					if r > 0:
						cell.intensity -= calculateIntensity(r)
			self.intensity = 0
			self.active = False
		else:
			print('Double leave.. weird')

	def getDistanceBetweenCells(c1, c2):
		return Point.getDistanceBetweenPoints(c1.getCenter(), c2.getCenter())

	def updateColor(self):
		if self.active:
			self.label['background'] = self.activecolor
			self.previousColor = self.activecolor
		else:
			# t = time.time()
			r, g, b = map(lambda z: min(255, round(int(z, 16)*self.intensity)), parseColor(self.color))
			color = '#{:02X}{:02X}{:02X}'.format(r, g, b)
			if color != self.previousColor:
				self.label['background'] = color
				self.previousColor = color
			# self.label['background'] = '#{:02X}{:02X}{:02X}'.format(r, g, b)
			# print('DBG: DELAY = {:.9f}'.format(time.time() - t))

	def update(self):
		self.updateColor()


class Field:

	"""Field class for canvas with grid
	"""

	def __init__(self, n, m, cellw, cellh):
		self.n, self.m = n, m
		self.cellsGrid = [[Cell(i*cellw, j*cellh, cellw, cellh, '#bbbbbb') for j in range(m)] for i in range(n)]
		self.cells = [cell for row in self.cellsGrid for cell in row]
		print(len(self.cells))

	def getCellFromPoint(self, p):
		for cell in self.cells:
			if cell.isContainsPoint(p):
				return cell

	def update(self):
		t = time.time()
		for cell in self.cells:
			cell.update()
		print('DBG: DELAY2 = {:.9f}'.format(time.time() - t))

		# for c1 in self.cells:
		# 	if not c1.active:
		# 		I = 0
		# 		for c2 in self.cells:
		# 			if c1 is not c2 and c2.active:
		# 				r = Cell.getDistanceBetweenCells(c1, c2)
		# 				if r > 0:
		# 					I += 16 / r  # FIXME: r^2?
		# 		c1.intensity = I


def tick():
	_DELAY = next(clock)
	label_fps['text'] = round(1./_DELAY) if _DELAY > 0 else 1
	field.update()


def timer():
	tick()
	root.after((lambda t: t if t > 0 else 1)(round(1000*_DELAY)), timer)


def Quit(event=None):
	root.quit()
	print('Quit!')


root = Tk()
root.title(_TITLE)
root.geometry('{}x{}+{}+{}'.format(_WIDTH, _HEIGHT, _LEFT, _TOP))
root.protocol('WM_DELETE_WINDOW', Quit)
root.resizable(False, False)
canvas = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#333333')
canvas.pack()
mouse = Mouse()
clock = clock_yield()
next(clock)  # get out initial zero
field = Field(_CELL_AMOUNT_X, _CELL_AMOUNT_Y, _CELL_WIDTH, _CELL_HEIGHT)
for cell in field.cells:
	cell.label.bind('<Enter>', cell.mouseEnter)
	cell.label.bind('<Leave>', cell.mouseLeave)
label_fps = Label(root, bg='#dddddd', fg='#000000', font='sans 20')
label_fps.place(x=16, y=16, width=64, height=64)

root.bind('<Escape>', Quit)
root.bind('<Control-c>', Quit)

root.after((lambda t: t if t > 0 else 1)(round(1000*_DELAY)), timer)
root.mainloop()
