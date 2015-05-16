mem = [0 for _ in range(30000)]  # memory
p = 0  # pointer
x = 0  # variable
out = []  # sdtout


def process(data, s):
	global mem, p, x, out

	i = 0
	while i < len(s):
		c = s[i]
		print('processing s[i={}] = {}'.format(i, c))

		if c == ',':
			mem[p] = int(data.pop(0)) % 256
			# mem[p] = int(input()) % 256
		elif c == '.':
			out.append(mem[p])
		elif c == '=':
			mem[p] = mem[p-1]
		elif c == '0':
			mem[p] = 0
		elif c == '!':
			x = mem[p]
		elif c == '?':
			mem[p] = x
		elif c == '*':
			mem[p] = (mem[p] * mem[p-1]) % 256
		elif c == '/':
			mem[p] = (mem[p] // mem[p-1]) % 256
		elif c == '+':
			mem[p] = (mem[p] + 1) % 256
		elif c == '-':
			mem[p] = (mem[p] - 1) % 256
		elif c == '>':
			p = (p + 1) % 30000
		elif c == '<':
			p = (p - 1) % 30000
		elif c == '|':
			p = 0
		elif c == '^':
			out.append(x)
		elif c == '$':
			x = p
		elif c == '[':
			start_if = i + 1
			end_if = start_if  # temporary set
			k = 1

			for j in range(start_if, len(s)):
				if s[j] == '[':
					k += 1
				elif s[j] == ']':
					k -= 1

				if k == 0:
					print('Wh::Going deeper: i+1:j ({} : {})'.format(i+1, j))
					end_if = j
					break

			print('While mem[p] ({})'.format(mem[p]))
			while mem[p]:
				print('While tick')
				# excluding end (there is closing bracet):
				process(data, s[start_if:end_if])

			i = end_if
		elif c == '(':
			start_if = i + 1
			end_if = start_if  # temporary set
			k = 1

			for j in range(start_if, len(s)):
				if s[j] == '(':
					k += 1
				elif s[j] == ')':
					k -= 1

				if k == 0:
					print('If::Going deeper: i+1:j ({} : {})'.format(i+1, j))
					end_if = j
					break

			print('If mem[p] ({})'.format(mem[p]))
			if mem[p]:
				print('If tick')
				# excluding end (there is closing bracet):
				process(data, s[start_if:end_if])

			i = end_if
		else:
			print('Unknown char :{}:'.format(c))
		i += 1


def main():
	## LOCAL
	global data
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	s = data.pop(0)
	print('Program: {}'.format(s))
	## ENDLOCAL

	# s = input()

	process(data, s)
	# print('\n'.join(map(str, out)))

	## LOCAL
	ans = '\n'.join(map(str, out))
	with open('output.txt', 'w') as f:
		f.write(ans)

	print('Done:')
	if len(ans) > 500:
		print(ans[:200] + '...')
	else:
		print(ans)
	## LOCAL

main()
