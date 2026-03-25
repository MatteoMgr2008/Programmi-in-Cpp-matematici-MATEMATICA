#include "OperazioniStatistiche.h"

OperazioniStatistiche::OperazioniStatistiche()
{

}

float OperazioniStatistiche::calcoloMediaAritmetica(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloMediaGeometrica(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloMediaArmonica(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloMediaQuadratica(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloMediaPonderata(vector<float> valori, vector<float> pesi)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloDeviazioneStandard(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloVarianza(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloMediana(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloModa(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloFrequenzaRelativa(vector<float> valori, float valore)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloFrequenzaAssoluta(vector<float> valori, float valore)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloPercentile(vector<float> valori, float percentile)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloScartoQuadraticoMedio(vector<float> valori)
{
	return 0.0f;
}

float OperazioniStatistiche::calcoloScartoSempliceMedio(vector<float> valori)
{
	return 0.0f;
}

void OperazioniStatistiche::aggiungiCalcoloStoricoOperazioniStatistiche()
{
	this->storico_risultati_calcoli_statistici.push_back(this->risultato_corrente);
}