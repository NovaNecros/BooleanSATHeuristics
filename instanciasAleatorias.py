import random as rn
import time as tm

#generador de instancias satisfacibilidad

def generador(n):
    m = 3**n
    print(m,"\n\n")
    
    A = [[(i//3**j)%3-1 for j in range(n)] for i in range(m)]
    rn.shuffle(A)
	
    ins = open("instancia11.txt", "w")
    for i in range(m):
        for j in range(n):
            ins.write(str(A[i][j]))
            if j < n-1:
                ins.write(",")
        ins.write("\n")
    ins.close()
		
n = 11
rn.seed()
start = tm.time()			

generador(n)

print("\n\n\nTiempo de ejecucion:", tm.time()-start, "segundos")

