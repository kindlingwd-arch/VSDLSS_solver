#!/usr/bin/env python3
# gen_sparse.py — write a VSDLSS matrix in the RedHawk .hdr/.mat*/.rhs format.
# Matrix: n=10 tridiagonal SPD (diag=2, off=-1), b = A*ones => solution = ones.
import struct, sys

job = sys.argv[1] if len(sys.argv) > 1 else "test_sparse"
n = int(sys.argv[2]) if len(sys.argv) > 2 else 10
diag = [2.0]*n
o = n-1
rows=[]; cols=[]; vals=[]
for i in range(1,n):          # 1-based upper off-diagonals (i, i+1)
    rows.append(i); cols.append(i+1); vals.append(-1.0)
assert len(rows)==o

def w(ext, data):
    with open(job+ext,"wb") as f: f.write(data)

w(".matd", struct.pack("<%dd"%n, *diag))
w(".matf", struct.pack("<%di"%o, *rows))
w(".matt", struct.pack("<%di"%o, *cols))
w(".mato", struct.pack("<%dd"%o, *vals))

# rhs = A*ones -> [1,0,...,0,1]
rhs=[]
for i in range(1,n+1):
    nb = (1 if i>1 else 0)+(1 if i<n else 0)
    rhs.append(2.0 - nb*1.0)
w(".rhs", struct.pack("<%dd"%n, *rhs))

hdr = "n %d\no %d\nm 0\na 100\nk 3\nz 1\nr 1\nc 0\nx 0\nu 0\n" % (n,o)
with open(job+".hdr","w") as f: f.write(hdr)
with open(job+".CLG","w") as f: f.write(hdr)

print("wrote %s.hdr/.matd/.matf/.matt/.mato/.rhs (n=%d, o=%d)" % (job,n,o))
print("expected solution x = ones")
