import numpy as np
import matplotlib.pyplot as plt
from sklearn import manifold

with open('opt', 'r') as f:
	raw = f.read().splitlines()
	ds_name = raw[0]
	L_opt = int(raw[1])
	T_opt = np.array(list(map(int, raw[2].split())))

with open(ds_name + '.txt', 'r') as f:
	dists = [list(map(float, line.split())) for line in f.readlines()]
n = len(dists)

cities = [i+1 for i in range(n)]

adist = np.array(dists)
amax = np.amax(adist)
adist /= amax

print('Calculating...')
mds = manifold.MDS(n_components=2, dissimilarity="precomputed")  # random_state=
results = mds.fit(adist)
print('Calculated.')

coords = results.embedding_
X, Y = coords[:, 0], coords[:, 1]

print('Plotting...')
plt.subplots_adjust(top=0.95, bottom=0.05, left=0.05, right=0.95)
plt.scatter(X, Y, marker='o')
plt.title(ds_name, fontsize=20)
# plt.axis('square')

# for label, x, y in zip(cities, coords[:, 0], coords[:, 1]):
# 	plt.annotate(
# 		label, xy=(x, y), xytext=(random.randint(30, 40)*random.choice([-1, 1]), random.randint(30, 40)*random.choice([-1, 1])),
# 		textcoords='offset points', ha='center', va='center',
# 		bbox=dict(boxstyle='round,pad=0.3', fc='yellow', alpha=0.3),
# 		arrowprops=dict(arrowstyle='->', connectionstyle='arc3,rad=0', alpha=0.4))

for i in range(n):
	a, b = T_opt[i]-1, T_opt[i+1]-1
	xa, ya = X[a], Y[a]
	xb, yb = X[b], Y[b]
	plt.annotate(
		"", xy=(xb, yb), xytext=(xa, ya),
		arrowprops=dict(
			arrowstyle='->',
			connectionstyle='arc3,rad=0',
			alpha=0.6))
	# plt.arrow(xa, ya, xb-xa, yb-ya, head_width=0.01, head_length=0.03, fc='k', ec='k')

print('Done. Showing...')
plt.show()
