/* Controlador de motor de passo */

#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>

#define PARADO 0
#define MEIOPASSO 1
#define UMPASSO 2

int  estado,
	 porta,
	 i,
	 tempo_entre_comandos;
char sequencia[8];
float graus; // quantos graus o rotor gira em um passo

int Inicializar()
{
	 porta = 888;
	 estado = PARADO;
	 sequencia[0] = 8;
	 sequencia[1] = 10;
	 sequencia[2] = 2;
	 sequencia[3] = 6;
	 sequencia[4] = 4;
	 sequencia[5] = 5;
	 sequencia[6] = 1;
	 sequencia[7] = 9;
	 i = 0;
	 outp(porta,sequencia[i]);
	 tempo_entre_comandos = 300;
	 graus = 7.5;
	 return 0;
}

int MudarEstado()
{
	if (estado == 0)
	{
		do
		{
			printf("De meio em meio passo (1) ou um em um passo (2)?");
			scanf("%d",&estado);
		} while (abs(estado) != 1 && abs(estado) != 2);
	}
	else
	{
		switch (estado)
		{
			case 1: estado = 2; printf("\nUm passo"); break;
			case -1: estado = -2; printf("\nUm passo"); break;
			case 2: estado = 1; printf("\nMeio passo");break;
			case -2: estado = -1; printf("\nMeio passo"); break;
		}
	}
	return 0;
}


int CalcularProximoPasso()
{
	i += estado;
	switch (i)
    {
    	case 8 : i = 0; break;
		case 9 : i = 1; break;
		case -1 : i = 7; break;
		case -2 : i = 6; break;
	}
	return 0;
}

int Acelerar()
{
	int temp;
	printf("\nAcelerando");
	for (temp = (tempo_entre_comandos * 4); temp > tempo_entre_comandos; temp -= 50)
	{
		delay(temp);
		CalcularProximoPasso();
		outp(porta,sequencia[i]);
		printf(".");
	}
	return 0;
}

int Desacelerar()
{
	int temp;
	printf("\nDesacelerando");
	for (temp = tempo_entre_comandos; temp < (tempo_entre_comandos * 4); temp += 50)
	{
		delay(temp);
		CalcularProximoPasso();
		outp(porta,sequencia[i]);
		printf(".");
	}
	return 0;
}

int InverterSentido()
{
	if (estado != 0)
	{
		Desacelerar();
		estado = estado * (-1);
		Acelerar();
	}
	else
	{
		estado = estado * (-1);
	}
	return 0;
}

char Girar()
{
	int estado_antigo;
	char tecla;
	estado_antigo = estado;
	if (estado == PARADO)
		MudarEstado();

	if (estado_antigo == 0)
		Acelerar();
	printf("\nGirando");
	while ((tecla = kbhit()) == 0)
	{
		delay(tempo_entre_comandos);
		CalcularProximoPasso();
		outp(porta,sequencia[i]);
		printf(".");
	}
	return tecla;
}

int Parar()
{
	if (estado != 0)
	{
		Desacelerar();
		printf("\nParando");
		estado = PARADO;
	}
	return 0;
}

int GirarNGraus()
{
	float passos, ngraus;

	printf("\nQuantos graus deseja girar? ");
	scanf("%f",&ngraus);
	MudarEstado();

	passos = ngraus / graus;

	printf("\nGirando");
	while (passos > 0)
	{
		delay(tempo_entre_comandos);
		if (passos >= 1 && abs(estado) == 2)
		{
			CalcularProximoPasso();
			passos--;
			outp(porta,sequencia[i]);
			printf(".");
		}
		else
		{
			if (estado == 2)
				estado = estado / 2;
			CalcularProximoPasso();
			passos -= 0.5;
			outp(porta,sequencia[i]);
			printf(".");
		}
	}
	estado = PARADO;
	printf("\nPressione ENTER para continuar...");
	getch();
	return 0;
}

