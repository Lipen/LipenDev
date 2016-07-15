__author__ = 'Lipen'

import urllib.request, urllib.parse, re, sys

#List or urls separated by newline
urls = '''

'''.strip().splitlines()

# Fix format:
# file_type = (lambda s: '.mp4' if re.sub('[\W_]+', '', s)=='' else '.'+re.sub('[\W_]+', '', s))(input('Specify file format (default is \'mp4\'): '))
# As is: (not recommended)
file_type = (lambda s: '' if s=='None' else '.mp4' if s=='' else s if s.startswith('.') else '.'+s)(input('Specify file format (default is \"mp4\") (Enter \"None\" to force no extention): ')).lower()
print('Selected file type: {}'.format(file_type if file_type != '' else '(None)'))

counter = 0
counterMax = 10
widthStuff = 27 ##
width = 80 - widthStuff - counterMax

def hook(chunk_number, maximum_size, total_size):
	global counter
	if counter < counterMax and total_size!=-1 and chunk_number>0:
		if total_size==-1:
			print('.'*counterMax, end='', flush=True)
		c = int( (chunk_number * maximum_size) / (total_size/counterMax) ) + 1
		if c>counter:
			counter += 1
			print('.', end='', flush=True)

def main():
	global counter
	if len(urls)>0:
		print('Will be downloaded total {} files.'.format(len(urls)))
		for i in range(len(urls)):
			url = urls[i]
			raw_name = url[url.find('title=')+6:]
			file_name = urllib.parse.unquote(raw_name)
			try:
				if len(file_name) > width:
					t = file_name[:width-2] + '..'
				else:
					t = file_name[:width]
				print('{: >3}. Downloading "{: <{}}" '.format(str(i+1)[:3], t, width), end='', flush=True)
			except UnicodeEncodeError:
				t = re.sub('(%[0-9a-zA-Z]{2})+', '?', raw_name.replace('%20', ' '))
				if len(file_name) > width:
					t = t[:width-3] + '..'
				else:
					t = t[:width-1]
				print('{: >3}. Downloading ~"{: <{}}" '.format(str(i+1)[:3], t, width-1), end='', flush=True)
			try:
				counter = 0
				urllib.request.urlretrieve(url, file_name+file_type, reporthook=hook)
				print(' Done!')
			except Exception as e:
				print('\nDownload error: {}'.format(e))
		print('\nAll done!')
	else:
		print('Nothing to download.')

if __name__=='__main__':
	try:
		main()
	except Exception as e:
		print('Something gone wrong: {}'.format(e))
input('\nEnd... ')
