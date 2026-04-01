#pragma once

#include "OperazioniBase.h"
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <numbers>

using namespace std;

class OperazioniStatistiche : public OperazioniBase
{
	private:
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
		map<float,int> calcoloFrequenzaRelativa(map<float, int> risultato_frequenza_assoluta);
		map<float,int> calcoloFrequenzaAssoluta(vector<float> valori);
		float calcoloPercentile(map<float,int> risultato_frequenza_relativa, float valore_percentile);
		float calcoloScartoQuadraticoMedio(vector<float> valori);
		float calcoloScartoSempliceMedio(vector<float> valori);
		float calcoloGaussiana(float valore_media, float valore_varianza, float valore_x);
		void aggiungiCalcoloStoricoOperazioniStatistiche();
};