f = {'y': 24, 'r': 17, 'u': 20, 'w': 22, 'p': 15, 't': 19, 'q': 16, 'i': 8, 'e': 4, 'o': 14}


def hash(s, p1pows):
	h = 0

	for i in range(len(s)):
		h += f[s[i]] * p1pows[i]

	return h


def genpass(n):
	fmt = '0>{}'.format(n)

	for t in range(10**n):
		yield ['qwertyuiop'[int(c)] for c in format(t, fmt)]


def solve(n, p1, p2, h):
	passwords = []
	p1pows = {}
	for i in range(n):
		p1pows[i] = p1 ** i

	for s in genpass(n):
		if hash(s, p1pows) % p2 == h:
			passwords.append(''.join(s))
			if p1pows[n-1]*24+100 < p2:  # weeeird :O
				break

	return passwords


def main():
	n, p1, p2 = map(int, input().split())
	h = int(input())

	ans = solve(n, p1, p2, h)
	for item in ans:
		print(item)

main()
