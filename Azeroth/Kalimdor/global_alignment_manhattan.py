# TAGS: Needleman-Wunsch algorithm, global alignment, dna, sequence, dynamic programming, affine gap, 3 layer Manhattan grid
import math

use_blosum62 = False
use_matrix = False


def score(a, b):
    if use_blosum62:
        BLOSUM62_raw = '''
        4 -1 -2 -2  0 -1 -1  0 -2 -1 -1 -1 -1 -2 -1  1  0 -3 -2  0 -2 -1  0 -4
        -1  5  0 -2 -3  1  0 -2  0 -3 -2  2 -1 -3 -2 -1 -1 -3 -2 -3 -1  0 -1 -4
        -2  0  6  1 -3  0  0  0  1 -3 -3  0 -2 -3 -2  1  0 -4 -2 -3  3  0 -1 -4
        -2 -2  1  6 -3  0  2 -1 -1 -3 -4 -1 -3 -3 -1  0 -1 -4 -3 -3  4  1 -1 -4
         0 -3 -3 -3  9 -3 -4 -3 -3 -1 -1 -3 -1 -2 -3 -1 -1 -2 -2 -1 -3 -3 -2 -4
        -1  1  0  0 -3  5  2 -2  0 -3 -2  1  0 -3 -1  0 -1 -2 -1 -2  0  3 -1 -4
        -1  0  0  2 -4  2  5 -2  0 -3 -3  1 -2 -3 -1  0 -1 -3 -2 -2  1  4 -1 -4
         0 -2  0 -1 -3 -2 -2  6 -2 -4 -4 -2 -3 -3 -2  0 -2 -2 -3 -3 -1 -2 -1 -4
        -2  0  1 -1 -3  0  0 -2  8 -3 -3 -1 -2 -1 -2 -1 -2 -2  2 -3  0  0 -1 -4
        -1 -3 -3 -3 -1 -3 -3 -4 -3  4  2 -3  1  0 -3 -2 -1 -3 -1  3 -3 -3 -1 -4
        -1 -2 -3 -4 -1 -2 -3 -4 -3  2  4 -2  2  0 -3 -2 -1 -2 -1  1 -4 -3 -1 -4
        -1  2  0 -1 -3  1  1 -2 -1 -3 -2  5 -1 -3 -1  0 -1 -3 -2 -2  0  1 -1 -4
        -1 -1 -2 -3 -1  0 -2 -3 -2  1  2 -1  5  0 -2 -1 -1 -1 -1  1 -3 -1 -1 -4
        -2 -3 -3 -3 -2 -3 -3 -3 -1  0  0 -3  0  6 -4 -2 -2  1  3 -1 -3 -3 -1 -4
        -1 -2 -2 -1 -3 -1 -1 -2 -2 -3 -3 -1 -2 -4  7 -1 -1 -4 -3 -2 -2 -1 -2 -4
         1 -1  1  0 -1  0  0  0 -1 -2 -2  0 -1 -2 -1  4  1 -3 -2 -2  0  0  0 -4
         0 -1  0 -1 -1 -1 -1 -2 -2 -1 -1 -1 -1 -2 -1  1  5 -2 -2  0 -1 -1  0 -4
        -3 -3 -4 -4 -2 -2 -3 -2 -2 -3 -2 -3 -1  1 -4 -3 -2 11  2 -3 -4 -3 -2 -4
        -2 -2 -2 -3 -2 -1 -2 -3  2 -1 -1 -2 -1  3 -3 -2 -2  2  7 -1 -3 -2 -1 -4
         0 -3 -3 -3 -1 -2 -2 -3 -3  3  1 -2  1 -1 -2 -2  0 -3 -1  4 -3 -2 -1 -4
        -2 -1  3  4 -3  0  1 -1  0 -3 -4  0 -3 -3 -2  0 -1 -4 -3 -3  4  1 -1 -4
        -1  0  0  1 -3  3  4 -2  0 -3 -3  1 -1 -3 -1  0 -1 -3 -2 -2  1  4 -1 -4
         0 -1 -1 -1 -2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -2  0  0 -2 -1 -1 -1 -1 -1 -4
        -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4  1
        '''
        BLOSUM62 = [list(map(int, row.split())) for row in BLOSUM62_raw.strip().split('\n')]
        LETTERS = 'ARNDCQEGHILKMFPSTWYVBZX*'
        x = LETTERS.find(a)
        y = LETTERS.find(b)

        return BLOSUM62[x][y]
    elif use_matrix:
        letters = 'ARNK'
        matrix_raw = '''
         5 -2 -1 -1
        -2  7 -1  3
        -1 -1  7  0
        -1  3  0  6
        '''
        matrix = [list(map(int, row.split())) for row in matrix_raw.strip().split('\n')]
        x = letters.index(a)
        y = letters.index(b)
        return matrix[x][y]
    else:
        coef_match = 1
        coef_mismatch = -1
        return coef_match if a == b else coef_mismatch


