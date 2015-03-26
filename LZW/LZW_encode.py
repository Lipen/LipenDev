# -*- coding: utf-8 -*-
with open('LZW_toEncode.txt', 'r') as f:
	s = f.read()
# print('File contains:\n-----\n{}\n-----'.format(s))
d = [chr(i) for i in range(256)]

stack = ''
flow = []

for c in s:
	stack += c
	if stack not in d:
		flow.append(d.index(stack[:-1]))
		d.append(stack)
		stack = c

if stack:
	flow.append(d.index(stack))

with open('LZW_Encoded.txt', 'w') as F:
	F.write('\n'.join(map(str, flow)))
