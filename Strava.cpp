#include <iostream>  
#include <cmath>     
#include <cstring>   
#include <locale>    
#include <iomanip>	 
#include <stdlib.h>  
#include <fstream>	 

using namespace std;

// Prototipagem de Funções 

double calculaDistancia(double lat, double lon, double novaLati, double novaLongi);

void converteDeSegundos(int segundos, int &horaFinal, int &minutoFinal, int &segundoFinal);

unsigned int converteParaSegundos(int hora, int minuto, int segundo);

double velDosPontos(double dist, int temp);

// Main

int main(){
	
	setlocale(LC_ALL, "Portuguese");  
	
	// Declarando a Struct
	
	struct dados_type{
		double lati, longi;
		double elevacao;
		double velocidade;
		unsigned short int hora, min, seg, tempo;
		
	}viagem[100];
	
	// Declarações de Variáveis
	
	double disTotal, distancia;
	double ganhoElevacao, perdaElevacao, maiorElevacao, menorElevacao;
	double velMedia, velMax;
	int horaFinal, minFinal, segFinal, tempoTotal, tempoEntrePontos, tamanho, i = 0;
	char acces;
	bool repeticao = true;
	string nome, x;
	ifstream read;
	ofstream write;
	
	// Entrada e Processamento de Dados

	cout << "Seja Bem-Vindo ao aplicativo Strava!" << endl << "Aqui você terá acesso à distância total de seu percurso, ao tempo total decorrido, suas velocidades máximas e médias e muito mais!" << endl; 
	cout << endl;
	cout << "Para começar, digite seu nome: ";
	cin >> nome;
	cout << endl;
	cout << "Muito bem, " << nome << "! Agora só precisamos que você habilite sua localização, para que o aplicativo seja capaz de executar todas as funções corretamente." << endl;
	cout << "Para isso, selecione uma das opções abaixo (Não se preocupe! O menu aparecerá novamente, caso você decida sair do programa):" << endl;
	cout << endl;
	
	while(repeticao){
		cout << "---------------------------------------------------------------------------------------------" << endl;
		cout << "|		=> Digite '1' para visualizar os dados obtidos pelo aplicativo	    	    |" << endl;
		cout << "|											    |" << endl;
		cout << "|		=> Digite '2' para visualizar as informações de sua corrida	     	    |" << endl;
		cout << "|											    |" << endl;
		cout << "|		=> Digite '3' para sair do aplicativo				  	    |" << endl;
		cout << "---------------------------------------------------------------------------------------------" << endl << endl;
		cout << "Digite a opção: ";
		cin >> acces;
		cout << endl;
	
		switch(acces){
			case '1': 			// Ler Dados do Arquivo
				cout << "As informações de entrada são:" << endl;
				read.open("myDataRead.txt", ifstream::in);
				
        		i = 0;

				read >> viagem[i].lati >> viagem[i].longi;
				
				// Exibindo Dados do Arquivo de Leitura 
				
				while( (viagem[i].lati != -00.000000) && (viagem[i].longi != -00.000000) ){
					cout << endl;
					if(i > 0)
						cout << "----------------" << endl;
					cout << endl;
					cout << "Ponto " << i+1 << ":" << endl;
					cout << endl;
					cout << "Latitude: " << viagem[i].lati << "	  Longitude: " << viagem[i].longi << endl;
					cout << endl;
					read >> viagem[i].elevacao;
					cout << "Elevação: " << viagem[i].elevacao << endl;
			    	cout << endl;
					read >> viagem[i].hora >> viagem[i].min >> viagem[i].seg;
					cout << "Hora: " << viagem[i].hora << ":" << viagem[i].min << ":" << viagem[i].seg << endl;
					
			    	i++;
			    	
					read >> viagem[i].lati >> viagem[i].longi;
			
				}
				cout << endl;
                cout << "Os dados de entrada foram lidos corretamente." << endl << endl << nome << ", por favor, selecione uma nova opção:" << endl;
				read.close();
				break;
			
			case '2':				// Ler Dados do Arquivo e fazer os cálculos
				read.open("myDataRead.txt", ifstream::in);
				write.open("myDataWrite.txt", ofstream::out);
				
				// Zerando as variáveis, para uma possível segunda leitura
				
				disTotal = 0;
				ganhoElevacao = 0; perdaElevacao = 0;
				tempoTotal = 0;
								
				cout << "Excelente! O aplicativo fará a leitura de seus dados e disponibilizará as informações para que você tenha acesso. Além disso, ele irá salvar esses dados caso você deseje usá-los posteriormente." << endl;
				
				i = 0;
				
				read >> viagem[i].lati >> viagem[i].longi;
			
				while( (viagem[i].lati != -00.000000) && (viagem[i].longi != -00.000000) ){
			
					read >> viagem[i].elevacao;
			    
					read >> viagem[i].hora >> viagem[i].min >> viagem[i].seg;
			
					viagem[i].tempo = converteParaSegundos(viagem[i].hora, viagem[i].min, viagem[i].seg);	// Lendo o tempo em horas, minutos e segundos e já convertendo para segundos
				
			    	i++;
			    	
					read >> viagem[i].lati >> viagem[i].longi;
			
				}
				
				tamanho = i - 1;
				
				for(i = 0; i < tamanho; i++){
					
								// Distância
					distancia = calculaDistancia(viagem[i].lati, viagem[i].longi, viagem[i+1].lati, viagem[i+1].longi);
					disTotal += distancia;
					
					
								// Tempo
					tempoEntrePontos = viagem[i+1].tempo - viagem[i].tempo;		// Calculando a diferença entre o tempo entre os pontos (já em segundos [linha 130])
					tempoTotal += tempoEntrePontos;			// Somando esse tempo para saber o tempo total
			      
			        
			        			// Velocidade
					viagem[i].velocidade = velDosPontos(distancia, tempoEntrePontos);	        
								
					if(i == 0){				// Caso seja o primeiro dado, ele seta a maior e a menor elevação para este valor, assim como a velocidade máxima
						
						// Setando a maior e a menor elevação para os primeiros valores lidos
						maiorElevacao = viagem[0].elevacao;
						menorElevacao = viagem[0].elevacao;
						
						// Setando a velocidade máxima para o primeiro valor lido
						velMax = viagem[0].velocidade;

					}
					else{
								// Comparando a maior e a menor elevação com o primeiro valor lido
				    	if(viagem[i].elevacao > maiorElevacao)
				    		maiorElevacao = viagem[i].elevacao;
						else if(viagem[i].elevacao < menorElevacao)
				    		menorElevacao = viagem[i].elevacao;
						
								// Comparando a velocidade máxima com o primeiro valor lido
						if(viagem[i].velocidade > velMax)
							velMax = viagem[i].velocidade;
																	
					}
								// Elevação
								// Comparando se a nova elevação é maior ou menor do que a anterior
					if(viagem[i+1].elevacao > viagem[i].elevacao)	 	// Se for maior
						ganhoElevacao += (viagem[i+1].elevacao - viagem[i].elevacao);	// A elevação é somada ao ganho
					else if(viagem[i+1].elevacao < viagem[i].elevacao)		// Se for menor
						perdaElevacao += (viagem[i].elevacao - viagem[i+1].elevacao);	// A elevação é somada à perda 
					
				}
				
				velMedia = (disTotal * 3600) / tempoTotal;		// Calculando a Velocidade Média ao final (Pegando a distância em Km e o tempo em horas [dividindo os segundos por 3600] )
			
				converteDeSegundos(tempoTotal, horaFinal, minFinal, segFinal);		// Convertendo o tempo total para horas, minutos e segundos
				
				// Saída de Dados
				
				cout << endl;
				cout << nome << ", no dia de hoje, seus resultados foram: " << endl << endl; 
				cout << "Distancia total: " << fixed << setprecision(2) << disTotal << " km" << endl;
				cout << "Tempo total decorrido: " << horaFinal << ":" << minFinal << ":" << segFinal << endl;
				cout << "Velocidade maxima: " << fixed << setprecision(2) << velMax << " km/h" << endl;
				cout << "Velocidade media: " << fixed << setprecision(2) << velMedia << " km/h" << endl;
				cout << "Ganho de elevacao: " << fixed << setprecision(2) << ganhoElevacao << " m" << endl;
				cout << "Perda de elevacao: -" << fixed << setprecision(2) << perdaElevacao << " m" << endl;
				cout << "Maior altitude: " << fixed << setprecision(2) << maiorElevacao << " m" << endl;
				cout << "Menor altitude: " << fixed << setprecision(2) << menorElevacao << " m" << endl;
				cout << endl;
				
				// Salvando Dados
				
				write << "Distancia total: " << fixed << setprecision(2) << disTotal << " km" << endl;
				write << "Tempo total decorrido: " << horaFinal << ":" << minFinal << ":" << segFinal << endl;
				write << "Velocidade maxima: " << fixed << setprecision(2) << velMax << " km/h" << endl;
				write << "Velocidade media: " << fixed << setprecision(2) << velMedia << " km/h" << endl;	
				write << "Ganho de elevacao: " << fixed << setprecision(2) << ganhoElevacao << " m" << endl;
				write << "Perda de elevacao: -" << fixed << setprecision(2) << perdaElevacao << " m" << endl;
				write << "Maior altitude: " << fixed << setprecision(2) << maiorElevacao << " m" << endl;
				write << "Menor altitude: " << fixed << setprecision(2) << menorElevacao << " m" << endl;
				
				cout << "Seus dados já estão salvos em um arquivo, caso você deseje acessá-los posteriormente. Aproveite para comparar suas corridas e seu desempenho ;)" << endl;	
				cout << endl;
				cout<<nome<<", selecione uma nova opção:"<<endl;
				read.close();
				write.close();				
				
				break;
			
			case '3':
				repeticao = false;
				break;
			
			default:
				cout << "A opção digitada é inválida. Por favor, digite novamente." << endl;
		}
	
	}
	cout << nome << ", obrigado por usar nosso aplicativo! Estamos te esperando para sua próxima corrida :D" << endl;
			
	return 0;
	
}

