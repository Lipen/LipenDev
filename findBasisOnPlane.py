class Vector3:
	def __init__(self, x, y, z):
		self.x, self.y, self.z = x, y, z

	def multiplyScalar(v1, v2):
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z

def getp(kx, ky, kz, limit_low, limit_up):
	p = []
	for x in range(limit_low, limit_up+1):
		for y in range(limit_low, limit_up+1):
			for z in range(limit_low, limit_up+1):
				if kx*x+ky*y+kz*z == 0 and not x==y==z==0:
					v = Vector3(x, y, z)
					p.append(v)
	return p

def coefFormat(k, b=False):
	if k>0:
		if k!=1:
			if b:
				return str(k)
			else:
				return '+' + str(k)
		else:
			return ''
	elif k<0:
		if k!=-1:
			return str(k)
		else:
			return '-'
	else:
		return '0'

def main():
	kx, ky, kz = map(int, input('Enter plane`s coefs (kx*x + ky*y + kz*z = 0): ').split())
	print('Plane: {}x{}y{}z = 0'.format(coefFormat(kx, True), coefFormat(ky), coefFormat(kz)))
	s = input('Enter lower and upper limits: ')
	if s!='':
		limit_low, limit_up = sorted(list(map(int, s.split())))
	else:
		limit_low, limit_up = -3, 3
	print('Limits: from {} to {}'.format(limit_low, limit_up))
	p = getp(kx,ky,kz,limit_low,limit_up)

	b = []
	for v1 in p:
		for v2 in p:
			if v1!=v2:
				if Vector3.multiplyScalar(v1, v2)==0:
					b.append([v1, v2])

	import random
	basis = b[random.randint(0, len(b)-1)]
	print('Random basis: {{{}, {}, {}}} and {{{}, {}, {}}}'.format(basis[0].x, basis[0].y, basis[0].z, basis[1].x, basis[1].y, basis[1].z))

if __name__ == '__main__':
	main()
input('End...')
