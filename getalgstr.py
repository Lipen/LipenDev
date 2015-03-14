import random

STR = 'Legere fabellas ut eam, eam illud discere euismod ne'
# ALPHABET = 'abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890,.!?@#$%^&*()-=_+/|\\{}[]:;\'"<>'
ALPHABET = 'abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890'

def getRandomChar(fr=None, to=None):
	j = ALPHABET.find(to) if to is not None else len(STR)-1
	# j = len(STR)-1 if j == -1 else j
	i = ALPHABET.find(fr) if fr is not None else 0
	# i = 0 if i == -1 else i
	return random.choice(ALPHABET[i:j+1] if i<j else ALPHABET[j:i+1])


def best(f):
	i = 0
	dm = sum([1 if STR[i] != f[0][i] else 0 for i in range(len(STR))])
	for j in range(1, len(f)):
		d = sum([1 if STR[i] != f[j][i] else 0 for i in range(len(STR))])
		if d < dm:
			i = j
			dm = d
	# print('dm = {}'.format(dm))
	return f[i], dm


def gen(s, n=10, chance=5, minamount=0, maxamount=5):
	w = []
	for i in range(n):
		t = s
		c = 0
		if minamount == 0:
			l = list(range(len(STR)))
			random.shuffle(l)
			for j in l:
				if c >= maxamount:
					break
				if STR[j] != t[j]:
					if random.randint(1, 100) <= chance:
						t = t[:j]+getRandomChar()+t[j+1:]
						# t = t[:j]+getRandomChar(t[j], STR[j])+t[j+1:]
						c += 1
		# print('Changed {} chars'.format(c))
		w.append(t)
	return w


def loop(s):
	i = 0
	while i < 500:
		i += 1
		f = gen(s, 10)
		f = gen(s, 10)
		s, dm = best(f)
		if i % 10 == 0:
			print('Best of f{} (dm={}): {}'.format(i, dm, s))

# t = 'Lpgire igcfllov vtItama sam iXlud eiwhhreIrziTmoe ne'
t = ''.join([getRandomChar() for _ in range(len(STR))])
print('Initial: {}'.format(t))
loop(t)
