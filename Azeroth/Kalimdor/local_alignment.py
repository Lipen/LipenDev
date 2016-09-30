# TAGS: Smith-Waterman algorithm, local alignment, dna, sequence, dynamic programming
import sys

use_blosum62 = False
use_matrix = False
colorize = sys.stdout.isatty()


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

    coef_indel = -1  # gap penalty (linear)

    grid = [[0 for _ in range(n + 1)] for _ in range(m + 1)]
    traceback = [['' for _ in range(n + 1)] for _ in range(m + 1)]
    # s1 = GCATGCU
    # s2 = GATTACA
    #  i-> 0   1  2  3  4  5  6  7@n
    # j    $   G  C  A  T  G  C  U
    # 0 $  0  -1 -2 -3 -4 -5 -6 -7
    #        _____________________
    # 1 G -1|  1  0 -1 -2 -3 -4 -5
    # 2 A -2|  0  0  1  0 -1 -2 -3
    #   ... |
    # m A -7| -5 -3 -1 -2 -2  0  0@answer
    for i in range(1, n + 1):
        grid[0][i] = -i
        traceback[0][i] = '-'
    for j in range(1, m + 1):
        grid[j][0] = -j
        traceback[j][0] = '|'
    traceback[0][0] = '@'

    # Fill the grid
    for j in range(1, m + 1):
        c2 = s2[j - 1]
        for i in range(1, n + 1):
            c1 = s1[i - 1]
            score_start = (0, '@')
            # score_start = (max(0, score(c1, c2)), '@')
            score_match = (grid[j - 1][i - 1] + score(c1, c2), '\\')
            score_delete = (grid[j][i - 1] + coef_indel, '-')
            score_insert = (grid[j - 1][i] + coef_indel, '|')
            scores = (score_start, score_match, score_delete, score_insert)

            score_max, arror = max(scores, key=lambda x: x[0])
            grid[j][i] = score_max
            traceback[j][i] = arror

    print('Filled grid:')
    print('       ' + '  '.join(s1))
    for i, row in enumerate(grid):
        print((' ' + s2)[i] + ' ' + ''.join(map('{:3}'.format, row)))
    print('Filled traceback:')
    print('    ' + ' '.join(s1))
    for i, row in enumerate(traceback):
        print((' ' + s2)[i] + ' ' + ''.join(map('{:2}'.format, row)))

    # Find end of local alignment == max in grid
    end = (0, 0, 0)
    for j in range(1, m + 1):
        for i in range(1, n + 1):
            v = grid[j][i]
            if v > end[2]:
                end = (i, j, v)
    print('End at {} with value {}'.format((end[0], end[1]), end[2]))

    # Local alignment
    s1_align = ''
    s2_align = ''
    i, j, *_ = end
    while i > 0 or j > 0:
        c = traceback[j][i]
        if c == '|':
            s1_align += '-'
            s2_align += s2[j - 1]
            j -= 1
        elif c == '\\':
            s1_align += s1[i - 1]
            s2_align += s2[j - 1]
            i -= 1
            j -= 1
        elif c == '-':
            s1_align += s1[i - 1]
            s2_align += '-'
            i -= 1
        elif c == '@':
            s1_align += s1[i - 1]
            s2_align += s2[j - 1]
            break
        else:
            raise "WTF"

    s1_align = s1_align[::-1]
    s2_align = s2_align[::-1]
    s1_left = s1[:i][:-1]  # need prefix 0..i-2, so cut 0..i-1 first,
    s2_left = s2[:j][:-1]  # then cut one more char (order is important!)
    s1_right = s1[end[0]:]
    s2_right = s2[end[1]:]
    pad_left = max(len(s1_left), len(s2_left))
    pad_right = max(len(s1_right), len(s2_right))
    s1_left_pad = '=' * (pad_left - len(s1_left))
    s2_left_pad = '=' * (pad_left - len(s2_left))
    s1_right_pad = '=' * (pad_right - len(s1_right))
    s2_right_pad = '=' * (pad_right - len(s2_right))
    s1_delim = '.'
    s2_delim = '^'

    if colorize:
        s1_align = '\033[92m' + s1_align + '\033[0m'
        s2_align = '\033[92m' + s2_align + '\033[0m'
        s1_left = '\033[91m' + s1_left + '\033[0m'
        s2_left = '\033[91m' + s2_left + '\033[0m'
        s1_right = '\033[91m' + s1_right + '\033[0m'
        s2_right = '\033[91m' + s2_right + '\033[0m'
        s1_left_pad = '\033[94m' + s1_left_pad + '\033[0m'
        s2_left_pad = '\033[94m' + s2_left_pad + '\033[0m'
        s1_right_pad = '\033[94m' + s1_right_pad + '\033[0m'
        s2_right_pad = '\033[94m' + s2_right_pad + '\033[0m'
        s1_delim = '\033[93m' + s1_delim + '\033[0m'
        s2_delim = '\033[93m' + s2_delim + '\033[0m'

    s1_aligned = '{left}{delim}{align}{delim}{right}'.format(
        left=s1_left_pad + s1_left,
        align=s1_align,
        right=s1_right + s1_right_pad,
        delim=s1_delim)
    s2_aligned = '{left}{delim}{align}{delim}{right}'.format(
        left=s2_left_pad + s2_left,
        align=s2_align,
        right=s2_right + s2_right_pad,
        delim=s2_delim)

    return (s1_aligned, s2_aligned, end[2])


def main():
    import random
    n = 10
    m = 10
    if use_blosum62:
        alphabet = 'ARNDCQEGHILKMFPSTWYVBZX'
    elif use_matrix:
        alphabet = 'ARNK'
    else:
        alphabet = 'ATGC'
    s1 = ''.join(random.choice(alphabet) for _ in range(n))
    s2 = ''.join(random.choice(alphabet) for _ in range(m))
    # s1 = 'AAAAAAAAAAATGCATG'
    # s2 = 'ATGCCCCCCCCCC'
    # s1 = "TATGGAGGAT"
    # s2 = "ACCTAGAACT"
    print('Initial sequences:\n\t{}\n\t{}'.format(s1, s2))

    s1_aligned, s2_aligned, score = align(s1, s2)
    print('Aligned sequences:\n\t{}\n\t{}\nScore: {}'.format(s1_aligned, s2_aligned, score))

if __name__ == '__main__':
    main()
