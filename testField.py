from tkinter import *
from common import *
from Vector2 import Vector2
from Point import Point

_TITLE = 'nxtsim'
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

	def __init__(self):
		pass


class Cell:

	def __init__(self, x, y, width, height, color=None, activecolor='#00FF33'):
		if color is None:
			color = getRandomColor()
		self.pos = Point(x, y)
		self.width, self.height = width, height
		self.color = color
		self.activecolor = activecolor
		self.label = Label(root, width=width, height=height, background=color)
		self.label.place(x=x, y=y)
		self.active = False

	def isContainsPoint(self, p):
		return (self.pos.x <= p.x <= self.pos.x+self.width) and (self.pos.y <= p.y <= self.pos.y+self.height)

	def getNumber(self):
		return '{}:{}'.format(round(self.pos.x/self.width), round(self.pos.y/self.height))

	def update(self):
		if self.isContainsPoint(mouse.pos):
			if not self.active:
				self.label['background'] = self.activecolor
				self.active = True
		else:
			if self.active:
				self.label['background'] = self.color
				self.active = False


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


def MouseClick(event=None):
	p = Point(event.x_root-_LEFT, event.y_root-_TOP)
	cell = field.getCellFromPoint(p)
	print('Clicked at {} inside cell #{} with pos={}'.format(p, cell.getNumber(), cell.pos))


def MouseMotion(event=None):
	p = Point(event.x_root-_LEFT, event.y_root-_TOP)
	mouse.pos = p
	# print('Motion at {}'.format(mouse.pos))


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
canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#333333')
canv.pack()
mouse = Mouse()
clock = clock_yield()
next(clock)  # get out initial zero
field = Field(_CELL_AMOUNT_X, _CELL_AMOUNT_Y, _CELL_WIDTH, _CELL_HEIGHT)
label_fps = Label(root, bg='#dddddd', fg='#000000', font='sans 20')
label_fps.place(x=16, y=16, width=64, height=64)

root.bind('<Escape>', Quit)
root.bind('<Control-c>', Quit)
root.bind('<Button-1>', MouseClick)
root.bind('<Motion>', MouseMotion)

root.after((lambda t: t if t > 0 else 1)(round(1000*_DELAY)), timer)
root.mainloop()
