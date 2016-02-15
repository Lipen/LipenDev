robots = [
	(500, 0, 0, 0, 0, 0),
	(-1000, -500, 1, 0, 0, 0),
	# (-300, 800, 0, 0, 0, 0),
	# (-600, 200, 3, 0, 0, 0),
]  # x, y, angle, __a, __b, __r
ball = (0, 0, 300, -50, 0, 0)  # x, y, vx, vy, ax, ay

with open('data', 'w') as f:
	f.write('{}\n'.format(len(robots)))
	f.write((' '.join('{}' for _ in range(len(ball))) + '\n').format(*ball))
	for robot in robots:
		f.write((' '.join('{}' for _ in range(len(robot))) + '\n').format(*robot))

with open('control', 'w') as f:
	f.write('\n'.join('0 0' for _ in range(len(robots))))
