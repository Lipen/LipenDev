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
	##
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	s = data[0]
	##

	# s = input()

	ans = solve(s)
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
