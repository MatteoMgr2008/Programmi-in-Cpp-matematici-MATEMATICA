#pragma once

#include "OperazioniBase.h"
#include <vector>

using namespace std;

class OperazioniStatistiche
{
	private:
		RisultatoOperazione risultato_corrente;
		list<RisultatoOperazione> storico_risultati_calcoli_statistici;
	public:
		OperazioniStatistiche();
		float calcoloMediaAritmetica(vector<float> valori);
		float calcoloMediaGeometrica(vector<float> valori);
		float calcoloMediaArmonica(vector<float> valori);
		float calcoloMediaQuadratica(vector<float> valori);
		float calcoloMediaPonderata(vector<float> valori, vector<float> pesi);
		float calcoloDeviazioneStandard(vector<float> valori);
		float calcoloVarianza(vector<float> valori);
		float calcoloMediana(vector<float> valori);
		float calcoloModa(vector<float> valori);
		float calcoloFrequenzaRelativa(vector<float> valori, float valore);
		float calcoloFrequenzaAssoluta(vector<float> valori, float valore);
		float calcoloPercentile(vector<float> valori, float percentile);
		float calcoloScartoQuadraticoMedio(vector<float> valori);
		float calcoloScartoSempliceMedio(vector<float> valori);
		void aggiungiCalcoloStoricoOperazioniStatistiche();
};