def align(s1, s2):
    n = len(s1)
    m = len(s2)

    coef_gap_open = -11
    coef_gap_extend = -1

    M = [[0 for _ in range(m + 1)] for _ in range(n + 1)]
    X = [[0 for _ in range(m + 1)] for _ in range(n + 1)]
    Y = [[0 for _ in range(m + 1)] for _ in range(n + 1)]
    # M[i][j] = score of best alignment x[:i] and y[:j] ending with (mis)match
    # X[i][j] = score of best alignment x[:i] and y[:j] ending with gap in s1
    # Y[i][j] = score of best alignment x[:i] and y[:j] ending with gap in s2
    traceback_M = [['.' for _ in range(m + 1)] for _ in range(n + 1)]
    traceback_X = [['.' for _ in range(m + 1)] for _ in range(n + 1)]
    traceback_Y = [['.' for _ in range(m + 1)] for _ in range(n + 1)]

    for i in range(1, n + 1):
        M[i][0] = -math.inf
        X[i][0] = -math.inf
        Y[i][0] = coef_gap_open + coef_gap_extend * (i - 1)
        traceback_Y[i][0] = 'Y'
    for j in range(1, m + 1):
        M[0][j] = -math.inf
        X[0][j] = coef_gap_open + coef_gap_extend * (j - 1)
        traceback_X[0][j] = 'X'
        Y[0][j] = -math.inf

    # Fill the grid
    for i in range(1, n + 1):
        c1 = s1[i - 1]
        for j in range(1, m + 1):
            c2 = s2[j - 1]

            (traceback_M[i][j], M[i][j]) = max((
                ('M', score(c1, c2) + M[i - 1][j - 1]),
                ('X', score(c1, c2) + X[i - 1][j - 1]),
                ('Y', score(c1, c2) + Y[i - 1][j - 1])), key=lambda x: x[1])
            (traceback_X[i][j], X[i][j]) = max((
                ('M', coef_gap_open + M[i][j - 1]),
                ('X', coef_gap_extend + X[i][j - 1]),
                ('Y', coef_gap_open + Y[i][j - 1])), key=lambda x: x[1])
            (traceback_Y[i][j], Y[i][j]) = max((
                ('M', coef_gap_open + M[i - 1][j]),
                ('X', coef_gap_open + X[i - 1][j]),
                ('Y', coef_gap_extend + Y[i - 1][j])), key=lambda x: x[1])

    def print_matrix(name, data):
        print('{} matrix:'.format(name))
        print('    ' + '  '.join('$' + s2))
        for i, row in enumerate(data):
            print(('$' + s1)[i] + ' ' + ''.join(map('{: 3}'.format, row)))

    print_matrix('M', M)
    print_matrix('X', X)
    print_matrix('Y', Y)

    def print_traceback(name, data):
        print('Filled traceback_{}:'.format(name))
        print('  ' + ' '.join('$' + s2))
        for i, row in enumerate(traceback_M):
            print(('$' + s1)[i] + ' ' + ' '.join(row))

    print_traceback('M', traceback_M)
    print_traceback('X', traceback_X)
    print_traceback('Y', traceback_Y)

    s1_aligned = ''
    s2_aligned = ''
    i = n
    j = m
    value, current = max((
        (M[n][m], 'M'),
        (X[n][m], 'X'),
        (Y[n][m], 'Y')), key=lambda x: x[0])

    while i > 0 or j > 0:
        if current == 'M':
            c = traceback_M[i][j]
            s1_aligned += s1[i - 1]
            s2_aligned += s2[j - 1]
            i -= 1
            j -= 1
            current = c
        elif current == 'X':
            c = traceback_X[i][j]
            s1_aligned += '-'
            s2_aligned += s2[j - 1]
            j -= 1
            current = c
        elif current == 'Y':
            c = traceback_Y[i][j]
            s1_aligned += s1[i - 1]
            s2_aligned += '-'
            i -= 1
            current = c
        else:
            raise ValueError('current = {}'.format(current))

    return s1_aligned[::-1], s2_aligned[::-1], value


def main():
    import random
    n = 10
    m = 15
    if use_blosum62:
        alphabet = 'ARNDCQEGHILKMFPSTWYVBZX'
    elif use_matrix:
        alphabet = 'ARNK'
    else:
        alphabet = 'ATGC'
    s1 = ''.join(random.choice(alphabet) for _ in range(n))
    s2 = ''.join(random.choice(alphabet) for _ in range(m))
    # s1 = 'PRTEINS'  # score: 8
    # s2 = 'PRTWPSEIN'
    # s1 = 'GCATGCT'
    # s2 = 'GATTACA'
    print('Initial sequences:\n\t{}\n\t{}'.format(s1, s2))

    s1_aligned, s2_aligned, score = align(s1, s2)
    print('Aligned sequences:\n\t{}\n\t{}\nScore: {}'.format(s1_aligned, s2_aligned, score))

if __name__ == '__main__':
    main()
