# -*- coding: utf-8 -*-
f = open('LZW_Encoded.txt', 'r')
s = f.read().splitlines()
f.close()
print('File contains:\n-----\n{}\n-----'.format(s))
d = [chr(i) for i in range(256)]

stack = ''
flow = []

for i in range(len(s)):
	c = d[int(s[i])]
	stack += c
	if stack in d:
		flow.append(stack)
	else:
		flow.append(c)
		d.append(stack)
		stack = c

with open('LZW_Decoded.txt', 'w') as F:
	F.write(''.join(flow))
