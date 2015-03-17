import pygame
from pygame.locals import *
from common import *
from Point import Point
from Vector2 import Vector2

# TODO: Inherit Text and other widgets in future from <Entity> class

FPS = 60
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

EXIT_KEYS = [K_ESCAPE]
MOD_KEYS = [K_LSHIFT, K_RSHIFT, K_LALT, K_RALT, K_LCTRL, K_RCTRL]


class Text(pygame.sprite.Sprite):

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
		R = ent.getRect()
		r = self.getRect()
		if isX:
			self.x += R.centerx - r.centerx
		if isY:
			self.y += R.centery - r.centery

	def getRect(self):
		return pygame.rect.Rect(self.pos, self.surface.get_size)

	def update(self):
		self.surface = self.font.render(self.text, True, self.color, self.colorBackground)

	def render(self, screen):
		# if self.isVisible:
		screen.blit(self.surface, list(self.pos))


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

		for k, v in pygame.color.THECOLORS.items():
			print('{: >20} : {}'.format(k, v))

		print('init ended succesfully')

	def events(self, Events):
		for event in Events:
			type = event.type
			if type == QUIT:
				pygame.quit()
				self.RUNNING = False
				return

			elif type == MOUSEMOTION:
				pos = event.pos

				# print('Motion at <{}>'.format(pos))

			elif type == MOUSEBUTTONDOWN and event.button == 1:
				pos = event.pos

				print('MouseLeftClick at <{}>'.format(pos))

			elif type == KEYDOWN:
				Key = event.key  # Just Integer
				Pressed = pygame.key.get_pressed()  # list of pressed (bits) ([0,1,0,1,0,0] - #1 and #3 are pressed)
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

			# RENDERING
			self.screen.fill(pygame.Color('black'))
			# self.screen.fill((0, 0, 0))
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
