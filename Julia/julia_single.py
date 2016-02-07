import numpy

w, h = 200, 200

bound = 1.5
re_min, re_max = -bound, bound
im_min, im_max = -bound, bound

c = complex(0.0, .65)

real_range = numpy.arange(re_min, re_max, (re_max - re_min) / w)
imag_range = numpy.arange(im_min, im_max, (im_max - im_min) / h)

fo = open('julia_single.pgm', 'w')
fo.write('P2\n# Julia set image\n{} {}\n255\n'.format(w, h))

for im in imag_range:
	for re in real_range:
		z = complex(re, im)
		n = 255

		while abs(z) < 10 and n >= 5:
			z = z*z + c
			n -= 5

		fo.write('{} '.format(n))

	fo.write('\n')

fo.close()
