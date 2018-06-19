fp = open('train.tsv')
out = open('train-conv.tsv', 'w')

fp.readline()
for line in fp:
    parsed = line.split('\t')
    string = parsed[2]
    sent = parsed[3].strip('\n')
    out.write(sent + " " + string + "\n")
