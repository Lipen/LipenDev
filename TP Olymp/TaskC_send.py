def solve(n, s):
	import re

	return len(s) >= n and re.search('[^a-zA-Z0-9]', s) == None and re.search('[a-z]', s) and re.search('[A-Z]', s) and re.search('[0-9]', s)


def main():
	n = int(input())
	s = input()

	ans = solve(n, s)
	print('YES' if ans else 'NO')

main()
