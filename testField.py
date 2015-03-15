from tkinter import *
from common import *
from Vector2 import Vector2
from Point import Point

# TODO: Replace Label inside cell to canvas's polygons (rects to start with)
# FIXME: Mouse class doesn't need anymore..

_TITLE = 'testField'
_CELL_WIDTH = 64
_CELL_HEIGHT = 64
_CELL_AMOUNT_X = 12
_CELL_AMOUNT_Y = 9
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
		self.activecolor = activecolor
		self.label = Label(root, width=width, height=height, background=color)
		self.label.place(x=x, y=y)
		self.label.bind('<Enter>', lambda e: self.mouseEnter())
		self.label.bind('<Leave>', lambda e: self.mouseLeave())
		self.active = False

	def isContainsPoint(self, p):
		return (self.pos.x <= p.x <= self.pos.x+self.width) and (self.pos.y <= p.y <= self.pos.y+self.height)

	def getNumber(self):
		return '{}:{}'.format(round(self.pos.x/self.width), round(self.pos.y/self.height))

	def mouseEnter(self):
		if not self.active:
			self.label['background'] = self.activecolor
			self.active = True
		else:
			print('Double enter.. weird')

	def mouseLeave(self):
		if self.active:
			self.label['background'] = self.color
			self.active = False
		else:
			print('Double leave.. weird')

	def update(self):
		pass


class Field:

	"""Field class for canvas with grid
	"""

	def __init__(self, n, m, cellw, cellh):
		self.n, self.m = n, m
		self.cellsGrid = [[Cell(i*cellw, j*cellh, cellw, cellh, '#999999') for j in range(m)] for i in range(n)]
		self.cells = [cell for row in self.cellsGrid for cell in row]
		print(len(self.cells))

	def getCellFromPoint(self, p):
		for cell in self.cells:
			if cell.isContainsPoint(p):
				return cell

	def update(self):
		for cell in self.cells:
			cell.update()


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
label_fps = Label(root, bg='#dddddd', fg='#000000', font='sans 20')
label_fps.place(x=16, y=16, width=64, height=64)

root.bind('<Escape>', Quit)
root.bind('<Control-c>', Quit)

root.after((lambda t: t if t > 0 else 1)(round(1000*_DELAY)), timer)
root.mainloop()
