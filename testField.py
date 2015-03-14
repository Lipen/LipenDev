from tkinter import *
from common import *

_TITLE = 'nxtsim'
_WIDTH = 800
_HEIGHT = 608
_FPS = 25.
_DELAY = 1./_FPS  # seconds
_STARTTIME = time.time()

root = Tk()
root.title(_TITLE)
canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')
label_fps = Label(root, bg='#cccccc', fg='#000000', font='sans 20')


class Cell:

	def __init__(self, x, y, width, height):
		self.pos = Point(x, y)


class Field:

	"""Field class for canvas with grid
	"""

	def __init__(self, n, m, cellx, celly):
		self.n, self.m = n, m
		self.cellx, self.celly = cellx, celly

	def update(self):
		pass


def tick():
	_DELAY = next(clock)
	label_fps['text'] = round(1./_DELAY) if _DELAY > 0 else 1
	field.update()


def timer():
	tick()
	root.after((lambda t: t if t > 0 else 1)(round(1000*_DELAY)), timer)


def QuitDestroy(event=None):
	root.destroy()
	root.quit()
	print('Destroyed!')

field = Field(25, 19, 32, 32)
clock = clock_yield()
next(clock)  # get out initial zero

root.bind('<Escape>', QuitDestroy)
root.bind('<Control-c>', QuitDestroy)

canv.pack()
label_fps.place(x=16, y=16, width=64, height=64)
root.after((lambda t: t if t > 0 else 1)(round(1000*_DELAY)), timer)
root.mainloop()
