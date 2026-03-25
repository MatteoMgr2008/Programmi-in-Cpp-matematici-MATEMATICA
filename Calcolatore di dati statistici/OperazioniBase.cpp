#include "OperazioniBase.h"

OperazioniBase::OperazioniBase()
{
	this->lista_risultati_calcoli = {};	
}

float OperazioniBase::calcoloLogaritmo(float valore_base, float valore_argomento)
{
	this->lista_risultati_calcoli.push_back(log(valore_argomento) / log(valore_base));
    return this->lista_risultati_calcoli.back();
}

float OperazioniBase::calcoloPotenza(float valore_base, float valore_esponente)
{
	for (int i = 0; i < valore_esponente; i++)
		this->lista_risultati_calcoli.push_back(this->lista_risultati_calcoli.back() * valore_base);
	return this->lista_risultati_calcoli.back();
}

float OperazioniBase::calcoloRadice(float valore_base, float valore_indice)
{
	//Calcolo radice = DA FINIRE DI SCRIVERE!!!
}