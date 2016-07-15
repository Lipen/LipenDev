from numpy import sin, pi, linspace
import matplotlib.pyplot as plt
from os import system as sys

# Varies a:b relation over frames. (const delta)

folder = '_tmpLissajousCurves/'
sys('mkdir ' + folder.rstrip('/'))

t = linspace(-pi, pi, 300)  # 200 for medium quality
delta = pi / 2

data = [[1, 1, 1]]
for j in range(1, 7+1):
	for i in range(1, j):
		data.append([i/j, i, j])
N = len(data)

# sort.. by increasing a:b
data.sort(key=lambda x: x[0])

for i in range(N):
	print('Processing {: >2d} from {}...'.format(i+1, N), end='', flush=True)
	_, a, b = data[i]
	x = sin(a * t + delta)
	y = sin(b * t)

	plt.clf()
	plt.plot(x, y)
	plt.title('{} : {}'.format(a, b), fontsize=20)
	plt.xlabel(r'$x$', fontsize=20)
	plt.ylabel(r'$y$', fontsize=20)
	plt.grid(True)

	plt.savefig(folder+'{:0>2d}.png'.format(i), transparent=True)
	print(' done!')
	# print('.', end='', flush=True)

print('Creating a gif...', end='', flush=True)
sys('convert -delay 30 -dispose previous {} -resize 75% -loop 0 LissajousCurve_animation.gif'.format(' '.join(folder+'{:0>2d}.png'.format(s) for s in range(N))))
print(' done!')

if folder:
	sys('rmdir {} /S /Q'.format(folder.rstrip('/')))
# sys('rm {}*.png'.format(folder))  # beware!

print('All done!')
