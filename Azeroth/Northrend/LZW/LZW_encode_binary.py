# -*- coding: utf-8 -*-
import math
with open('LZW_toEncode.txt', 'r') as f:
	s = f.read()
d = [chr(i) for i in range(256)]

stack = ''
flow = []

for c in s:
	stack += c
	if stack not in d:
		flow.append(bin(d.index(stack[:-1]))[2:].rjust(int(math.log(len(d), 2))+1, '0'))
		d.append(stack)
		stack = c

if stack:
	flow.append(bin(d.index(stack))[2:].rjust(int(math.log(len(d), 2))+1, '0'))

with open('LZW_Encoded.txt', 'w') as F:
	F.write(''.join(map(str, flow)))
