# enter D
# init alpha, beta, e, q
# init edges -> set eta(i,j) + set initial phe
# place ants all over the graph
# find initial (local) T*, L*  //greedy?

# Main cycle:
# for t in 1..t_max
# 	for k in 1..m  // loop over ants
# 		build T_k(t), calc len(L_k(t))
#	compare all L_k with L*:
# 		update T* and L*
# 	update all edges (phe)

# print T*, L*
import random


class Edge:

	def __init__(self, v1, v2, weight, phe):
		self.v1 = v1
		self.v2 = v2
		self.weight = weight
		self.eta = 1. / weight
		self.phe = phe
		self.phe_acc = 0.0


class Ant:

	def __init__(self, town):
		self.town = town
		self.start = town
		self.visited = []
		self.path_len = 0.0

#     1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
W = [[0, 29, 82, 46, 68, 52, 72, 42, 51, 55, 29, 74, 23, 72, 46],  # 1
	 [29, 0, 55, 46, 42, 43, 43, 23, 23, 31, 41, 51, 11, 52, 21],  # 2
	 [82, 55, 0, 68, 46, 55, 23, 43, 41, 29, 79, 21, 64, 31, 51],  # 3
	 [46, 46, 68, 0, 82, 15, 72, 31, 62, 42, 21, 51, 51, 43, 64],  # 4
	 [68, 42, 46, 82, 0, 74, 23, 52, 21, 46, 82, 58, 46, 65, 23],  # 5
	 [52, 43, 55, 15, 74, 0, 61, 23, 55, 31, 33, 37, 51, 29, 59],  # 6
	 [72, 43, 23, 72, 23, 61, 0, 42, 23, 31, 77, 37, 51, 46, 33],  # 7
	 [42, 23, 43, 31, 52, 23, 42, 0, 33, 15, 37, 33, 33, 31, 37],  # 8
	 [51, 23, 41, 62, 21, 55, 23, 33, 0, 29, 62, 46, 29, 51, 11],  # 9
	 [55, 31, 29, 42, 46, 31, 31, 15, 29, 0, 51, 21, 41, 23, 37],  # 10
	 [29, 41, 79, 21, 82, 33, 77, 37, 62, 51, 0, 65, 42, 59, 61],  # 11
	 [74, 51, 21, 51, 58, 37, 37, 33, 46, 21, 65, 0, 61, 11, 55],  # 12
	 [23, 11, 64, 51, 46, 51, 51, 33, 29, 41, 42, 61, 0, 62, 23],  # 13
	 [72, 52, 31, 43, 65, 29, 46, 31, 51, 23, 59, 11, 62, 0, 59],  # 14
	 [46, 21, 51, 64, 23, 59, 33, 37, 11, 37, 61, 55, 23, 59, 0]]  # 15
n = len(W)

alpha = 0.8  # non-greedyness
beta = 1.0 - alpha
Q = 50
rho = 0.02

phe_min = 1
phe_max = 20

_SHOW_PHEROMON_MAP = True
_SHOW_ETA_MAP = False

t_max = 1000
try:
	with open('opt', 'r') as f:
		f_ = f.read().splitlines()
		L_opt = float(f_[0])
		T_opt = f_[1]
except:
	L_opt = 0
	T_opt = ''
L_opt_now = 0
T_opt_now = ''


def get_TL(edges_t):
	T = [0]
	g = 0
	L = 0.0

	for _ in range(n-1):
		m = 0
		m_index = -1
		for j in range(n):
			if j != g and j not in T and (edges_t[g][j].phe > m or m_index == -1):
				m = edges_t[g][j].phe
				m_index = j
		assert m_index >= 0, "Index... not found?"
		L += edges_t[g][m_index].weight
		g = m_index
		T.append(g)

	L += edges_t[g][0].weight
	T.append(0)

	return (T, L)


def T_format(T):
	return '->'.join(map(lambda x: str(x+1), T))


