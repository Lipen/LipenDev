import random
import re


# Some species:
# CATGACTTCAGCTGGTTAAC :: @TSAG~

class Cell:
	seq = ''
	genes = ''

	def __init__(self, seq):
		self.seq = seq

	def getGenes(self):
		return '@' + self.genes + '~'

	def test(self):
		t = re.match('.*?(ATG)+(?P<genes>(...)*?)((TAG)|(TAA)|(TGA)).*', self.seq)
		if t:
			self.genes = convertToProteins(t.group('genes'))
			return True
		else:
			return False

	def mutate(self):
		t = random.randint(0, len(self.seq)-1)
		self.seq = self.seq[:t] + {'A': 'GCT', 'G': 'ACT', 'T': 'AGC', 'C': 'ATG'}[self.seq[t]][random.randint(0, 2)] + self.seq[t+1:]


def initial_random_cell(n=20):
	return Cell(''.join('ACGT'[random.randint(0, 3)] for _ in range(n)))


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


def turn(A):
	F = []

	for i in range(len(A)):
		q = A[i].seq
		for k in range(5):
			t = Cell(q)
			t.mutate()
			if t.test():
				F.append(t)

	return F


def main():
	F = []
	n = 1

	while len(F) < n:
		f = initial_random_cell()
		if f.test():
			F.append(f)
			print('Born #{}: {} :: {}'.format(len(F), f.seq, f.getGenes()))

	print('Initial generation:')
	for i in range(len(F)):
		print('#{: <2}: {} :: {}'.format(i+1, F[i].seq, F[i].getGenes()))
	print('-------------------')

	for _ in range(5):  # skip turns
		F = turn(F)

	print('New generation:')
	for i in range(len(F)):
		print('#{: <2}: {} :: {}'.format(i+1, F[i].seq, F[i].getGenes()))
	print('-------------------')


main()
