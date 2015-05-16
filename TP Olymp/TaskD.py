mem = [0 for _ in range(30000)]  # memory
p = 0  # pointer
x = 0  # variable
out = []  # sdtout


def debug(s):
	if False:
		print(s)


def process(s):
	global mem, p, x, out

	i = 0  # position in current string slice
	while i < len(s):
		c = s[i]
		if debug:
			debug('processing s[i={}] = {}'.format(i, c))

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
					debug('Wh::Going deeper: i+1:j ({} : {})'.format(i+1, j))
					end_if = j
					break

			debug('While mem[p] ({})'.format(mem[p]))
			while mem[p]:
				debug('While tick')
				# excluding end (there is closing bracet):
				process(s[start_if:end_if])

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
					debug('If::Going deeper: i+1:j ({} : {})'.format(i+1, j))
					end_if = j
					break

			debug('If mem[p] ({})'.format(mem[p]))
			if mem[p]:
				debug('If tick')
				# excluding end (there is closing bracet):
				process(s[start_if:end_if])

			i = end_if
		## LOCAL
		else:
			debug('Unknown char :{}:'.format(c))
		## ENDLOCAL
		i += 1


def main():
	## LOCAL
	global data
	with open('input.txt', 'r') as f:
		data = f.read().splitlines()

	s = data.pop(0)
	debug('Program: {}'.format(s))
	## ENDLOCAL

	# s = input()

	process(s)

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
	## ENDLOCAL

main()
