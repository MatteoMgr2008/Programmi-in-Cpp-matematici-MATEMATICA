#include "OperazioniStatistiche.h"

using namespace std;

OperazioniStatistiche::OperazioniStatistiche()
{
	this->risultato_corrente = { {}, "", 0.0f };
}

float OperazioniStatistiche::calcoloMediaAritmetica(vector<float> valori)
{	
	float risultato_media_aritmetica = calcoloSommatoria(valori) / valori.size();
	this->risultato_corrente = { valori, "media aritmetica", risultato_media_aritmetica };
	return risultato_media_aritmetica;
}

float OperazioniStatistiche::calcoloMediaGeometrica(vector<float> valori)
{
	float risultato_media_geometrica = pow(calcoloProduttoria(valori), 1.0f / valori.size());
	this->risultato_corrente = { valori, "media geometrica", risultato_media_geometrica };
	return risultato_media_geometrica;
}

float OperazioniStatistiche::calcoloMediaArmonica(vector<float> valori)
{
	vector<float> valori_originali = valori;
	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] = 1.0f / valori[i];
	}
	float risultato_media_armonica = valori.size() / calcoloSommatoria(valori);
	this->risultato_corrente = { valori_originali, "media armonica", risultato_media_armonica };
}

float OperazioniStatistiche::calcoloMediaQuadratica(vector<float> valori)
{
	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] = calcoloPotenza(valori[i], 2.0f);
	}
	float risultato_media_quadratica = sqrt(calcoloMediaAritmetica(valori));
	this->risultato_corrente = { valori, "media quadratica", risultato_media_quadratica };
	return risultato_media_quadratica;
}

float OperazioniStatistiche::calcoloMediaPonderata(vector<float> valori, vector<float> pesi)
{
	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] *= pesi[i];
	}
	float risultato_media_ponderata = calcoloSommatoria(valori) / calcoloSommatoria(pesi);
	this->risultato_corrente = { valori, "media ponderata", risultato_media_ponderata };
	return risultato_media_ponderata;
}

float OperazioniStatistiche::calcoloDeviazioneStandard(vector<float> valori)
{
	float media = calcoloMediaAritmetica(valori);

	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] = calcoloPotenza(valori[i] - media, 2.0f);
	}

	float risultato_deviazione_standard = sqrt(calcoloMediaAritmetica(valori));
	this->risultato_corrente = { valori, "deviazione standard", risultato_deviazione_standard };
	return risultato_deviazione_standard;
}

float OperazioniStatistiche::calcoloVarianza(vector<float> valori)
{
	float risultato_varianza = calcoloPotenza(calcoloDeviazioneStandard(valori), 2.0f);
	this->risultato_corrente = { valori, "varianza", risultato_varianza };
	return risultato_varianza;
}

float OperazioniStatistiche::calcoloMediana(vector<float> valori)
{
	// ordinamento dei valori per il calcolo della mediana
	for (int i = 0; i < (int)valori.size(); i++)
	{
		for (int j = 0; j < (int)valori.size() - 1; j++)
		{
			if (valori[j] > valori[j + 1])
			{
				swap(valori[j], valori[j + 1]); // ordinamento dei valori con l'algoritmo bubble sort (scambio di posizione tra i valori)
			}
		}
	}

	float risultato_mediana = 0.0f;
	if (valori.size() % 2 == 0)
	{
		risultato_mediana = (valori[valori.size() / 2 - 1] + valori[valori.size() / 2]) / 2.0f;
	}
	else
	{
		risultato_mediana = valori[valori.size() / 2];
	}

	this->risultato_corrente = { valori, "mediana", risultato_mediana };
	return risultato_mediana;
}

float OperazioniStatistiche::calcoloModa(vector<float> valori)
{
	map<float, int> frequenza_assoluta = calcoloFrequenzaAssoluta(valori);
	float risultato_moda = 0.0f;
	int max_frequenza = 0;
	for (auto& coppia : frequenza_assoluta)
	{
		if (coppia.second > max_frequenza)
		{
			max_frequenza = coppia.second;
			risultato_moda = coppia.first;
		}
	}
	this->risultato_corrente = { valori, "moda", risultato_moda };
	return risultato_moda;
}

map<float,int> OperazioniStatistiche::calcoloFrequenzaRelativa(map<float,int> frequenza_assoluta)
{	
	map<float, int> frequenza_relativa;

	for (auto& coppia : frequenza_assoluta)	
	{
		frequenza_relativa[coppia.first] = (float)coppia.second / frequenza_assoluta.size();
	}
	return frequenza_relativa;
}

map<float, int> OperazioniStatistiche::calcoloFrequenzaAssoluta(vector<float> valori)
{
	map<float, int> frequenza_assoluta;

	for (int i=0; i<valori.size(); i++)
	{
		frequenza_assoluta[valori[i]]++;
	}
	return frequenza_assoluta;
}

float OperazioniStatistiche::calcoloPercentile(map<float,int> frequenza_relativa, float percentile)
{
	float frequenza_cumulata = 0.0f;
	for (auto& coppia : frequenza_relativa)
	{
		frequenza_cumulata += coppia.second;
		if (frequenza_cumulata >= percentile)
		{
			return coppia.first;
		}
	}
	return 0.0f;
}

float OperazioniStatistiche::calcoloScartoQuadraticoMedio(vector<float> valori)
{
	return calcoloDeviazioneStandard(valori);
}

float OperazioniStatistiche::calcoloScartoSempliceMedio(vector<float> valori)
{
	return 0.0f;
}

void OperazioniStatistiche::aggiungiCalcoloStoricoOperazioniStatistiche()
{
	this->storico_risultati_calcoli_statistici.push_back(this->risultato_corrente);
}