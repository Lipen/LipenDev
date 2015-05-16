import itertools


def gen_primes():
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
	n = int(input())

	ans = solve(n)
	print(*ans)

main()
