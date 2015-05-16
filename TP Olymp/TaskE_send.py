from math import *


def f(e, x):
	return abs(eval(e.replace('^', '**').replace('x', str(x))))


def solve(e, a, b):
	N = 5741
	t = f(e, a) + f(e, b)

	for i in range(1, 2*N):
		if i % 2 == 0:
			t += 2*f(e, a + (b-a)*i/2/N)
		else:
			t += 4*f(e, a + (b-a)*i/2/N)

	return (b-a)*t/6/N


def main():
	a, b = map(int, input().split())
	e = input()

	ans = solve(e, a, b)
	print(ans)

main()
