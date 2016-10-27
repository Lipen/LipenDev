# path = 'seqs/seqERV3-1_macaque_chr2.fasta'
# pos_ltr5 = (96109, 95584)
# pos_ltr3 = (88903, 88379)
# path = 'seqs/seqERV3-1_pan.fasta'
# pos_ltr5 = (579, 1190)
# pos_ltr3 = (9604, 10206)
# path = 'seqs/seqERVIPF-10H.fasta'
# pos_ltr5 = (5377, 4938)
# pos_ltr3 = (1064, 619)
path = 'seqs/seqERV3-1_macaque_chr3.fasta'
pos_ltr5 = (2194, 2803)
pos_ltr3 = (11225, 11824)

with open(path) as f:
    data = ''.join(line.strip() for line in f.readlines()[1:])

i, j = pos_ltr5
if i < j:
    ltr5 = data[i - 1:j]
else:
    ltr5 = data[j - 1:i][::-1]

i, j = pos_ltr3
if i < j:
    ltr3 = data[i - 1:j]
else:
    ltr3 = data[j - 1:i][::-1]

print('>5\'LTR\n{}\n>3\'LTR\n{}'.format(ltr5, ltr3))