int GirarNPassos()
{
	float passos;

	printf("Quantos passos deseja girar? ");
	scanf("%f",&passos);
	MudarEstado();
	printf("\nGirando\n");
	while (passos > 0)
	{
		delay(tempo_entre_comandos);
		if (passos >= 1 && abs(estado) == 2)
		{
			CalcularProximoPasso();
			passos--;
			outp(porta,sequencia[i]);
			printf("\r%f",passos);
		}
		else
		{
			if (estado == 2)
				estado = estado / 2;
			CalcularProximoPasso();
			passos -= 0.5;
			outp(porta,sequencia[i]);
			printf("\r%f",passos);
		}
	}
	estado = PARADO;
	printf("\nPressione ENTER para continuar...");
	getch();
	return 0;
}

int GirarN()
{
	int opcao;

	if (estado != PARADO)
	{
		Parar();
	}

	do
	{
		printf("Deseja andar n passos (1) ou n graus (2)?");
		scanf("%d",&opcao);
	} while (opcao != 1 && opcao != 2);
	if (opcao == 1)
		GirarNPassos();
	else
		GirarNGraus();

	return 0;
}

int DiminuirPoucoVelocidade()
{
	printf("\nDiminuindo a velocidade... ");
	tempo_entre_comandos += 1;
	printf("%d milissegundos",tempo_entre_comandos);
	return 0;
}

int DiminuirMuitoVelocidade()
{
	printf("\nDiminuindo a velocidade... ");
	tempo_entre_comandos += 20;
	printf("%d milissegundos",tempo_entre_comandos);
	return 0;
}

int AumentarPoucoVelocidade()
{
	printf("\nAumentando a velocidade...");
	if (tempo_entre_comandos > 0)
		tempo_entre_comandos -= 1;
	printf("%d milissegundos",tempo_entre_comandos);
	return 0;
}

int AumentarMuitoVelocidade()
{
	printf("\nAumentando a velocidade...");
	if (tempo_entre_comandos > 20)
		tempo_entre_comandos -= 20;
	printf("%d milissegundos",tempo_entre_comandos);
	return 0;
}

int main()
{
	char comando;

	printf("Inicilizando o programa...");
	Inicializar();
	clrscr();
	do
	{
		clrscr();
		printf("1 - Girar\n");
		printf("2 - Inverter sentido\n");
		printf("3 - Aumentar pouco a velocidade\n");
		printf("4 - Aumentar muito a velocidade\n");
		printf("5 - Diminuir pouco a velocidade\n");
		printf("6 - Diminuir muito a velocidade\n");
		printf("7 - Parar\n");
		printf("8 - Girar n graus ou n passos\n");
		printf("9 - Mudar passos\n");
		printf("0 - Sair\n");
		comando = getch();
		SWITCH:
		switch (comando)
		{
			case '1': comando = Girar();
					  goto SWITCH;
			case '2': InverterSentido();
					  if (estado != 0)
					  {
						comando = '1';
						goto SWITCH;
					  }
					  break;
			case '3': AumentarPoucoVelocidade();
					  if (estado != 0)
					  {
						comando = '1';
						goto SWITCH;
					  }
					  break;
			case '4': AumentarMuitoVelocidade();
					  if (estado != 0)
					  {
						comando = '1';
						goto SWITCH;
					  }
					  break;
			case '5': DiminuirPoucoVelocidade();
					  if (estado != 0)
					  {
						comando = '1';
						goto SWITCH;
					  }
					  break;
			case '6': DiminuirMuitoVelocidade();
					  if (estado != 0)
					  {
						comando = '1';
						goto SWITCH;
					  }
					  break;
			case '7': Parar();
					  break;
			case '8': GirarN();
					  break;
			case '9': MudarEstado();
					  if (estado != 0)
					  {
						comando = '1';
						goto SWITCH;
					  }
					  break;
		}
	} while (comando != '0');
}