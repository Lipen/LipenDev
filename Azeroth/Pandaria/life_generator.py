from os import system

_sep = ' '
_alive = '■'  # alt+254
_dead = '·'  # alt+249
_extra = 2  # 0  # extra free space around actual field

data_to_string = lambda data: '\n'.join(_sep.join(_alive if (x, y) in data else _dead for x in range(min(data, key=lambda t: t[0])[0]-_extra, max(data, key=lambda t: t[0])[0]+1+_extra)) for y in range(min(data, key=lambda t: t[1])[1]-_extra, max(data, key=lambda t: t[1])[1]+1+_extra))


def life_generator(data_initial):
	data = set(data_initial)

	while True:
		newdata = set()

		for (x, y) in data:
			s = 0

			for i in [-1, 0, 1]:
				for j in [-1, 0, 1]:
					if i or j:
						if (x+i, y+j) in data:
							s += 1
						else:
							if sum((x+i+ti, y+j+tj) in data for ti in [-1, 0, 1] for tj in [-1, 0, 1] if ti or tj) == 3:
								newdata.add((x+i, y+j))

			if s == 2 or s == 3:
				newdata.add((x, y))

		yield newdata

		data = set(newdata)


def main():
	# state = ((0, 1, 0, 0, 0, 0, 0),
	# 		 (0, 0, 1, 0, 0, 0, 0),
	# 		 (1, 1, 1, 0, 0, 0, 0),
	# 		 (0, 0, 0, 0, 0, 1, 1),
	# 		 (0, 0, 0, 0, 0, 1, 1),
	# 		 (0, 0, 0, 0, 0, 0, 0),
	# 		 (1, 1, 1, 0, 0, 0, 0))
	# state = ((0, 1, 0, 0, 1, 0, 0, 0, 0, 0),
	# 		 (0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
	# 		 (1, 1, 1, 0, 0, 0, 0, 0, 0, 0),
	# 		 (0, 0, 0, 0, 0, 1, 1, 0, 0, 0),
	# 		 (0, 0, 0, 1, 0, 0, 1, 0, 1, 0),
	# 		 (0, 0, 0, 0, 0, 0, 0, 0, 0, 1),
	# 		 (1, 1, 1, 0, 0, 0, 0, 0, 1, 1))
	state = ((0, 0, 0, 0, 0, 0, 1, 0),
			 (1, 1, 0, 0, 0, 0, 0, 0),
			 (0, 1, 0, 0, 0, 1, 1, 1),)  # Diehard
	# state = ((1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1),)  # Inifinity

	data = set()
	for y in range(len(state)):
		for x in range(len(state[0])):
			if state[y][x]:
				data.add((x, y))

	life = life_generator(data)

	n = 0
	e = ''

	system('cls')
	print('Initial data ({} alive):\n'.format(len(data)) + data_to_string(data))

	while True:
		e = input()
		if e.lower() in ['q', 'quit', 'e', 'exit', 'stop']:
			break

		data = next(life)
		if n < 1000 and data:
			n += 1
			system('cls')
			print('Turn #{}. Alive: {}\n'.format(n, len(data)) + data_to_string(data))
		else:
			break

	system('cls')
	print('The end.\n\nTotal turns: {}'.format(n))

if __name__ == '__main__':
	main()
