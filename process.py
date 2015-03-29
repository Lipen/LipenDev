import os
sys = os.system

FLAG_clear = ['/c', '-c']
FLAG_window = ['/w', '-w']
FLAG_exit = ['/e', '-e']


def main():
	print('List of existing <*.cpp> files:')
	files = []
	counter = 0
	for file in os.listdir():
		if file[-4:] == '.cpp':
			counter += 1
			files.append(file)
			print('{:->3d}) {}'.format(counter, file[:-4]))

	name = ''
	flags = []
	command, *ex = input('Enter your <command> [<name>] [<*flags>]: ').split()
	if len(ex):
		name = ex[0]
		flags = list(ex[1:])
		try:
			name = files[int(name) - 1]
		except:
			if name[0] == '#':
				try:
					fileid = int(name[1:])
					name = files[fileid - 1]
				except:
					pass
	else:
		flags = list(ex)

	if command == 'open':
		if len(list(set(FLAG_clear).intersection(set(flags)))) > 0:
			sys('cls')
		if len(list(set(FLAG_window).intersection(set(flags)))) > 0:
			sys('start {}'.format(name))
		else:
			sys('{}'.format(name))
	elif command == 'compile':
		if len(list(set(FLAG_clear).intersection(set(flags)))) > 0:
			sys('cls')
		print('Compiling...')
		err = sys('g++ {} -std=c++0x -o {}.exe'.format(name, name[:-4]))
		if err:
			print('Error during compiling. <{}>'.format(err))
		else:
			print('Compiled succesfully.')
	elif command == 'run':
		if len(list(set(FLAG_clear).intersection(set(flags)))) > 0:
			sys('cls')
		print('Compiling...')
		err = sys('g++ {} -std=c++0x -o {}.exe'.format(name, name[:-4]))
		if err:
			print('Error during compiling. <{}>'.format(err))
		else:
			print('Compiled succesfully. Starting:\n' + '-' * 31)
			if len(list(set(FLAG_window).intersection(set(flags)))) > 0:
				err2 = sys('start {}.exe'.format(name[:-4]))
			else:
				err2 = sys('{}.exe'.format(name[:-4]))
			if err2:
				print('-' * 30 + '\nError during execution. <{}>'.format(err2))
			else:
				print('-' * 17 + '\nDone succesfully.')
	elif command == 'list':
		if name != '':
			if len(list(set(FLAG_clear).intersection(set(flags)))) > 0:
				sys('cls')
			print('List of existing <*.{}> files:'.format(name))
			l = len(name)
			for file in os.listdir():
				if file[-l:] == name:
					print('{:>20}'.format(file[:-l - 1]))
		else:
			print('List of all existing files:')
			for file in os.listdir():
				print('{:>20}'.format(file))

	if len(list(set(FLAG_exit).intersection(set(flags)))) == 0:
		input('-' * 25 + '\nEnd. Press enter to exit: ')

main()
