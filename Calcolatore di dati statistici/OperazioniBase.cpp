#include "OperazioniBase.h"

using namespace std;

OperazioniBase::OperazioniBase()
{
	this->risultato_corrente = { vector<float>{}, "", 0.0f };
	this->storico_risultati_calcoli_base = list<RisultatoOperazione>();
}

// Il logaritmo di un numero è l'esponente a cui bisogna elevare una base per ottenere quel numero
float OperazioniBase::calcoloLogaritmo(float valore_base, float valore_argomento)
{
	float risultato_logaritmo = log(valore_argomento) / log(valore_base);
	this->storico_risultati_calcoli_base.push_back({ vector<float>{valore_base, valore_argomento}, "logaritmo (log)", risultato_logaritmo });
	return risultato_logaritmo;
}

// La potenza di un numero è il risultato di elevare un numero (la base) a un certo esponente
float OperazioniBase::calcoloPotenza(float valore_base, float valore_esponente)
{
	float risultato_potenza = 1.0f;
	for (int i = 0; i < valore_esponente; i++)		
	{
		risultato_potenza *= valore_base;
	}	
	this->risultato_corrente = { vector<float>{valore_base, valore_esponente}, "potenza", risultato_potenza };
	return risultato_potenza;
}

// La radice di un numero è il valore che, elevato a un certo indice, restituisce quel numero
float OperazioniBase::calcoloRadice(float valore_base, float valore_indice)
{
	float risultato_radice = pow(valore_base, 1.0f / valore_indice);
	this->risultato_corrente = { vector<float>{valore_base, valore_indice}, "radice", risultato_radice };
	return risultato_radice;
}

// La sommatoria è la somma di una serie di valori
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

// La produttoria è il prodotto di una serie di valori
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

// Il valore assoluto di un numero è il numero senza il suo segno 
float OperazioniBase::calcoloValoreAssoluto(float valore_non_assoluto)
{
	float risultato_valore_assoluto = this->calcoloRadice(this->calcoloPotenza(valore_non_assoluto, 2.0f), 2.0f);
	this->risultato_corrente = { vector<float>{valore_non_assoluto}, "valore assoluto", risultato_valore_assoluto };
	return risultato_valore_assoluto;
}

void OperazioniBase::aggiungiCalcoloStoricoOperazioniBase()
{
	this->storico_risultati_calcoli_base.push_back(this->risultato_corrente);
}

void OperazioniBase::inizializzaStoricoOperazioniBase(int indice_calcolo_storico)
{

}

void OperazioniBase::leggiStoricoOperazioniBase()
{

}