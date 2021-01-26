import numpy
import scipy
from scipy import sparse
from scipy.sparse import linalg
import threading

DIM = 90
PTS = 100000
K = 10
dsvd = 1
U = []
s = []
v = []
def client(ID):
	f = open('YPMSD'+str(ID)+'.in', 'r')
	tmp = [[float(j) for j in i.split()] for i in f.readlines()]
	v.append(tmp)
	f.close()
	a = numpy.array(tmp)
	tU,ts,tV = linalg.svds(a, k=dsvd+1) 
	U.append(tU)
	s.append(ts)

client(1)
client(2)
client(3)

lrv = scipy.dot(scipy.mat(U[0]),sparse.diags(s[0]).toarray())
data = [[float(lrv[:,i][j]) for i in range(dsvd + 1)] for j in range(PTS)]
lrv = scipy.dot(scipy.mat(U[1]),sparse.diags(s[1]).toarray())
for i in range(PTS):
	for j in range(dsvd+1):
		data[i].append(float(lrv[:,j][i]))
lrv =  scipy.dot(scipy.mat(U[2]),sparse.diags(s[2]).toarray())
for i in range(PTS):
	for j in range(dsvd+1):
		data[i].append(float(lrv[:,j][i]))

sU,ss,sV = linalg.svds(data, k=dsvd)

lrv = scipy.mat(sU).dot(scipy.mat(sU).T.dot(scipy.mat(v[0])))
data = [[float(lrv[:,i][j]) for i in range(dsvd)] for j in range(PTS)]
lrv = scipy.mat(sU).dot(scipy.mat(sU).T.dot(scipy.mat(v[1])))
for i in range(PTS):
	for j in range(dsvd):
		data[i].append(float(lrv[:,j][i]))
lrv = scipy.mat(sU).dot(scipy.mat(sU).T.dot(scipy.mat(v[2])))
for i in range(PTS):
	for j in range(dsvd):
		data[i].append(float(lrv[:,j][i]))

f = open('YPMSD'+str(K)+'_'+str(dsvd)+'.in', 'w')
for item in data:
	f.writelines(["%s " %i for i in item])

