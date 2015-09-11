from numpy import sin, pi, linspace
import matplotlib.pyplot as plt
from os import system as sys

# Varies delta over frames. (const a:b)

folder = '_tmpLissajousCurves/'
sys('mkdir ' + folder.rstrip('/'))

t = linspace(-pi, pi, 300)  # 200 for medium quality
N = 64  # steps (frames?)
a = 3
b = 5

for i in range(N+1):
	delta = i/N * 2*pi
	x = sin(a * t + delta)
	y = sin(b * t)

	plt.clf()
	plt.plot(x, y)

	plt.savefig(folder+'{:0>2d}.png'.format(i), transparent=True)

sys('convert -delay 10 -dispose previous {} -resize 75% -loop 0 LissajousCurve_animation.gif'.format(' '.join(folder+'{:0>2d}.png'.format(s) for s in range(N))))

if folder:
	sys('rmdir {} /S /Q'.format(folder.rstrip('/')))  # beware!
