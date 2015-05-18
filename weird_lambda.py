d = [lambda: c for c in 'ABC']
# Same:
# d = []
# for c in 'ABC':
# 	d.append(lambda: c)

for func in d:
	print(func())
