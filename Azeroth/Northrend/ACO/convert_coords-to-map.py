import math
import pyperclip

with open('input.txt', 'r') as f:
	raw = f.read().splitlines()
	data = [list(map(float, line.strip().split())) for line in raw]

n = len(data)

edges = [[0 for _ in range(n)] for _ in range(n)]

for i in range(n-1):
	xi, yi = data[i][1], data[i][2]
	for j in range(i+1, n):
		xj, yj = data[j][1], data[j][2]
		d = math.hypot(xi-xj, yi-yj)
		edges[i][j] = d
		edges[j][i] = d

s = '\n'.join(' '.join(map(lambda x: str(round(x)), row)) for row in edges)
pyperclip.copy(s)

if len(s) > 1000:
	print(s[:500] + '...')
else:
	print(s)
