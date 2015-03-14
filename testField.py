from tkinter import *
from common import *
from Vector2 import Vector2
from Point import Point

_TITLE = 'nxtsim'
_WIDTH = 768
_HEIGHT = 640
_FPS = 25
_DELAY = 1/_FPS  # seconds
_STARTTIME = time.time()


class Cell:

	def __init__(self, x, y, width, height, color=None, activecolor='#00FF33'):
		if color is None:
			color = getRandomColor()
		self.pos = Point(x, y)
		self.width, self.height = width, height
		self.label = Label(root, width=width, height=height, background=color, activebackground=activecolor)
		self.label.place(x=x, y=y)

	def isContainsPoint(self, p):
		return self.pos.x <= p.x <= self.pos.x+self.width and self.pos.y <= p.y <= self.pos.y+self.height


class Field:

	"""Field class for canvas with grid
	"""

	def __init__(self, n, m, cellw, cellh):
		self.n, self.m = n, m
		self.cells = [[Cell(i*cellw, j*cellh, cellw, cellh, '#999999') for j in range(m)] for i in range(n)]

	def getCellFromPoint(self, p):
		for cell in self.cells:
			if cell.isContainsPoint(p):
				return cell

	def update(self):
		pass


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
root.geometry('{}x{}+420+60'.format(_WIDTH, _HEIGHT))
root.protocol('WM_DELETE_WINDOW', Quit)
root.resizable(False, False)
canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#333333')
canv.pack()
clock = clock_yield()
next(clock)  # get out initial zero
field = Field(6, 5, 128, 128)
label_fps = Label(root, bg='#dddddd', fg='#000000', font='sans 20')
label_fps.place(x=16, y=16, width=64, height=64)

root.bind('<Escape>', Quit)
root.bind('<Control-c>', Quit)

root.after((lambda t: t if t > 0 else 1)(round(1000*_DELAY)), timer)
root.mainloop()
