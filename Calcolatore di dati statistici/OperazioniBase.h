#pragma once

#include <map>
#include <list>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct RisultatoOperazione
{
	vector<float> valori_operazione;
	string tipo_operazione;
	float risultato_operazione;
};

class OperazioniBase
{
	protected:	
		RisultatoOperazione risultato_corrente;
	private:
		list<RisultatoOperazione> storico_risultati_calcoli_base;
	public:
		OperazioniBase();
		float calcoloPotenza(float valore_base, float valore_esponente);
		float calcoloRadice(float valore_base, float valore_indice);
		float calcoloLogaritmo(float valore_base, float valore_argomento);
		float calcoloSommatoria(vector<float> valori_sommatoria);
		float calcoloProduttoria(vector<float> valori_produttoria);
		void aggiungiCalcoloStoricoOperazioniBase();
};