from tkinter import *
from common import *

_TITLE = 'nxtsim'
_WIDTH = 800
_HEIGHT = 600
_FPS = 1000
_DELAY = round(1000./_FPS)
_STARTTIME = time.time()

root = Tk()
root.title(_TITLE)
canv = Canvas(root, width=_WIDTH, height=_HEIGHT, bg='#505050')
label_fps = Label(root, bg='#cccccc', fg='#000000', font='sans 20')


class NXT:

	"""NXT sim
	"""

	# @property
	# def omega(self):
	# 	return self._omega

	# @omega.setter
	# def omega(self, value):
	# 	self._omega = value

	@property
	def u(self):
		return self._u

	@property
	def theta(self):
		return self._theta

	@property
	def I(self):
		return self._I

	def __init__(self, Lp, R, J, voltage, ke, kt):
		self.Lp = Lp
		self.J = J
		self.R = R
		self.voltage = voltage
		self.ke = ke
		self.kt = kt
		self.omega = 0
		self._u = 0
		self._I = 0
		self._theta = 0

	def force(self, power):
		self._u = (100 if power > 100 else -100 if power < -100 else power)*self.voltage/100

	def update(self):
		self._I += (self.u - self.ke*self.omega - self.R*self.I)/self.Lp*_DELAY/1000.
		self.omega += (self.I*self.kt/self.J)*_DELAY/1000.
		self._theta += self.omega*_DELAY/1000.


def tick():
	_DELAY = next(clock)
	_FPS = 1./_DELAY
	label_fps['text'] = round(_FPS)
	nxt.update()
	print('Theta = {}, I={},omega={},u={}'.format(nxt.theta, nxt.I, nxt.omega, nxt.u))

	nxt.force(100*math.sin(time.time()-_STARTTIME))


def timer():
	tick()
	root.after((lambda t: t if t > 0 else 1)(round(_DELAY/1000.)), timer)


def QuitDestroy(event=None):
	print('Quitting...')
	root.destroy()
	root.quit()
	print('Destroyed.')

nxt = NXT(Lp=0.0047, R=4.5, J=0.002513808, voltage=8.0, ke=0.5, kt=0.5)
clock = clock_yield()
next(clock)  # get out initial zero

# nxt.force(100)
# root.after(1000, lambda: nxt.force(0))

root.bind('<Escape>', QuitDestroy)
root.bind('<Control-c>', QuitDestroy)

canv.pack()
label_fps.place(x=16, y=16, width=64, height=64)
root.after(_DELAY, timer)
root.mainloop()
