import pygame
from pygame.locals import *
from common import *
from Point import Point

# TODO: <Mouse> class.
# TODO: Inside Cell.update: check if mouse inside cell and .. light..

FPS = 60
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
CELL_WIDTH = 10
CELL_HEIGHT = CELL_WIDTH
CELL_AMOUNT_X = SCREEN_WIDTH // CELL_WIDTH
CELL_AMOUNT_Y = SCREEN_HEIGHT // CELL_HEIGHT
SCREEN_WIDTH = CELL_WIDTH * CELL_AMOUNT_X
SCREEN_HEIGHT = CELL_HEIGHT * CELL_AMOUNT_Y

EXIT_KEYS = [K_ESCAPE]
MOD_KEYS = [K_LSHIFT, K_RSHIFT, K_LALT, K_RALT, K_LCTRL, K_RCTRL]


# Override
def getRandomColor(name=None):
	if name:
		return random.choice([Color(c) for c in pygame.color.THECOLORS if name in c])
	else:
		return random.choice(list(pygame.color.THECOLORS.values()))


def calculateIntensity(r):
	return (8*CELL_WIDTH) / r**(1.85)


class Entity(pygame.sprite.Sprite):

	def getRect(self):
		return pygame.rect.Rect(list(self.pos), self.surface.get_size)

	def render(self, screen):
		# if self.isVisible:
		screen.blit(self.surface, list(self.pos))


class Text(Entity):

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

	def __init__(self, pos=None, text='<DefaultString>', color=(255, 255, 255), colorBackground=None, font=None):
		# TODO: <Text class> Implement isVisible
		if pos is None:
			self.pos = Point(0, 0)
		else:
			self.pos = pos
		if font is None:
			self.font = LightSim.font_monospace
		else:
			self.font = font
		self.text = str(text)
		self.color = color
		self.colorBackground = colorBackground
		self.update()

	def setText(self, text):
		self.text = str(text)
		self.update()

	def centerTowardsEntity(self, ent, isX=True, isY=True):
		# FIXME: Move to <Entity> class?
		R = ent.getRect()
		r = self.getRect()
		if isX:
			self.x += R.centerx - r.centerx
		if isY:
			self.y += R.centery - r.centery

	def update(self):
		self.surface = self.font.render(self.text, True, self.color, self.colorBackground)


class Cell(Entity):

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

	def __init__(self, x, y, width, height, color=None, activecolor=(255, 255, 255, 255)):
		if color is None:
			self.color = getRandomColor()
		else:
			self.color = color
		self.pos = Point(x, y)
		self.width, self.height = width, height
		self.previousColor = self.color
		self.activecolor = activecolor
		self.active = False
		self.intensity = 0
		self.surface = pygame.Surface((width, height))
		self.surface.fill(Color('black'))  # is this line neccessary?..

	def isContainsPoint(self, p):
		return (self.x < p.x <= self.x+self.width) and (self.y < p.y <= self.y+self.height)

	def getCenter(self):
		return Point(self.x + self.width/2, self.y + self.height/2)

	def getNumber(self):
		return '{}:{}'.format(round(self.x/self.width), round(self.y/self.height))

	def getDistanceBetweenCells(c1, c2):
		return Point.getDistanceBetweenPoints(c1.getCenter(), c2.getCenter())

	def update(self):
		if self.active:
			if self.activecolor != self.previousColor:
				self.surface.fill(self.activecolor)
				self.previousColor = self.activecolor
		else:
			newcolor = tuple(map(lambda z: min(255, z*self.intensity), self.color[:3])) + (255, )
			if newcolor != self.previousColor:
				self.surface.fill(newcolor)
				self.previousColor = newcolor


