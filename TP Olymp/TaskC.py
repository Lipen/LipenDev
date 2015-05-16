def solve(n, s):
	import re

	return len(s) >= n and re.search('[^a-zA-Z0-9]', s) == None and re.search('[a-z]', s) and re.search('[A-Z]', s) and re.search('[0-9]', s)


def main():
	##
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	n = int(data[0])
	s = data[1]
	##

	# n = int(input())
	# s = input()

	ans = solve(n, s)
	# print('YES' if ans else 'NO')

	##
	ans = 'YES' if ans else 'NO'
	with open('output.txt', 'w') as f:
		f.write(ans)

	print('Done:')
	if len(ans) > 500:
		print(ans[:200] + '...')
	else:
		print(ans)
	##

main()
