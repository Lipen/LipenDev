# -*- coding: utf-8 -*-
def round_f(s, n):
	s = str(s)
	k = 0
	n = int(n)
	while s[k] != '.':
		k += 1
	if (n+k) < len(s)-1:
		if s[k+1+n] > '5':
			return s[:k+n] + str(int(s[k+n])+1)
		elif s[k+1+n] < '5':
			return s[:k+1+n]
		else:
			if int(s[k+n]) % 2 == 0:
				return s[:k+n] + str(int(s[k+n])+1)
			else:
				return s[:k+1+n]
	elif (n+k) == len(s)-1:
		return s
	else:
		return 'Error!'

while True:
	st, b = input().split()
	print(round_f(st, b))
