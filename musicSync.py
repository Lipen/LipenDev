'''
Usage:
	* "Master" path - main path. "Slave" path - secondary path.
	* Script compares files inside these pathes:
		- File marks to be copied from master to slave if there is no
			such file in slave
		- File marks to be deleted in slave if there is no such file
			in master
	** For now: script compares only file names, not their content
'''

import os, shutil

def dot(c='.', arg=None):
	print(c, end='', flush=True)
	if arg is not None:
		return arg

pathMaster = input('Enter master path: ')
if pathMaster=='' or not os.path.exists(pathMaster):
	pathMaster = 'C:\\Data\\Media\\Music'
	print('Default ', end='', flush=True)
print('Master\'s path: {}'.format(pathMaster))

pathSlave = input('Enter slave path: ')
if pathSlave=='' or not os.path.exists(pathSlave):
	pathSlave = 'E:\\Media\\Music'
	print('Default ', end='', flush=True)
print('Slave\'s path: {}'.format(pathSlave))

print('Building master directory list', end='', flush=True)
dirlistMaster = [dot(arg=item) for item in os.listdir(pathMaster) if os.path.isdir(os.path.join(pathMaster, item))]
print(' done!')
print('Building slave directory list', end='', flush=True)
dirlistSlave = [dot(arg=item) for item in os.listdir(pathSlave) if os.path.isdir(os.path.join(pathSlave, item))]
print(' done!')

copyList = [] #[[from, to] , ...]
deleteList = []
print('Building master absent directory list', end='', flush=True)
absentDirsMaster = [dot(arg=item) for item in dirlistSlave if item not in dirlistMaster]
print(' done!')
print('Building slave absent directory list', end='', flush=True)
absentDirsSlave = [dot(arg=item) for item in dirlistMaster if item not in dirlistSlave]
print(' done!')

print('Processing directories', end='', flush=True)
for directory in dirlistSlave:
	dot(':')
	if directory in dirlistMaster:
		dot('.')
		for fileSlave in [f for f in os.listdir(os.path.join(pathSlave, directory)) if f[-4:]=='.mp3']:
			if fileSlave not in [f for f in os.listdir(os.path.join(pathMaster, directory)) if f[-4:]=='.mp3']:
				deleteList.append(os.path.join(pathSlave, directory, fileSlave))
		for fileMaster in [f for f in os.listdir(os.path.join(pathMaster, directory)) if f[-4:]=='.mp3']:
			if fileMaster not in [f for f in os.listdir(os.path.join(pathSlave, directory)) if f[-4:]=='.mp3']:
				copyList.append([os.path.join(pathMaster, directory, fileMaster), os.path.join(pathSlave, directory), fileMaster, directory])
print(' done!')

print('\nReport:\n')

print('{} entries going to be delete at {}{}'.format(len(deleteList), pathSlave, '.' if len(deleteList)==0 else ':'))
if len(deleteList)>0:
	for item in deleteList:
		print(item[len(pathSlave)+1:])

print('\n{} entries going to be copied to {}{}'.format(len(copyList), pathSlave, '.' if len(copyList)==0 else ':'))
if len(copyList)>0:
	for item in copyList:
		print('{}: {}'.format(item[3], item[2]))

if len(absentDirsMaster)>0:
		print('\nThere {} {} absent directories in Master:'.format('is' if len(absentDirsMaster)==1 else 'are', len(absentDirsMaster)))
		for item in absentDirsMaster:
			print(item)

if len(absentDirsSlave)>0:
		print('\nThere is {} absent directories in Slave:'.format(len(absentDirsSlave)))
		for item in absentDirsSlave:
			print(item)

while 1:
	e = input('Exit or continue...').lower()
	if e in ['cont', 'continue', 'go', 'next', 'second']:
		break
	elif e in ['exit', 'quit', 'first', 'stop']:
		exit()
	else:
		continue

e = input('Type:\n\t\'d\' to delete files\n\t\'c\' to copy files.\n... ').lower()
print('\nProcessing actions.\n')

if 'd' in e:
	if len(deleteList)>0:
		print('Removing {} entries:'.format(len(deleteList)))
		for item in deleteList:
			os.remove(item)
			print('  {}  has been deleted'.format(item))
		print('Removing done.')
	else:
		print('Deleting isn\'t necessary.')
if 'c' in e:
	if len(copyList)>0:
		print('Copying {} entries:'.format(len(copyList)))
		for item in copyList:
			shutil.copy2(item[0], item[1])
			print('  {}  has been copied'.format(item[2]))
		print('Copying done.')
	else:
		print('Copying isn\'t necessary.')

input('\nAll done...')
input('End...')
input('Quit...')
