#include "OperazioniBase.h"

using namespace std;

OperazioniBase::OperazioniBase()
{
	this->risultato_corrente = { {}, "", 0.0f };
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
	float risultato_potenza = 1.0f;
	for (int i = 0; i < valore_esponente; i++)		
	{
		risultato_potenza *= valore_base;
	}	
	this->risultato_corrente = { {valore_base, valore_esponente}, "potenza", risultato_potenza };
	return risultato_potenza;
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
	for (int i = 0; i < (int)valori_sommatoria.size(); i++)
	{
		risultato_sommatoria += valori_sommatoria[i];
	}
	this->risultato_corrente = { valori_sommatoria, "sommatoria", risultato_sommatoria };
	return risultato_sommatoria;
}

float OperazioniBase::calcoloProduttoria(vector<float> valori_produttoria)
{
	float risultato_produttoria = 1.0f;
	for (int i = 0; i < (int)valori_produttoria.size(); i++)
	{
		risultato_produttoria *= valori_produttoria[i];
	}
	this->risultato_corrente = { valori_produttoria, "produttoria", risultato_produttoria };
	return risultato_produttoria;
}

void OperazioniBase::aggiungiCalcoloStoricoOperazioniBase()
{
	this->storico_risultati_calcoli_base.push_back(this->risultato_corrente);
}