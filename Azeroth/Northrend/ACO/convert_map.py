with open('input.txt', 'r') as f:
	data = [x.strip().split() for x in f.readlines()]

n = len(data)
assert len(data[0]) == n, "Matrix is not square"

width = max([len(data[i][j]) for i in range(n) for j in range(n)])

s = 'std::array< std::array<size_t, {0}>, {0} > W {{{{\n'.format(n)
s += ',\n'.join(map(lambda line: '{' + ','.join(map(lambda x: format(x, ' >{}'.format(width)), line)) + '}', data))
s += '}};'

print(s)
