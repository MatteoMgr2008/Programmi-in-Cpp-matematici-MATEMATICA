#include "OperazioniStatistiche.h"

using namespace std;

OperazioniStatistiche::OperazioniStatistiche()
{
	this->risultato_corrente = { {}, "", 0.0f };
}

float OperazioniStatistiche::calcoloMediaAritmetica(vector<float> valori)
{	
	float risultato = calcoloSommatoria(valori) / valori.size();
	this->risultato_corrente = { valori, "media aritmetica", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloMediaGeometrica(vector<float> valori)
{
	float risultato = pow(calcoloProduttoria(valori), 1.0f / valori.size());
	this->risultato_corrente = { valori, "media geometrica", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloMediaArmonica(vector<float> valori)
{
	for (int i = 0; i < sizeof(valori); i++)
	{
		valori[i] = 1.0f / valori[i];
	}
	float risultato = valori.size() / calcoloSommatoria(valori);
	this->risultato_corrente = { valori, "media armonica", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloMediaQuadratica(vector<float> valori)
{
	for (int i = 0; i < sizeof(valori); i++)
	{
		valori[i] = calcoloPotenza(valori[i], 2.0f);
	}
	float risultato = sqrt(calcoloMediaAritmetica(valori)/valori.size());
	this->risultato_corrente = { valori, "media quadratica", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloMediaPonderata(vector<float> valori, vector<float> pesi)
{
	for (int i = 0; i < sizeof(valori); i++)
	{
		valori[i] *= pesi[i];
	}
	float risultato = calcoloSommatoria(valori) / calcoloSommatoria(pesi);
	this->risultato_corrente = { valori, "media ponderata", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloDeviazioneStandard(vector<float> valori)
{
	float media = calcoloMediaAritmetica(valori);

	for (int i = 0; i < sizeof(valori); i++)
	{
		valori[i] = calcoloPotenza(valori[i] - media, 2.0f);
	}

	float risultato = sqrt(calcoloMediaAritmetica(valori) / valori.size());
	this->risultato_corrente = { valori, "deviazione standard", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloVarianza(vector<float> valori)
{
	float risultato = sqrt(calcoloDeviazioneStandard(valori));
	this->risultato_corrente = { valori, "varianza", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloMediana(vector<float> valori)
{
	// ordinamento dei valori
	for (int i = 0; i < sizeof(valori); i++)
	{
		for (int j = 0; j < sizeof(valori) - 1; j++)
		{
			if (valori[j] > valori[j + 1])
			{
				float temp = valori[j];
				valori[j] = valori[j + 1];
				valori[j + 1] = temp;
			}
		}
	}

	float risultato = 0.0f;
	if (valori.size() % 2 == 0)
	{
		risultato = (valori[valori.size() / 2 - 1] + valori[valori.size() / 2]) / 2.0f;
	}
	else
	{
		risultato = valori[valori.size() / 2];
	}

	this->risultato_corrente = { valori, "mediana", risultato };
	return risultato;
}

float OperazioniStatistiche::calcoloModa(vector<float> valori)
{
	map<float, int> frequenza_assoluta = calcoloFrequenzaAssoluta(valori);
	float risultato = 0.0f;
	int max_frequenza = 0;
	for (auto& coppia : frequenza_assoluta)
	{
		if (coppia.second > max_frequenza)
		{
			max_frequenza = coppia.second;
			risultato = coppia.first;
		}
	}
	this->risultato_corrente = { valori, "moda", risultato };
	return risultato;

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
	float percentile = 0.0f;
	for (auto& coppia : frequenza_relativa)
	{
		if (coppia.second >= percentile)
		{
			percentile = coppia.first;
		}
	}
	return percentile;
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