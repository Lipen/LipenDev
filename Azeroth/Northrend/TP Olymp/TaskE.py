from math import *


def f(e, x):
	return abs(eval(e.replace('^', '**').replace('x', '('+str(x)+')')))


def solve(e, a, b):
	N = 1999
	t = f(e, a) + f(e, b)

	for i in range(1, 2*N):
		if i % 2 == 0:
			t += 2*f(e, a + (b-a)*i/2/N)
		else:
			t += 4*f(e, a + (b-a)*i/2/N)

	return (b-a)*t/6/N


def main():
	##
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	a, b = map(int, data[0].split())
	e = data[1]
	##

	# a, b = map(int, input().split())
	# e = input()

	ans = solve(e, a, b)
	# print(ans)

	##
	ans = str(ans)
	with open('output.txt', 'w') as f:
		f.write(ans)

	print('Done:')
	if len(ans) > 500:
		print(ans[:200] + '...')
	else:
		print(ans)
	##

main()
