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
		// Costruttore della classe OperazioniStatistiche
		OperazioniStatistiche();

		// Dichiarazione dei metodi per le operazioni statistiche
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
		float calcoloCovarianza(vector<float> valori_x, vector<float> valori_y);
		float calcoloCoefficienteDiCorrelazioneDiBravaisPearson(vector<float> valori_x, vector<float> valori_y);
		float calcoloDistribuzioneChiQuadrato(vector<vector<float>> valori_osservati);
		vector<vector<float>> calcoloTabellaValoriAttesi(vector<vector<float>> valori_osservati);

		// Dichiarazione dei metodi per la gestione dello storico dei calcoli
		void aggiungiCalcoloStoricoOperazioniStatistiche();
		void inizializzaStoricoOperazioniStatistiche(int indice_calcolo_storico);
		void leggiStoricoOperazioniStatistiche();
};