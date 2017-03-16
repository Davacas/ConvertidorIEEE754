#include <stdio.h>
#include <stdlib.h>

//Prototipos de funciones.
void menu(); 
void bin_a_dec();
void dec_a_bin();

//Función main. Sólo lanza el menú.
int main () {
	menu();
	return 0;
}

//Definición de función que hace funcionar un menú.
void menu (){
	float num = 0;
	short opc;

	//Se le dan las opciones de conversión al usuario y elige una.
	printf("¿Qué deseas hacer?\n");
	printf("\t1) Convertir de decimal a coma flotante de precisión simple.\n");
	printf("\t2) Convertir de coma flotante con precisión simple a decimal.\n" );
	printf("Tu opción: ");
	scanf("%hd", &opc);

	//El programa manda llamar la función necesaria de acuerdo a la opción del usuario.
	switch(opc) {
		case 1:
			dec_a_bin(num);
			break;
		case 2:
			bin_a_dec(num);
			break;
		default:
			printf("Esa opción no es válida.\n");
			printf("Por favor, vuelve a ejecutar el programa e ingresa una opción válida.\n");
	}

	return;
}

//Función que convierte de decimal a binario.
void dec_a_bin() {
	//Variables auxiliares para los cálculos.
	short bits_int = 0;	//Contador de bits de la parte entera.
	short bits_dec = 0; //Contador de bits de la parte decimal.
	short exp = 0;		//Valor decimal del exponente.
	short num_bin[32];	//Conversión temporal del decimal a 32 bits
	int i, j, entero;	//Contadores y parte entera del decimal ingresado.
	float decimal;		//Parte fraccionaria del decimal ingresado.
	float dec;			//Decimal ingresado.
	//Arreglos que almacenarán el número binario final.
	short signo;
	short exponente[8];
	short mantisa[23];

	//Se solicita un número al usuario y se almacena.
	printf("\nIngresa un número real: ");
	scanf("%f", &dec);

	//Se ve si el número ingresado es positivo o negativo y se asigna el bit de signo.
	if (dec >= 0) 
		signo = 0;
	else {
		dec = dec*-1; 		//De ser negativo, se obtiene su valor absoluto.
		signo = 1;
	}

	entero = (int) dec;		//Se obtiene la parte entera.
	decimal = dec - entero;	//Se obtiene la parte fraccionaria.	

	//Se hace la conversión de la parte entera decimal a binario por medio de divisiones.
	while (entero > 0) {
		if ((entero % 2) == 1)
			num_bin[bits_int] = 1;
		else
			num_bin[bits_int] = 0;
		entero /= 2;
		bits_int ++; //Se va contando el número de bits que ocupa la parte entera.
	}

	//Se asigna el valor binario real obtenido anteriormente a la mantisa, puesto que sale al revés.
	//Se omite el valor del bit más significativo como indica el algoritmo.
	for (i = 1; i < bits_int; i++)
		mantisa[i-1] = num_bin[bits_int-i-1];
	bits_dec = bits_int-1; //Se respalda el número de bits del entero para luego calcular el exponente.

	//Si existe la parte decimal, se hace su conversión a binario por medio de multiplicaciones.
	//El binario que va resultando se almacena directo a la mantisa.
	if (decimal > 0.0) {
		while (1) {
			decimal *= 2;
			if (decimal > 1) {
				mantisa[bits_dec] = 1;
				decimal -= 1;
			}
			else if (decimal < 1)
				mantisa[bits_dec] = 0;
			else
				break;
			bits_dec ++;
		}
		mantisa[bits_dec] = 1; //Se almacena un uno provocado por la última división.
	}
	else
		mantisa[bits_dec] = 0; //Si no hay fraccionarios, se asigna un 0 a la mantisa.

	//Se llena de 0s el resto de la mantisa.
	for (i = bits_dec+1; i < 23; i++)
		mantisa[i] = 0;
	//Hasta este punto, la mantisa ya tiene el valor que le corresponde.

	exp = 127 + bits_int - 1; //Se obtiene el valor del exponente para el formato IEEE 754.
	//Se hace la conversión de este exponente a binario por medio de divisiones.
	i = 0;
	while (exp > 0) {
		if ((exp % 2) == 1)
			num_bin[i] = 1;
		else
			num_bin[i] = 0;
		exp /= 2;
		i ++;
	}
	//Se pasa el valor binario obtenido anteriormente al exponente, puesto que sale al reves.
	for (j = 0; j < 8; j++)
		exponente[j] = num_bin[7-j];
	//Se asigna un valor al exponente en casos específicos.
	if (dec >= 0.0 && dec < 2.0)
		exponente[0] = 0;

	//Se imprime el numero resultante
	printf("\tSigno: %hd", signo);
	printf("\n");

	printf("\tExponente: ");
	for (i = 0; i < 8; i++)
		printf("%hd", exponente[i]);
	printf("\n");

	printf("\tMantisa: ");
	for (i = 0; i < 23; i++)
		printf("%hd", mantisa[i]);
	printf("\n");

	return;
}

