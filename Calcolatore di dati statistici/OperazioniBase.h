#pragma once

#include <map>
#include <list>
using namespace std;

class OperazioniBase
{
	private:
		list<float> lista_risultati_calcoli;
	public:
		OperazioniBase();
		float calcoloPotenza(float valore_base, float valore_esponente);
		float calcoloRadice(float valore_base, float valore_indice);
		float calcoloLogaritmo(float valore_base, float valore_argomento);
		float calcoloSommatoria(float array_valori[]);

};