def main():
	global L_opt, T_opt, L_opt_now, T_opt_now

	edges_t = [[None for _ in range(n)] for _ in range(n)]
	edges = []
	for i in range(n-1):
		for j in range(i+1, n):
			e = Edge(v1=i, v2=j, weight=W[i][j], phe=random.uniform(1., 5.))
			edges.append(e)
			edges_t[i][j] = e
			edges_t[j][i] = e
	# print('-------------')
	# for i in range(n):
	# 	for j in range(n):
	# 		print('i,j = {},{}. {}'.format(i, j, 'Edge' if edges_t[i][j] else 'NONE'))
	# print('-------------')

	ants = [Ant(i) for i in range(n)]

	for t in range(1, t_max+1):
		for k in range(len(ants)):
			ant = ants[k]
			possible = []
			probs = []  # only top 'phe^a * eta^b'
			probs_sum = 0.0

			for i in range(n):
				if i not in ant.visited and i != ant.start and i != ant.town:
					possible.append(i)
					e = edges_t[ant.town][i]
					# print('Edge at {}, {}: {}'.format(ant.town, i, e))
					p = e.phe**alpha * e.eta**beta
					probs_sum += p
					probs.append((p, i))

			if possible:
				probabilities = []
				acc = 0
				for p, i in probs:
					acc += p / probs_sum
					probabilities.append((acc, i))
				# print('Probabilities: {}'.format([round(x[0], 3) for x in probabilities]))

				r = random.random()
				j = -1
				# if probabilities[-1][0] < 100:
				# 	print('<100!!!!! '
				for pp, z in probabilities[:-1]:
					if pp < r:
						j = z
						break
				else:  # for..else
				# if j == -1:
					j = probabilities[-1][1]
				assert j >= 0, "j == -1 :: WTF???"

				e = edges_t[ant.town][j]
				# print('\t!Edge at {}, {}: {}'.format(ant.town, i, e))
				ant.path_len += e.weight
				e.phe_acc += Q/ant.path_len

				edges_t[ant.town][j] = e
				edges_t[j][ant.town] = e
				ant.town = j
				ant.visited.append(j)
			else:  # goto start
				e = edges_t[ant.town][ant.start]
				ant.path_len += e.weight
				e.phe_acc += Q/ant.path_len

				edges_t[ant.town][ant.start] = e
				edges_t[ant.start][ant.town] = e
				ant.town = ant.start
				# ant.visited.append(ant.start)

				# T, L = get_TL(edges_t)
				T, L = [ant.start]+ant.visited+[ant.start], ant.path_len
				if L < L_opt or T_opt == '':
					print('OVERWRITING L_opt={} and T_opt={} WITH:\n\tL={} T=<{}>'.format(L_opt, T_opt, L, T_format(T)))
					L_opt = L
					T_opt = T_format(T)
				if L < L_opt_now or T_opt_now == '':
					L_opt_now = L
					T_opt_now = T_format(T)

				# print('Ant #{} path: {}'.format(k, ant.visited))
				ant.visited = []
				ant.path_len = 0.0

		for i in range(n-1):
			for j in range(i+1, n):
				e = edges_t[i][j]
				e.phe = (1 - rho) * e.phe + e.phe_acc
				e.phe = max(min(e.phe, phe_max), phe_min)
				e.phe_acc = 0.0
				edges_t[i][j] = e
				edges_t[j][i] = e

		if t % (t_max//6) == 0:
			print('t = {}...'.format(t))
	print('Finishing...')

	if _SHOW_PHEROMON_MAP:
		print('Pheromon map:')
		print('****|' + ' |'.join(map(lambda x: '{: >3}'.format(x+1), range(n))))
		for i in range(n):
			print('{: >3} |'.format(i+1) + ' |'.join(map(lambda x: '{: >3.0f}'.format(x.phe) if x else ' - ', edges_t[i])))
	else:
		print('Pheromon map omitted.')
	if _SHOW_ETA_MAP:
		print('Eta map:')
		print('****|' + ' |'.join(map(lambda x: '{: >3}'.format(x+1), range(n))))
		for i in range(n):
			print('{: >3} |'.format(i+1) + ' |'.join(map(lambda x: '{: >3.0f}'.format(100*x.eta) if x else ' - ', edges_t[i])))
	else:
		print('Pheromon map omitted.')

	print('Optimal path global ({:.1f}):\n\t{}'.format(L_opt, T_opt))
	print('Minimal ant`s path found ({:.1f}):\n\t{}'.format(L_opt_now, T_opt_now))
	T, L = get_TL(edges_t)
	print('Pheromon map optimal path ({:.1f}):\n\t{}'.format(L, T_format(T)))

	with open('opt', 'w') as f:
		f.writelines('{}\n{}'.format(L_opt, T_opt))

main()
