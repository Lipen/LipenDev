import pygame
from pygame.locals import *
from common import *
from Point import Point
from Vector2 import Vector2
from Cell import Cell
from Text import Text
from Field import Field

# TODO: <Mouse> class.

FPS = 60
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
CELL_WIDTH = 20
CELL_HEIGHT = CELL_WIDTH
CELL_AMOUNT_X = SCREEN_WIDTH // CELL_WIDTH
CELL_AMOUNT_Y = SCREEN_HEIGHT // CELL_HEIGHT
SCREEN_WIDTH = CELL_WIDTH * CELL_AMOUNT_X
SCREEN_HEIGHT = CELL_HEIGHT * CELL_AMOUNT_Y

EXIT_KEYS = [K_ESCAPE]
MOD_KEYS = [K_LSHIFT, K_RSHIFT, K_LALT, K_RALT, K_LCTRL, K_RCTRL]


def blurSurf(surface, amt):
	"""
	Blur the given surface by the given 'amount'.  Only values 1 and greater
	are valid.  Value 1 = no blur.
	"""
	if amt < 1.0:
		raise ValueError("Arg 'amt' must be greater than 1.0, passed in value is %s" % amt)
	# scale = 1.0/float(amt)
	# surf_size = surface.get_size()
	# scale_size = (int(surf_size[0]*scale), int(surf_size[1]*scale))
	# surf = pygame.transform.smoothscale(surface, scale_size)
	# surf = pygame.transform.smoothscale(surf, surf_size)
	surf = pygame.transform.rotozoom(surface, 0, 1./amt)
	surf = pygame.transform.rotozoom(surf, 0, amt)
	return surf


def calculateIntensity(r):
	return (8*CELL_WIDTH) / r**(1.8)
Cell.calculateIntensity = calculateIntensity


# @Override
def getRandomColor(name=None):
	if name:
		return random.choice([Color(c) for c in pygame.color.THECOLORS if name in c])
	else:
		return random.choice(list(pygame.color.THECOLORS.values()))


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

				# Left click switches cell to be source or not
				source = self.field.getCellFromPoint(pos)
				source.click()

			elif type == MOUSEBUTTONDOWN and event.button == 3:
				pos = Point(*event.pos)

				print('MouseRightClick at {}'.format(pos))

				# Right click turns off nearest sources
				n, m = pos.x//CELL_WIDTH, pos.y//CELL_HEIGHT
				for i in range(n-3, n+4):
					for j in range(m-3, m+4):
						cell = self.field.getCellFromNumber(i, j)
						if cell is not None:
							if cell.active:
								cell.click()

			elif type == KEYDOWN:
				Key = event.key  # Just Integer
				Pressed = pygame.key.get_pressed()  # list of pressed (bits) ([0,1,0,1,0,0...] - #1 and #3 are pressed)
				Mod = pygame.key.get_mods()  # Mod list
				Name = pygame.key.name(Key)  # Name <- Int(Key)

				if Key in EXIT_KEYS:
					pygame.quit()
					self.RUNNING = False
					return

				# DEBUG:
				# if Key == K_SPACE:
				# 	for c in [cell.color for cell in self.field.cells]:
				# 		print(c)

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
			# self.screen.fill(pygame.Color('black'))  # FIXME: Isn't neccessary
			for cell in self.field.cells:
				cell.render(self.screen)
			self.screen.blit(blurSurf(self.screen, 4), (0, 0))
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
