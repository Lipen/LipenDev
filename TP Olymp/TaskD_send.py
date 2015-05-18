mem = [0 for _ in range(30000)]  # memory
p = 0  # pointer
x = 0  # variable
out = []  # sdtout


def process(s):
	global mem, p, x, out

	i = 0  # position in current string slice
	while i < len(s):
		c = s[i]

		if c == ',':
			try:
				mem[p] = int(input()) % 256
			except:
				mem[p] = 0
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
			mem[p] = (mem[p] * mem[(p-1) % 30000]) % 256
		elif c == '/':
			mem[p] = (mem[p] // mem[(p-1) % 30000])
		elif c == '+':
			mem[p] = (mem[p] + 1) % 256
		elif c == '-':
			mem[p] = max(0, mem[p] - 1)
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
					end_if = j
					break

			while mem[p]:
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
					end_if = j
					break

			if mem[p]:
				# excluding end (there is closing bracet):
				process(s[start_if:end_if])

			i = end_if
		i += 1


def main():
	s = input()

	process(s)

	print('\n'.join(map(str, out)))

main()
