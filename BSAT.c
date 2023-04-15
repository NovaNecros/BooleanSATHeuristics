#include<stdio.h>
#include<stdlib.h>
#include<time.h>

const int num_variables = 14; //num variables
const int num_clausulas = 100000; //num clausulas, 0 < num_clausulas <= 3^num_variables

//F.O.: No. de clausulas falsas
int f(int * s, int ** A, int * cont)
{
	int val = num_clausulas;
	++cont[0];
	
	for(int i=0; i<num_clausulas; ++i)
	{
		for(int j=0; j<num_variables; ++j)
		{	
			if(s[j]*A[i][j] > 0)
			{
				--val;
				break;
			}
		}
	}
	
	return val;
}

//copiar arreglos
void copiar(int * copia, int * original, int tam)
{
	for(int i=0; i<tam; ++i)
	{
		copia[i] = original[i];
	}
}

// imprimir arreglos enteros
void imprimir(int * arr, int tam)
{
	printf("(");
	for(int i=0; i<tam; ++i)
	{
		printf("%3d ", arr[i]);
	}
	printf(")");
}

// intercambiar elementos en un arreglo
void swap(int * a, int * b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

// revolver un arreglo
void shuffle(int * arr, int tam)
{
	int j;
	
	for(int i=tam-1; i>0; --i)
	{
		j = rand() % i;
		swap(&arr[i], &arr[j]);
	}
}

// busqieda binaria
// llamado recursivo
double BSrec(double * arr, double x, int inicio, int fin)
{
	int med = (inicio + fin) / 2;
	
	if(x == arr[med] || fin - inicio < 2)
	{
		return med;
	}
	else if(x < arr[med])
	{
		return BSrec(arr, x, inicio, med);
	}
	else
	{
		return BSrec(arr, x, med, fin);
	}
}

// inicializacion
double binarySearch(double * arr, double x, int tam)
{
	return BSrec(arr, x, 0, tam);
}

// Indice del minimo de un arreglo con busqueda secuencial
int argMin(int * arr, int tam)
{
	int indice = 0;
	
	for(int i=1; i<tam; ++i)
	{
		if(arr[i] < arr[indice])
		{
			indice = i;
		}
	}
	
	return indice;
}

// aproximacion a e^x
double exp64(double x)
{
	if(x < -25.0)
	{
		return 0;
	}
	
	double y = x/64;
	
	++y;
	
	y *= y; y *= y; y *= y;
	y *= y; y *= y; y *= y;
	
	return y;
}

// solucion inicial aleatoria
void solucionInicial(int * s0)
{	
	for(int i=0; i<num_variables; ++i)
	{
		s0[i] = 2*(rand() % 2) - 1;
	}
}

// vecino aleatorio, distancia de Hamming k <= 3
void randomVecino(int * s, int * vecino, int k)
{
	int p,q,rn;
	
	copiar(vecino, s, num_variables);
	rn = rand() % k + 1;
		
	for(int i=0; i<rn; ++i)
	{
		do 
		{
			q = rand() % num_variables;
		}
		while(i != 0 && q == p);
		
		vecino[q] = -vecino[q];
		p = q;
	}
}

// Metodo de ascenso a la montana 
int hillClimbing(int ** A, int * s0, int * s, int * cont)
{
	int MAX_ITERA = 1000;
	int ITERA_BL = 5;
	int dist = 1;
	int f0, fs, fv;
	int * vecino = (int *) malloc(num_variables*sizeof(int));
	
	solucionInicial(s0);
	copiar(s,s0,num_variables);
	
	f0 = f(s0, A, cont);
	fs = f0;
	
	printf("\n Metodo: Ascenso a la Montana");
	printf("\n Iteraciones: \t %d", MAX_ITERA);
	printf("\n Iteraciones B.L.: %d, \t Distancia de Hamming: %d", ITERA_BL, dist);
	printf("\n Tam Instancia: \t num_variables = %d variables, \t num_clausulas = %d clausulas", num_variables,num_clausulas);
	printf("\n Solucion inicial: \t s0 = "); imprimir(s0,num_variables);
	printf("\n\t\t\t f(s0) = %d", f0);
	printf("\n\n\t    i \t\tf(actual) \tf(mejor)");
	printf("\n\t %5d \t\t %5d \t\t %5d", 0, f0, fs);
	
	for(int i=0; i<MAX_ITERA; ++i)
	{
		for (int j=0; j<ITERA_BL; ++j)
		{
			randomVecino(s,vecino,dist);
			fv = f(vecino, A, cont);
			
			if(j == 0 || fv <= f0)
			{
				copiar(s0,vecino,num_variables);
				f0 = fv;
			}
		}

		if(f0 < fs)
		{
			copiar(s,s0,num_variables);
			fs = f0;
		}
		

		printf("\n\t %5d \t\t %5d \t\t %5d", i+1, f0, fs);
	}
	
	return fs;
}

// Metodo de recocido simulado
int simulatedAnnealing(int ** A, int * s0, int * s, int * cont)
{
	double T = 1000.0;
	double Tf = 0.01;
	double a = 0.99;
	int ITERA_BL = 5;
	int dist = 1;
	
	int fs,f0,f1,fv,d;
	int * s1, * vecino;
	double rn;
	
	vecino = (int *) malloc(num_variables*sizeof(int));
	s1 = (int *) malloc(num_variables*sizeof(int));
	
	solucionInicial(s0);
	copiar(s, s0, num_variables);
	f0 = f(s0, A, cont);
	fs = f0;
	
	printf("\n: Recocido Simulado");
	printf("\n Parametros: \t T0 = %.3f,\t Tf = %.3f, \ta = %.3f", T, Tf, a);
	printf("\n Tam Instancia: \t num_variables = %d variables, \tm = %d clausulas", num_variables,num_clausulas);
	printf("\n Iteraciones B.L.: %d, \t Distancia de Hamming: %d", ITERA_BL, dist);
	printf("\n Solucion inicial: \t s0 = "); imprimir(s0,num_variables);
	printf("\n\t\t\t f(s0) = %d", f0);
	printf("\n\n\t       T \tf(vecino) \tf(actual) \tf(mejor)");
	printf("\n\t      --- \t   ---    \t %5d \t\t %5d",f0,fs);
	
	while(T >= Tf)
	{
		for (int i=0; i<ITERA_BL; ++i)
		{
			randomVecino(s0,vecino,dist);
			fv = f(vecino, A, cont);
			
			if(i == 0 || fv < f1)
			{
				copiar(s1,vecino,num_variables);
				f1 = fv;
			}
		}
		
		if(f1 < fs)
		{
			copiar( s,s1,num_variables);
			copiar(s0,s1,num_variables);
			fs = f1;
			f0 = f1;
		}
		else if(f1 <= f0)
		{
				copiar(s0,s1,num_variables);
				f0 = f1;
		}
		else //criterio de Metropolis
		{
			d = f1 - f0; //d > 0
			rn = (double) (rand() % RAND_MAX) / (RAND_MAX - 1);
			
			if(rn < exp64(-d/T)) 
			{
				copiar(s0,s1,num_variables);
				f0 = f1;
			}
		}
		
		
		printf("\n\t  %9.3f \t %5d \t\t %5d \t\t %5d",T,f1,f0,fs);	
		
		T *= a;
	}
	
	return fs;
}

// Algoritmo Genetico
// Cruza por parejas
void recombinacion(int ** P, int ** Q, int num_part, int tam_pob, double proba_cruza)
{
	int * part    = (int *) malloc((num_part+2)*sizeof(int));
	int * indices = (int *) malloc(tam_pob*sizeof(int));
	double rn = 0.0;
	
	for(int i=0; i<tam_pob; ++i)
	{
		indices[i] = i;
	}
	shuffle(indices, tam_pob);
	
	for(int i=0; i<tam_pob; i+=2)
	{
		rn = (double) (rand() % RAND_MAX) / (RAND_MAX - 1.0);
		if(rn < proba_cruza)
		{
			part[0] = 0;
			part[num_part+1] = num_variables - 1; 
			for(int l=1; l<=num_part; ++l)
			{
				part[l] = part[l-1] + 1 + (rand() % (num_variables-num_part+l-part[l-1]));
			}
			
			for(int l=1; l<=num_part+1; ++l)
			{
				for(int j=part[l-1]; j<part[l]; ++j)
				{
					Q[i][j] = P[indices[(i+1)-(l%2)]][j];
					Q[i+1][j] = P[indices[i+(l%2)]][j];
				}
			}
		}
		else
		{
			for(int l=0; l<2; ++l)
			{
				copiar(Q[i+l], P[i+l], num_variables);
			}
		}
	}
//	printf("\nAqui hay algo raro:\n\n\t");
//	for(int i=0;i<tam_pob;i++){
//		imprimir(Q[i], num_variables);
//		printf("\n\t");
//	}
//	getchar();
}

// Mutacion gen por gen
void mutacion(int ** P, int tam_pob, double proba_mut)
{
	double rn = 0.0;
	
	for(int i=0; i<tam_pob; ++i)
	{
		for(int j=0; j<num_variables; ++j)
		{
			rn = (double) (rand() % RAND_MAX) / (RAND_MAX - 1.0);
			
			if(rn < proba_mut)
			{
				P[i][j] = -P[i][j];
			}
		}
	}
}

// Seleccion por ruleta
void seleccion(int ** P, int ** Q, int * fP, int * fQ, int tam_pob)
{
	double suma = 0.0, rn = 0.0;
	int   ** R = (int **) malloc(2*tam_pob*sizeof(int *));
	int    * fR    = (int *) malloc(2*tam_pob*sizeof(int));
	double * proba = (double *) malloc(2*tam_pob*sizeof(double));
	int indice, fmin;
	
	for(int i=0; i<tam_pob; ++i)
	{
		R[i] = (int *) malloc(num_variables*sizeof(int));
		R[i+tam_pob] = (int *) malloc(num_variables*sizeof(int));
		fR[i] = fP[i];
		fR[i+tam_pob] = fQ[i];
		copiar(R[i], P[i], num_variables);
		copiar(R[i+tam_pob], Q[i], num_variables);
	}
	
	indice = argMin(fR, 2*tam_pob);
	fmin = fR[indice];
	
	for(int i=0; i<2*tam_pob; ++i)
	{
		proba[i] = (double) tam_pob/((fR[i]-fmin)+tam_pob);
		suma += proba[i];
	}
	
	proba[0] = proba[0] / suma;
	for(int i=1; i<2*tam_pob; ++i)
	{
		proba[i] = proba[i-1] + proba[i]/suma;
	}
	
	//Elitismo
	fP[0] = fmin;
	copiar(P[0], R[indice], num_variables);
	
	//Ruleta
	for(int i=1; i<tam_pob; ++i)
	{
		rn = (double) (rand() % RAND_MAX) / (RAND_MAX - 1);
		indice = binarySearch(proba, rn, 2*tam_pob);
		fP[i] = fR[indice];
		copiar(P[i], R[indice], num_variables);
	}
	
//	printf("\n\n "); imprimir(fR, 2*tam_pob);
//	printf("\n( ");
//	for(int i=0; i<2*tam_pob; ++i)
//	{
//		if(i==0)
//		printf("%.3f ", proba[i]);
//		else
//		printf("%.3f ", proba[i]-proba[i-1]);
//	}
//	printf(")");
}

// Cuerpo principal algoritmo genetico
int geneticAlgorithm(int ** A, int * s, int * cont)
{
	int num_gen = 1000;
	int tam_pob = 10;
	double proba_cruza = 0.5;
	double proba_mut = 0.5;
	int num_part = 2;
	
	int *  f0 = (int  *) malloc(tam_pob*sizeof(int));
	int *  f1 = (int  *) malloc(tam_pob*sizeof(int));
	int ** P0 = (int **) malloc(tam_pob*sizeof(int *));
	int ** P1 = (int **) malloc(tam_pob*sizeof(int *));
	
	//Poblacion Inicial
	for(int i=0; i<tam_pob; ++i)
	{
		P0[i] = (int *) malloc(num_variables*sizeof(int));
		P1[i]  = (int *) malloc(num_variables*sizeof(int));
		solucionInicial(P0[i]);
		f0[i] = f(P0[i], A, cont);
	}
	
	printf("\n Metodo: Algoritmo Genetico");
	printf("\n Parametros:");
	printf("\n\tNumero de Generaciones = %d,\t Tamano de la Poblacion = %d", num_gen, tam_pob);
	printf("\n\tProbabilidad de Cruza = %.3f,\t Probabilidad de Mutacion = %.3f", proba_cruza, proba_mut);
	printf("\n Tamano Instancia: \n\tn = %d variables, \tm = %d clausulas", num_variables, num_clausulas);
	printf("\n\n\n Generacion %d", 0);
	printf("\n\n\t\t\t\t s \t\t\t\t\t   f(s)");
	for(int i=0; i<tam_pob; ++i)
	{
		printf("\n\t"); imprimir(P0[i], num_variables);
		printf("\t %6d",f0[i]);
	}

	
	for(int t=0; t<num_gen; ++t)
	{
		recombinacion(P0, P1, num_part, tam_pob, proba_cruza);
		mutacion(P1, tam_pob, proba_mut);
		
		for(int i=0; i<tam_pob; ++i)
		{
			f1[i] = f(P1[i], A, cont);
		}
		
		seleccion(P0, P1, f0, f1, tam_pob);
		
		if((t+1) % (num_gen/10) == 0)
		{
			printf("\n\n\n Generacion %d", t+1);
			printf("\n\n\t\t\t\t s \t\t\t\t\t   f(s)");
			for(int i=0; i<tam_pob; ++i)
			{
				printf("\n\t"); imprimir(P0[i], num_variables);
				printf("\t %6d",f0[i]);
			}
		}
	}
	
	copiar(s, P0[0], num_variables);
	return f0[0];
}


int main()
{
	srand(time(NULL));
	int * s0, * s, * cont;	
	int fs, op;
	double t;
	FILE * instancia;
	
	s0 = (int *) malloc(num_variables*sizeof(int));
	s = (int *) malloc(num_variables*sizeof(int));
	cont = (int *) malloc(sizeof(int));
	cont[0] = 0;
	
	int ** A = (int **) malloc(num_clausulas*sizeof(int *));
	for(int i=0; i<num_clausulas; ++i)
	{
		A[i] = (int *) malloc(num_variables*sizeof(int));
	}
	
	instancia = fopen("instancia.txt","r");
	for(int i=0; i<num_clausulas; ++i)
	{
		for(int j=0; j<num_variables; ++j)
		{
			fscanf(instancia, "%d, ", &A[i][j]);
		}
	}
	fclose(instancia);

	do
	{
		printf("\n 1) \t Ascenso a la Montana");
		printf("\n 2) \t Recocido Simulado");
		printf("\n 3) \t Algoritmo Genetico\n\n ");
		scanf("%d", &op);
		system("cls");
	}
	while(op < 1 || op > 3);
	
	clock_t start = clock();
	switch(op)
	{
		case 1: 
			fs = hillClimbing(A, s0, s, cont);
			break;
			
		case 2:
			fs = simulatedAnnealing(A, s0, s, cont);
			break;
			
		case 3:
			fs = geneticAlgorithm(A, s, cont);
			
		default:
			break;
	}
	t = (double)(clock()-start) / CLOCKS_PER_SEC;
	
	printf("\n\n\n Mejor Solucion:\t s = "); imprimir(s,num_variables);
	printf("\n Mejor Valor de la F.O.:\t f(s) = %d", fs);
	printf("\n\n No. de llamadas a la F.O.:\t %d", cont[0]);
	printf("\n Tiempo de Ejecucion:\t %f segundos", t);
	
	return 69;
}


