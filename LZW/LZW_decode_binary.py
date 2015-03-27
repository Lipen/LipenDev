# -*- coding: utf-8 -*-
import math
with open('LZW_Encoded.txt', 'r') as f:
	s = f.read()
d = [chr(i) for i in range(256)]

i = 0
stack = ''
flow = []

while i < len(s):
	j = i + int(math.log(len(d)+1, 2))+1
	c = d[int(s[i:j], 2)]
	stack += c
	if stack in d:
		flow.append(stack)
	else:
		flow.append(c)
		d.append(stack)
		stack = c
	i = j

with open('LZW_Decoded.txt', 'w') as F:
	F.write(''.join(flow))
