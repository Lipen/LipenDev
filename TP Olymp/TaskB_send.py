def solve(s):
	if len(s) != 80:
		return '0'*80
	elif s.startswith('1111') and not s.endswith('1111'):
		return s
	elif s.endswith('1111') and not s.startswith('1111'):
		return s[::-1]
	else:
		return '0'*80


def main():
	s = input()

	ans = solve(s)
	print(ans)

main()
