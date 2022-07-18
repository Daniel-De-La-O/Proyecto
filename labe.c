#include <stdio.h>
#include <stdlib.h>

//Aquí se guardan las variables.
char** laberintoE;
int** laberintoI;
int i, j;
int filas;
int fila;
int PrimFila;
int columna;
int columnas;
int PrimColum;
//Para el layout del laberinto se usa enumeración.
enum layout {espacio, pared, meta, camino};

//Se asigna el espacio del laberinto en la memoria
void asignarlaberintoE()
{
//malloc asigna la memoria solicitada y le devuelve un puntero.
	laberintoE = malloc(filas * sizeof(char*));
//Lo mismo para columnas
	for (i = 0; i < filas; ++i){
		laberintoE[i] = malloc(columnas * sizeof(char*));
	}
}
//Se hace lo mismo
void asignarlaberintoI()
{
	laberintoI = malloc(filas * sizeof(int*));
	for (i = 0; i < filas; ++i){
		laberintoI[i] = malloc(columnas * sizeof(int*));
	}
}

//Aquí se recibe el archivo. 
void laberintoEF(char* nombre) {
	char car;
//Para nominar el valor máximo de filas y columnas que puede haber (que tendrá un valor de x-1 dado que el último es la terminación en nulo del string). 
//No se qué tan grande será el laberinto introducido, pero voy a asumir que no se pasa de 99999 caracteres, por lo que se desgina 6.
	char filasM[6] = {'\0'};
	char columM[6] = {'\0'};
//Para identificar las filas y columnas y saber cuando moverse de una a la otra al recorrer el laberinto.
	int filasP = 0;
	int columP = 0;
	int cambio = 0;
//Se abre y lee el archivo.
	FILE* archivo = fopen(nombre, "r");  
//Iterador  
	if (archivo) {
//Se lee y si no se detecta que es el final del archivo (EOF), sigue leyendo.        
		while ((car = getc(archivo)) != EOF) {
			if (car == '\n') {
				break;
//Se detecta la coma de la primera línea, o sea, se detecta que se cambia de fila a columna.
			} else if (car == ','){
				cambio = 1;
//Donde se encuentre el caracter "car" con el iterador, se va a grabar con el string de filasM y después se sigue a la siguiente posición. Lo mismo para columnas.
			} else if (!cambio) {
				filasM[filasP] = car;
				filasP++;
			} else {
				columM[columP] = car;
				columP++;
			}
		}
//Caso en el que no se presente un archivo de laberinto.
	} else {
		printf("No hay archivo");
		return;
	}
//La función atoi transforma el argumento de string en un entero. 
	filas = atoi(filasM);
	columnas = atoi(columM);
//Se llama de nuevo a la función
	asignarlaberintoE();
//Iteración (ya se está empezando en el laberinto)
	for (i = 0; i < filas; ++i) {
		for (j = 0; j < columnas; ++j) {
//Si se llega a una nueva línea se ignora para no restaurar un carácter de la línea nueva en el arreglo.
			if ((car = getc(archivo)) == '\n') {
				car = getc(archivo);
			}
//Ahora se establece laberinto[i][j] igual al caracter que se usó.
			laberintoE[i][j] = car;
//Saber donde se empieza
			if (car =='1') {
				PrimFila = i;
				PrimColum = j;
			}
		}
	}
//Cerrar el archivo
	fclose(archivo);
}
//Analizar la estructura interna del laberinto
void laberintoIF() {
	asignarlaberintoI();

	for (i = 0; i < filas; ++i) {
		for (j = 0; j < columnas; ++j) {
//Si se detecta un 0, se sabe que es una parede
			if (laberintoE[i][j] == '0') {
				laberintoI[i][j] = pared;
//Si se detecta un 2, se sabe que es la meta
			} else if (laberintoE[i][j] == '2') {
				laberintoI[i][j] = meta;
//Si no se detecta nada, se sabe que es espacio vacío.
			} else {
				laberintoI[i][j] = espacio;
			}
		}
	}
}	
//Función para imprimir el laberinto
void LaberintoF()
{
	for (i = 0; i < filas; ++i) {
		for (j = 0; j < columnas; ++j) {
			printf("%c", laberintoE[i][j]);
		}
//Imprimir bien
        printf("\n");
	}	
}
//El camino de 1s
void Ruta()
{
	for (i = 0; i < filas; ++i) {
		for (j = 0; j < columnas; ++j) {
//Por el mismo uso de variable, se puede enredar un poco, pero lo que significa es que si laberintoE no es la entrada
//y laberintoI es el camino, entonces laberintoE se representará con un 1 de camino.
			if (laberintoE[i][j] != '1') {
                if (laberintoI[i][j] == camino) {
                    laberintoE[i][j] = '1';
                }    
			}   
		}
	}
}
//DFS = Deep First Search, es una técnica donde el usuario atraviesa con el nodo inicial del gráfico y luego profundiza hasta que el usuario encuentra el nodo requerido. 
//Funciona bien para navegar laberintos.
int DFS(fila, columna) {
//Se graba donde se encuentra uno basado en la entrada y se asigna un puntero a laberintoI para analizar la coordenada.
	int* loc = &laberintoI[fila][columna];
//Si loc está en la meta, regresa TRUE.
	if (*loc == meta) {
		return 1;
	}
///Si loc está en un espacio vacío que no es parte de la solución, lo toma como pared y no pone nada
	if (*loc == espacio) {
		*loc = pared;
//Para los siguientes 4, si llegan a la solución, se deja un 1 por la ruta más simple por donde se paso 
		if (DFS(fila, columna - 1)){
			*loc = camino;
			return 1;
		} 
		if (DFS(fila + 1, columna)){
			*loc = camino;
			return 1;
		} 
		if (DFS(fila, columna + 1)){
			*loc = camino;
			return 1;		
		} 
		if (DFS(fila - 1, columna)){
			*loc = camino;
			return 1;
		}
	}
//Si no se está en un espacio vacío es porque está en una pared así que regresa 0.
	return 0;
}

//Resolver e imprimir el laberinto
int main() {
	laberintoEF("laberinto.txt");
	laberintoIF();
//Si no se encuentra una entrada, no habrá un camino
	if (!DFS(PrimFila, PrimColum)) {
		printf("No se encontró un camino.\n");
//Caso contrario, se traza la ruta
	} else {
		Ruta();
		LaberintoF();
	}
	return 0;
}