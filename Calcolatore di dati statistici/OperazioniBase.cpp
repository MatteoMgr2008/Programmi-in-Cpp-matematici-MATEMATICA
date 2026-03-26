#include "OperazioniBase.h"

using namespace std;

OperazioniBase::OperazioniBase()
{
	this->storico_risultati_calcoli_base = list<RisultatoOperazione>();
}

float OperazioniBase::calcoloLogaritmo(float valore_base, float valore_argomento)
{
	float risultato_logaritmo = log(valore_argomento) / log(valore_base);
	this->storico_risultati_calcoli_base.push_back({ {valore_base, valore_argomento}, "logaritmo (log)", risultato_logaritmo });
	return risultato_logaritmo;
}

float OperazioniBase::calcoloPotenza(float valore_base, float valore_esponente)
{
	for (int i = 0; i < valore_esponente; i++)		
	{
		valore_base *= valore_base;
	}	
	this->risultato_corrente = { {valore_base, valore_esponente}, "potenza", valore_base };
	return valore_base;
}

float OperazioniBase::calcoloRadice(float valore_base, float valore_indice)
{
	float risultato_radice = pow(valore_base, 1.0f / valore_indice);
	this->risultato_corrente = { {valore_base, valore_indice}, "radice", risultato_radice };
	return risultato_radice;
}

float OperazioniBase::calcoloSommatoria(vector<float> valori_sommatoria)
{
	float risultato_sommatoria = 0.0f;
	for (int i = 0; i < sizeof(valori_sommatoria); i++)
	{
		risultato_sommatoria += valori_sommatoria[i];
	}
	this->risultato_corrente = { valori_sommatoria, "sommatoria", risultato_sommatoria };
	return risultato_sommatoria;
}

void OperazioniBase::aggiungiCalcoloStoricoOperazioniBase()
{
	this->storico_risultati_calcoli_base.push_back(this->risultato_corrente);
}