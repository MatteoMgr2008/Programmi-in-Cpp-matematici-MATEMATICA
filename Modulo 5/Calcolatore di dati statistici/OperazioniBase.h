#pragma once

#include <map>
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <variant>

using namespace std;

struct RisultatoOperazione
{
	variant<
		pair<vector<float>, vector<float>>,
		pair<map<float, int>, float>,
		pair<map<float, float>, float>,
		vector<float>,
		map<float, int>,
		map<float, float>,
		vector<vector<float>>
	> valori_operazione;

	string tipo_operazione;
	variant<
		float, 
		map<float, int>,
		map<float, float>,
		vector<vector<float>>
	> risultato_operazione;
};

class OperazioniBase
{
	protected:	
		RisultatoOperazione risultato_corrente;
	private:
		list<RisultatoOperazione> storico_risultati_calcoli_base;
	public:
		// Costruttore della classe OperazioniBase
		OperazioniBase();

		// Dichiarazione dei metodi per le operazioni di base
		float calcoloPotenza(float valore_base, float valore_esponente);
		float calcoloRadice(float valore_base, float valore_indice);
		float calcoloLogaritmo(float valore_base, float valore_argomento);
		float calcoloSommatoria(vector<float> valori_sommatoria);
		float calcoloProduttoria(vector<float> valori_produttoria);
		float calcoloValoreAssoluto(float valore_non_assoluto);

		// Dichiarazione dei metodi per la gestione dello storico dei calcoli
		void aggiungiCalcoloStoricoOperazioniBase();
		void inizializzaStoricoOperazioniBase(int indice_calcolo_storico);
		void leggiStoricoOperazioniBase();
};