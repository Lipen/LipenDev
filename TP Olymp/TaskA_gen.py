# def gen_primes():
# 	# Sieve of Eratosthenes
# 	# Code by David Eppstein, UC Irvine, 28 Feb 2002
# 	# http://code.activestate.com/recipes/117119/
# 	D = {}
# 	q = 2

# 	while True:
# 		if q not in D:
# 			yield q
# 			D[q * q] = [q]
# 		else:
# 			for p in D[q]:
# 				D.setdefault(p + q, []).append(p)
# 			del D[q]

# 		q += 1

import itertools


# def gen_primes(start=2):
def gen_primes():
	# if start <= 2:
	yield 2
	D = {}
	for q in itertools.count(3, 2):
		p = D.pop(q, 0)
		if p:
			x = q + p
			while x in D:
				x += p
			D[x] = p
		else:
			# if q >= start:
			yield q
			D[q*q] = 2*q


def solve(n):
	gen = gen_primes()

	for x in gen:
		if x > n//2+1:
			break

		if n % x != 0:
			continue

		# pre-check y==x
		if n % (x*x) == 0:
			return [x, x, n // (x*x)]

		for y in gen:
			if y > n//2+1:
				break

			if n % (x*y) == 0:
				return [x, y, n // (x*y)]

	return [1, 1, n]


def main():
	##
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	n = int(data[0])
	##

	# n = int(input())

	ans = solve(n)
	# print(*ans)

	##
	ans = ' '.join(map(str, ans))
	with open('output.txt', 'w') as f:
		f.write(ans)

	print('Done:')
	if len(ans) > 500:
		print(ans[:200] + '...')
	else:
		print(ans)
	##

main()