// Funções

double calculaDistancia(double lat, double lon, double novaLati, double novaLongi){
	double latRad, lonRad, novaLatiRad, novaLongiRad, distancia;

	latRad = lat * 3.1415926536 / 180;
	lonRad = lon * 3.1415926536 / 180;
	novaLatiRad = novaLati * 3.1415926536 / 180;
	novaLongiRad = novaLongi * 3.1415926536 / 180;

	distancia = 6371.0 * acos(sin(latRad) * sin(novaLatiRad) + cos(latRad) * cos(novaLatiRad) * cos(novaLongiRad - lonRad));

	return distancia;

}

void converteDeSegundos(int segundos, int &horaFinal, int &minutoFinal, int &segundoFinal){
	
	horaFinal = segundos / 3600;
	minutoFinal = (segundos % 3600) / 60;
	segundoFinal = (segundos % 3600) % 60;
  
}

unsigned int converteParaSegundos(int hora, int minuto, int segundo){
	int total;
	
	total = (hora * 3600) + (minuto * 60) + segundo;
	
	return total; 
  
}

double velDosPontos(double dis, int tem){
	double vel;
	
	vel = (dis * 3600) / tem;		// Pegando a distância Km e transformando o tempo de segundos para horas (Dividindo os segundos por 3600)
	
	return vel;
	
}