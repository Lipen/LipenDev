from common import *
from Cell import Cell


class Field:

	"""Field class..
	"""

	def __init__(self, n, m, cellw, cellh):
		self.n, self.m = n, m
		self.cellsGrid = [[Cell(self, i*cellw, j*cellh, cellw, cellh, (random.randint(221, 255), random.randint(221, 255), random.randint(221, 255), 255)) for j in range(m)] for i in range(n)]
		self.cells = [cell for row in self.cellsGrid for cell in row]
		print('Total cells: {}'.format(len(self.cells)))

	def getCellFromPoint(self, p):
		for cell in self.cells:
			if cell.isContainsPoint(p):
				return cell

	def getCellFromNumber(self, n, m):
		if n < 0 or n >= self.n or m < 0 or m >= self.m:
			return None
		return self.cellsGrid[n][m]

	def update(self):
		# t = time.time()
		for cell in self.cells:
			cell.update()
		# print('TimeIt: Field.update() - {:.3f} ms.'.format(1000*(time.time() - t)))
