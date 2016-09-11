# TAGS: Needleman-Wunsch algorithm, global alignment, dna, sequence, dynamic programming

s1 = 'GCATGCU'
s2 = 'GATTACA'
assert len(s1) == len(s2), "please use strings with the same size :c"
n = len(s1)


def score(a, b):
    if a == b:
        return 1
    else:
        return -1

grid = [list(-i for i in range(n + 1))]
for i in range(1, n + 1):
    grid.append([-i] + [0 for _ in range(n)])
# for row in grid:
#     print(' '.join(map(lambda x: '{: 2}'.format(x), row)))

# Fill in the Table
for i in range(1, n + 1):
    for j in range(1, n + 1):
        grid[i][j] = max(grid[i - 1][j] - 1, grid[i][j - 1] - 1, grid[i - 1][j - 1] + score(s1[j - 1], s2[i - 1]))

print('Filled grid:')
print('  '.join('  ' + s1))
for i, row in enumerate(grid):
    print((' ' + s2)[i] + ' ' + ' '.join(map('{: 2}'.format, row)))

# Backtrace:
s1_aligned = ''
s2_aligned = ''
i = n
j = n
while i > 0 and j > 0:
    gap1, gap2, mmatch = grid[i][j - 1], grid[i - 1][j], grid[i - 1][j - 1]

    if mmatch >= gap1 and mmatch >= gap2:
        s1_aligned = s1[j - 1] + s1_aligned
        s2_aligned = s2[i - 1] + s2_aligned
        i -= 1
        j -= 1
    elif gap1 >= gap2 and gap1 >= mmatch:
        s1_aligned = '-' + s1_aligned
        s2_aligned = s2[i - 1] + s2_aligned
        j -= 1
    else:
        s1_aligned = s1[j - 1] + s1_aligned
        s2_aligned = '-' + s2_aligned
        i -= 1

print('Aligned sequences:\n\t{}\n\t{}'.format(s1_aligned, s2_aligned))
