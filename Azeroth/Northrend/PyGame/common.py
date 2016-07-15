import math
import time
import random
import re


def clock_yield():
	pt = 0
	while 1:
		t = time.time()
		yield t - pt
		pt = t


def getRandomColor():
	return '#{:06X}'.format(random.randint(0, 16777215))


def sign(x):
	return 1 if x > 0 else -1 if x < 0 else 0


def fround(f, precision=3):
	return format(f, '.{}f'.format(precision)).rstrip('0').rstrip('.')


def parseColor(color):
	# return re.findall('[0-9a-fA-F]{2}', color)
	return color[1:3], color[3:5], color[5:]
