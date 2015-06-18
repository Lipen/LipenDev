import random
import re
# CATGACTTCAGCTGGTTAAC :: @TSAG~


class Cell:
	seq = ''
	genes = ''
	food = 0
	feeding_rate = 1.
	name = ''

	def __init__(self, seq, name):
		self.seq = seq
		self.name = name

	def getGenes(self):
		return '@' + self.genes + '~'

	def getName(self):
		if len(self.name) > 20:
			return self.name[:7] + '...' + self.name[-10:]
		else:
			return self.name

	def test(self):
		# think that smaller regex:
		# (ATG)+(?P<genes>(...)*?)((TAG)|(TAA)|(TGA))
		# will be enough...
		t = re.match('.*?(ATG)+(?P<genes>(...)*?)((TAG)|(TAA)|(TGA)).*', self.seq)  # FIXME: replace regex with proper searching loop
		# bug: ATGGCCGTAATTGTAGGCCATCCTATCACACCCTGGCCGTTTAATGTAAA shouldn`t fit, because first ATG-start codon doesn`t have matching stop-codon, BUT another one ATG at the end has it, which shouldn`t matter, because first doesn`t!
		if t:
			self.genes = convertToProteins(t.group('genes'))

			# There goes genes' functions:
			self.feeding_rate = max(.1, 1. + 0.5*self.genes.count('R')**(0.5) + 1.*self.genes.count('RRRRR')**(2) - 1.*self.genes.count('E'))

			##
			return True
		else:  # else it is a junk
			return False

	def feed(self, k=1):
		self.food += k*self.feeding_rate

	def mutate(self, n=1):
		for t in random.sample(range(len(self.seq)), n):
			# t = random.randint(0, len(self.seq)-1)
			self.seq = self.seq[:t] + {'A': 'GCT', 'G': 'ACT', 'T': 'AGC', 'C': 'ATG'}[self.seq[t]][random.randint(0, 2)] + self.seq[t+1:]

	def produce(self):
		F = []
		k = int(self.food)

		limMin = 3
		limEx = 10
		ex = 5

		if k > limMin:
			if k > limEx:
				k = limEx + (k-limEx)//ex
				F.append(self)  # Also reproduces itself

			for i in range(k):
				t = Cell(self.seq, convertName(self.name, i+1, k))
				t.mutate(random.randint(1, 3))
				if t.test():
					F.append(t)

		self.food = 0
		return F


def initial_random_cell(n=20):
	return Cell(''.join('ACGT'[random.randint(0, 3)] for _ in range(n)), '&')


def convertToProteins(s):
	d = {'TTT': 'F', 'TTC': 'F', 'TTA': 'L', 'TTG': 'L', 'TCT': 'S',
	'TCC': 'S', 'TCA': 'S', 'TCG': 'S', 'TAT': 'Y', 'TAC': 'Y', 'TAA': 'x',
	'TAG': 'x', 'TGT': 'C', 'TGC': 'C', 'TGA': 'x', 'TGG': 'W', 'CTT': 'L',
	'CTC': 'L', 'CTA': 'L', 'CTG': 'L', 'CCT': 'P', 'CCC': 'P', 'CCA': 'P',
	'CCG': 'P', 'CAT': 'H', 'CAC': 'H', 'CAA': 'Q', 'CAG': 'Q', 'CGT': 'R',
	'CGC': 'R', 'CGA': 'R', 'CGG': 'R', 'ATT': 'I', 'ATC': 'I', 'ATA': 'I',
	'ATG': 'M', 'ACT': 'T', 'ACC': 'T', 'ACA': 'T', 'ACG': 'T', 'AAT': 'N',
	'AAC': 'N', 'AAA': 'K', 'AAG': 'K', 'AGT': 'S', 'AGC': 'S', 'AGA': 'R',
	'AGG': 'R', 'GTT': 'V', 'GTC': 'V', 'GTA': 'V', 'GTG': 'V', 'GCT': 'A',
	'GCC': 'A', 'GCA': 'A', 'GCG': 'A', 'GAT': 'D', 'GAC': 'D', 'GAA': 'E',
	'GAG': 'E', 'GGT': 'G', 'GGC': 'G', 'GGA': 'G', 'GGG': 'G', '': ''}
	return ''.join(d[s[i*3:(i+1)*3]] for i in range(len(s)//3))


def convertName(name, i, n):
	s = ''

	if n < 10:
		s = str(i)
	else:
		if n <= 26:
			s = chr(ord('A') + i-1)
		else:
			x = i-1
			while x:
				s += chr(ord('a') + x % 26)
				x //= 26
			s = s[::-1].lower()
			s = s[0].upper()+s[1:]

	return name + s


def turn(A, i):
	if not A:
		return []

	F = []

	food = 50 + i**(1/4)
	while food > 0:
		a = random.choice(A)
		x = min(food, random.uniform(0.5, 2.))
		a.feed(x)
		food -= x

	for i in range(len(A)):
		f = A[i].produce()
		F.extend(f)

	return F


def main():
	F = []
	n = 5

	while len(F) < n:
		f = initial_random_cell(100)
		if f.test():
			F.append(f)
			# print('Born #{}: \"{}\" :: {}'.format(len(F), f.getName(), f.getGenes()))

	print('Initial generation: ({} species)'.format(len(F)))
	for i in range(len(F)):
		print('#{: <2}: \"{}\" :: {}'.format(i+1, F[i].getName(), F[i].getGenes()))
	print('-------------------')

	for i in range(100):  # skip turns
		F = turn(F, i)

	print('New generation: ({} species)'.format(len(F)))
	for i in range(len(F)):
		print('#{: <2}: \"{}\" :: {}'.format(i+1, F[i].getName(), F[i].getGenes()))
	print('-------------------')


main()
