import sys
import os

if len(sys.argv) == 2:

    path, ext = os.path.splitext(sys.argv[1])

    fp = open(sys.argv[1])

    newPath = path + "-converted" + ext
    out = open(newPath, 'w')

    fp.readline()
    for line in fp:
        parsed = line.split('\t')
        string = parsed[2]
        sent = parsed[3].strip('\n')
        out.write(sent + " " + string + "\n")

else:
    print("No file given.")
