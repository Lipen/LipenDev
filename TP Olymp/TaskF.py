def f(c):
	return ord(c) - 97  # ord('a')==97


def hash(s, p1, p2):
	h = 0
	p = 1

	for c in s:
		h += f(c) * p
		p *= p1

	return h % p2


def genpass(n):
	t = [0 for _ in range(n)]
	alp = list('qwertyuiop')

	while True:
		yield ''.join(map(lambda x: alp[x], t))
		if t[-1] < len(alp)-1:
			t[-1] += 1
		else:
			t[-1] = 0
			for i in range(2, n+1):
				if t[-i] < len(alp)-1:
					t[-i] += 1
					break
				else:
					t[-i] = 0
			else:
				return


def solve(n, p1, p2, h):
	passwords = []

	for s in genpass(n):
		if hash(s, p1, p2) == h:
			passwords.append(s)
			print('Found pass: {}'.format(s))

	return passwords


def main():
	##
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	n, p1, p2 = map(int, data[0].split())
	h = int(data[1])
	##

	# n, p1, p2 = map(int, input().split())
	# h = int(input())

	ans = solve(n, p1, p2, h)
	# for item in ans:
	# 	print(item)

	##
	ans = '\n'.join(ans)
	with open('output.txt', 'w') as f:
		f.write(ans)

	print('Done:')
	if len(ans) > 500:
		print(ans[:200] + '...')
	else:
		print(ans)
	##

main()