void bin_a_dec(){
	//Declaración de variables.
	float base = 128.0;			//Valor máximo de base 2 que puede tomar el exponente
	char signo;					//Variable que almacena el signo
	char exponente[50] = {'0'};	//Cadena que recibe binario desde teclado.
	char mantisa[50] = {'0'};	//Cadena que recibe binario desde teclado.
	short exp = 0;				//Variable que almacena el exponente en decimal.
	double dec = 0.0;			//Variable que almacena el valor final.
	int i;						//Contador.

	//Se solicitan datos al usuario y se hacen alguas validaciones de entrada.
	getchar();
	printf("\nPara los datos solicitados a continuación se rellenará con 0s en caso de hacer falta bits o se recortará el valor si excede la capacidad.\n\n");
	printf("Signo: ");
	scanf("%c", &signo);
	if (signo != '0' && signo != '1') {
		printf("El signo sólo puede ser 0 ó 1. Intenta de nuevo.\n");
		exit(-1);
	}
	getchar();
	printf("Exponente: ");
	scanf(" %[^\n]", exponente);
	getchar();
	printf("Mantisa: ");
	scanf(" %[^\n]", mantisa);

	//Se calcula el valor decimal del exponente mientras se validan las entradas.
	for (i = 0; i < 8; i ++) {
		if (exponente[i] == 49)
			exp += base;
		else if (exponente[i] != 0 && exponente[i] != 48){
			printf("Se ingresó un valor diferente de 0 o 1. Intenta de nuevo.\n");
			exit(-1);
		}
		base /= 2;
	}

	//Se obtiene y valida el valor del exponente.
	if ((exp -= 127) < 0) {
		printf("El valor del exponente es incorrecto. Intenta de nuevo.\n");
		exit(-1);
	}

	//Se coloca un 1 al principio de la mantisa para contar los binarios fraccionarios desde ahí.
	for (i = 22; i >= 0; i --)
		mantisa [i+1] = mantisa[i];
	mantisa[0] = '1';

	//Se calcula el corrimiento de bits de la mantisa.
	base = 1;
	for (i = 0; i < exp; i++) 
		base *= 2;

	//Se calcula el valor decimal final a partir de la mantisa y el exponente
	for (i = 0; i < 23; i ++) {
		if (mantisa[i] == 49)
			dec += base;
		else if (mantisa[i] != 0 && mantisa[i] != 48){
			printf("Se ingresó un valor diferente de 0 o 1. Intenta de nuevo.\n");
			exit(-1);
		}
		base /= 2.0;
	}

	//Se asigna el signo al valor final.
	if (signo == '1')
		dec = dec*-1;

	//Se imprime el valor final.
	printf("\tEl valor decimal es: %lf\n", dec);
	
	return;
}