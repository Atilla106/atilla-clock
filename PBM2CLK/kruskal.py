# N noeuds du graphe {a,b,c...}
# A aretes et valuations {(a,b,1),(a,c,2)..}
#
# Kruskal (N,A)

def trouver (u, C):
  i = 0
  for c in C:
    if (u in c): return (c,i)
    i += 1

def fusion (C, ucomp, vcomp):
  return [ucomp[0] + vcomp[0]] + [i for j, i in enumerate(C) if j not in [ucomp[1], vcomp[1]]]


def Kruskal (N, A):
  A = sorted(A, key= lambda A: A[2])
  C = [[u] for u in N]
  T = []
  n = len(N)
  for shortestA in A:
    u, v = shortestA[0], shortestA[1]
    ucomp, vcomp = trouver(u, C), trouver(v, C)
    if (ucomp != vcomp):
      print u, v, ":",C
      C = fusion (C, ucomp, vcomp)
      T.append((u,v))
      if (len(T) == (n-1)): break
      
  print "\nArbre couvrant minimal:\n", T
  return T


#  Graphe de test

N = ["a1","a2","a3", "a4", "a5", "a6", "a7"]

A = [("a1","a2",1), ("a1","a4",4),
 ("a2","a4",6), ("a2","a5",4), ("a2","a3",2),
 ("a3","a5",5), ("a3","a6",6),
 ("a4","a5",3), ("a4","a7",4),
 ("a5","a6",8), ("a5","a7",7),
 ("a6","a7",3)]

Kruskal(N, A)