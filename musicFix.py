'''
Fixes broken "&" sign, which is sometimes vanishes, leaving double space
on his place. So we should replace this double space on sign "&"

Usage:
	Script prompts for path and then automatically scans folders to find
	files with "double spaces" in their names, replacing them for "&".
'''

import os
path = input('Enter path: ')
if path=='' or not os.path.exists(pathMaster):
	path = 'C:\\Data\\Media\\Music'
	print('Default path: {}'.format(path))
else:
	print('Path: {}'.format(path))
wrongfiles = [[file, directory] for directory in [d for d in os.listdir(path) if os.path.isdir(os.path.join(path, d))] for file in os.listdir(os.path.join(path, directory)) if file.find('  ')>=0]
if len(wrongfiles)>0:
	print('List of wrong files:\n  ' + '\n  '.join([f[0] for f in wrongfiles]))
	while 1:
		e = input('Type f to fix them: ').lower()
		if e in ['q', 'e', 'quit', 'exit', 'stop']:
			break
		elif e in ['f', 'fix']:
			print('Fixing', end='', flush=True)
			for item in [os.path.join(path, f[1], f[0]) for f in wrongfiles]:
				os.rename(item, item.replace('  ', ' & '))
				print('.', end='', flush=True)
			print(' done!')
			break
else:
	print('Fixing isn\'t necessary')
input('End...')
