import pygame
from common import *


class Entity(pygame.sprite.Sprite):

	def getRect(self):
		return pygame.rect.Rect(list(self.pos), self.surface.get_size)

	def render(self, screen):
		# if self.isVisible:
		screen.blit(self.surface, list(self.pos))
