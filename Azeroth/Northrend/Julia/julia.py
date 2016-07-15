import numpy
import os

w, h = 200, 200
STEP_N = 8
PRE_CLEAN = True

bound = 1
re_min, re_max = -bound, bound
im_min, im_max = -bound, bound

real_range = numpy.arange(re_min, re_max, (re_max - re_min) / w)
imag_range = numpy.arange(im_min, im_max, (im_max - im_min) / h)


if (os.path.isdir('_tmp')):
	print('_tmp folder already exist')
	if PRE_CLEAN:
		print('Pre-cleaning ...')
		os.system('rm -f _tmp/*.pgm')
else:
	os.mkdir('_tmp')


frame = 0

for c_im in numpy.arange(0.0, 1.5, 0.01):
	c = complex(0, c_im)
	frame += 1

	print('dbg: Frame #{} ...'.format(frame))

	fo = open('_tmp/julia_{:03d}.pgm'.format(frame), 'w')
	fo.write('P2\n# Julia set image :: frame {}\n{} {}\n255\n'.format(frame, w, h))

	for im in imag_range:
		for re in real_range:
			z = complex(re, im)
			n = 255

			while abs(z) < 2 and n >= STEP_N:
				z = z*z + c
				n -= STEP_N

			fo.write('{} '.format(n))

		fo.write('\n')

	fo.close()


os.system('convert -delay 10 _tmp/*.pgm julia_.gif')
os.system('convert julia_.gif -coalesce ( -clone -2-1 ) -quiet -layers OptimizePlus -loop 0 julia.gif')

# os.system('rm -rf _tmp/*.pgm')
# os.rmdir('_tmp')