class Field:

	"""Field class..
	"""

	def __init__(self, n, m, cellw, cellh):
		self.n, self.m = n, m
		self.cellsGrid = [[Cell(i*cellw, j*cellh, cellw, cellh, (153, 153, 153, 255)) for j in range(m)] for i in range(n)]
		self.cells = [cell for row in self.cellsGrid for cell in row]
		print('Total cells: {}'.format(len(self.cells)))

	def getCellFromPoint(self, p):
		for cell in self.cells:
			if cell.isContainsPoint(p):
				return cell

	def update(self):
		# t = time.time()
		for cell in self.cells:
			cell.update()
		# print('TimeIt: Field.update() - {:.3f} ms.'.format(1000*(time.time() - t)))


class LightSim:

	"""LightSim..
	"""

	def __init__(self):
		print('init started')

		pygame.init()
		# pygame.mouse.set_visible(False)
		pygame.display.set_caption('lightsimPygame')
		self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), 0, 32)
		self.clock = pygame.time.Clock()
		self.RUNNING = True

		self.font_monospace = pygame.font.SysFont("monospace", 16)
		self.font_main = pygame.font.SysFont("Verdana", 34, True)
		self.font_fps = pygame.font.SysFont("Verdana", 16, True)

		self.text_fps = Text(pos=Point(16, 16), color=(150, 255, 0), font=self.font_fps)
		self.field = Field(CELL_AMOUNT_X, CELL_AMOUNT_Y, CELL_WIDTH, CELL_HEIGHT)

		print('init ended succesfully')

	def events(self, Events):
		for event in Events:
			type = event.type
			if type == QUIT:
				pygame.quit()
				self.RUNNING = False
				return

			elif type == MOUSEMOTION:
				pos = Point(*event.pos)

				# print('Motion at <{}>'.format(pos))

			elif type == MOUSEBUTTONDOWN and event.button == 1:
				pos = Point(*event.pos)

				print('MouseLeftClick at {}'.format(pos))

				for source in self.field.cells:
					if source.isContainsPoint(pos):
						if source.active:
							source.active = False
							source.intensity -= 1
							for cell in self.field.cells:
								if cell is not source:
									r = Cell.getDistanceBetweenCells(cell, source)
									if r > 0:
										cell.intensity -= calculateIntensity(r)
						else:
							source.active = True
							source.intensity += 1
							for cell in self.field.cells:
								if cell is not source:
									r = Cell.getDistanceBetweenCells(cell, source)
									if r > 0:
										cell.intensity += calculateIntensity(r)
						break

			elif type == KEYDOWN:
				Key = event.key  # Just Integer
				Pressed = pygame.key.get_pressed()  # list of pressed (bits) ([0,1,0,1,0,0...] - #1 and #3 are pressed)
				Mod = pygame.key.get_mods()  # Mod list
				Name = pygame.key.name(Key)  # Name <- Int(Key)

				if Key in EXIT_KEYS:
					pygame.quit()
					self.RUNNING = False
					return

				if Mod & KMOD_CTRL and Key not in MOD_KEYS:
					print('Pressed <{}> (#{}) with pressed ctrl.'.format(Name, Key))
				if Mod & KMOD_SHIFT and Key not in MOD_KEYS:
					print('Pressed <{}> (#{}) with pressed shift.'.format((Name.upper()), Key))
				if Mod & KMOD_ALT and Key not in MOD_KEYS:
					print('Pressed <{}> (#{}) with pressed alt.'.format(Name, Key))

	def loop(self):
		print('loop start')
		while self.RUNNING:
			self.events(pygame.event.get())
			if not self.RUNNING:
				break

			elapsed = self.clock.tick(FPS)
			fps = 1000. / elapsed
			# seconds = pygame.time.get_ticks() / 1000.

			# PROCESSING
			self.text_fps.setText('FPS: {}'.format(round(fps)))
			self.field.update()

			# RENDERING
			# self.screen.fill(pygame.Color('black'))
			for cell in self.field.cells:
				cell.render(self.screen)
			self.text_fps.render(self.screen)

			pygame.display.flip()


lightsim = LightSim()
try:
	lightsim.loop()
except Exception as e:
	# print('Something gone wrong:\n{}'.format(e))
	print('Something gone wrong:')
	raise e
else:
	print('Succesfully ended.')
