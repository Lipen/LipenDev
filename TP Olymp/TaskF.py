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
		if t % 128742 == 0:
			print('Ex:', t)
			# print([alp[c] for c in format(t, '0>10')])
		# if t[-1] < len(alp)-1:
		# 	t[-1] += 1
		# else:
		# 	t[-1] = 0
		# 	for i in range(2, n+1):
		# 		if t[-i] < len(alp)-1:
		# 			t[-i] += 1
		# 			break
		# 		else:
		# 			t[-i] = 0
		# 	else:
		# 		return


def solve(n, p1, p2, h):
	passwords = []
	p1pows = {}
	for i in range(n):
		p1pows[i] = p1 ** i

	for s in genpass(n):
		if hash(s, p1pows) % p2 == h:
			passwords.append(''.join(s))
			print('Found pass: {}'.format(''.join(s)))
			if p1pows[n-1]*24+100 < p2:  # weeeird :O
				break

	return passwords


def main():
	##
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	n, p1, p2 = map(int, data[0].split())
	h = int(data[1])
	##

	s_ = 'ieuwoqe'
	n_ = len(s_)
	p1_ = 21
	p2_ = 717900988311817754
	p1pows_ = {}
	for i in range(n_):
		p1pows_[i] = p1_ ** i
	print('dbg: hash({}) = {}'.format(s_, hash(s_, p1pows_) % p2_))

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
