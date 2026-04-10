#include "OperazioniStatistiche.h"

using namespace std;

OperazioniStatistiche::OperazioniStatistiche()
{
	this->risultato_corrente = { vector<float>{}, "", 0.0f };
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
	return risultato_media_armonica;
}

float OperazioniStatistiche::calcoloMediaQuadratica(vector<float> valori)
{
	vector<float> valori_originali = valori;
	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] = calcoloPotenza(valori[i], 2.0f);
	}
	float risultato_media_quadratica = sqrt(calcoloMediaAritmetica(valori));
	this->risultato_corrente = { valori_originali, "media quadratica", risultato_media_quadratica };
	return risultato_media_quadratica;
}

float OperazioniStatistiche::calcoloMediaPonderata(vector<float> valori, vector<float> pesi)
{
	vector<float> valori_originali = valori;
	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] *= pesi[i];
	}
	float risultato_media_ponderata = calcoloSommatoria(valori) / calcoloSommatoria(pesi);
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori_originali, pesi}, "media ponderata", risultato_media_ponderata };
	return risultato_media_ponderata;
}

float OperazioniStatistiche::calcoloDeviazioneStandard(vector<float> valori)
{
	vector<float> valori_originali = valori;
	float media = calcoloMediaAritmetica(valori);
	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] = calcoloPotenza(valori[i] - media, 2.0f);
	}
	float risultato_deviazione_standard = sqrt(calcoloMediaAritmetica(valori));
	this->risultato_corrente = { valori_originali, "deviazione standard", risultato_deviazione_standard };
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

	// calcolo della mediana
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
	map<float, int> risultato_frequenza_assoluta = calcoloFrequenzaAssoluta(valori);
	float risultato_moda = 0.0f;
	int frequenza_massima = 0;
	for (auto& coppia : risultato_frequenza_assoluta)
	{
		if (coppia.second > frequenza_massima)
		{
			frequenza_massima = coppia.second;
			risultato_moda = coppia.first;
		}
	}
	this->risultato_corrente = { valori, "moda", risultato_moda };
	return risultato_moda;
}

map<float, int> OperazioniStatistiche::calcoloFrequenzaAssoluta(vector<float> valori)
{
	map<float, int> risultato_frequenza_assoluta;
	for (int i = 0; i < valori.size(); i++)
	{
		risultato_frequenza_assoluta[valori[i]]++;
	}
	this->risultato_corrente = { valori, "frequenza assoluta", risultato_frequenza_assoluta };
	return risultato_frequenza_assoluta;
}

map<float,int> OperazioniStatistiche::calcoloFrequenzaRelativa(map<float,int> risultato_frequenza_assoluta)
{	
	map<float, int> risultato_frequenza_relativa;
	for (auto& coppia : risultato_frequenza_assoluta)	
	{
		risultato_frequenza_relativa[coppia.first] = (float)coppia.second / risultato_frequenza_assoluta.size();
	}
	this->risultato_corrente = { risultato_frequenza_assoluta, "frequenza relativa", risultato_frequenza_relativa };
	return risultato_frequenza_relativa;
}

float OperazioniStatistiche::calcoloPercentile(map<float,int> risultato_frequenza_relativa, float valore_percentile)
{
	float frequenza_cumulata = 0.0f;
	for (auto& coppia : risultato_frequenza_relativa)
	{
		frequenza_cumulata += coppia.second;
		if (frequenza_cumulata >= valore_percentile)
		{
			float risultato_percentile = coppia.first;
			this->risultato_corrente = { pair<map<float,int>, float>{risultato_frequenza_relativa, valore_percentile}, "percentile", risultato_percentile };
			return risultato_percentile;
		}
	}
	float risultato_percentile = 0.0f;
	this->risultato_corrente = { pair<map<float,int>, float>{risultato_frequenza_relativa, valore_percentile}, "percentile", risultato_percentile };
	return risultato_percentile;
}

float OperazioniStatistiche::calcoloScartoSempliceMedio(vector<float> valori)
{
	float risultato_media_aritmetica = calcoloMediaAritmetica(valori);
	vector<float> valori_scarti_assoluti;
	for (int i = 0; i < (int)valori.size(); i++)
	{
		valori_scarti_assoluti.push_back(calcoloValoreAssoluto(valori[i] - risultato_media_aritmetica));
	}
	float risultato_scarto_semplice_medio = calcoloSommatoria(valori_scarti_assoluti) / (float)valori.size();
	this->risultato_corrente = { valori, "scarto semplice medio", risultato_scarto_semplice_medio };
	return risultato_scarto_semplice_medio;
}

float OperazioniStatistiche::calcoloScartoQuadraticoMedio(vector<float> valori)
{
	float risultato_scarto_quadratico_medio = calcoloDeviazioneStandard(valori);
	this->risultato_corrente = { valori, "scarto quadratico medio", risultato_scarto_quadratico_medio };
	return risultato_scarto_quadratico_medio;
}

float OperazioniStatistiche::calcoloGaussiana(float valore_media, float valore_varianza, float valore_x) 
{
	float n = 1 / calcoloRadice(2 * numbers::pi_v<long double>*calcoloPotenza(valore_varianza, 2), 2);
	float risultato_gaussiana = n * calcoloPotenza(1 / numbers::e_v<long double>, calcoloPotenza(valore_x - valore_media, 2) / (2 * calcoloPotenza(valore_varianza, 2)));
	this->risultato_corrente = { vector<float>{valore_media, valore_varianza, valore_x}, "gaussiana", risultato_gaussiana };
	return risultato_gaussiana;
}

float OperazioniStatistiche::calcoloCovarianza(vector<float> valori_x, vector<float>valori_y)
{
	float media_valori_x = this->calcoloMediaAritmetica(valori_x);
	float media_valori_y = this->calcoloMediaAritmetica(valori_y);
	float risultato_covarianza = 0.0f;
	for (int i = 0; i < valori_x.size(); i++)
	{
		risultato_covarianza += (valori_x[i] - media_valori_x) * (valori_y[i] - media_valori_y);
	}
	risultato_covarianza /= valori_x.size();
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori_x, valori_y}, "covarianza", risultato_covarianza };
	return risultato_covarianza;
}

float OperazioniStatistiche::calcoloCoefficienteDiCorrelazioneDiBravaisPearson(vector<float> valori_x, vector<float> valori_y)	
{
	float covarianza = this->calcoloCovarianza(valori_x, valori_y);
	float deviazione_standard_x = this->calcoloDeviazioneStandard(valori_x);
	float deviazione_standard_y = this->calcoloDeviazioneStandard(valori_y);
	float risultato_coefficiente_Bravais_Pearson = covarianza / (deviazione_standard_x * deviazione_standard_y);
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori_x, valori_y}, "coefficiente di correlazione di Bravais-Pearson", risultato_coefficiente_Bravais_Pearson };
	return risultato_coefficiente_Bravais_Pearson;
}

float OperazioniStatistiche::calcoloDistribuzioneChiQuadrato(vector<vector<float>> valori_osservati)	
{
	float risultato_chi_quadrato = 0.0f;
	vector<vector<float>> tabella_valori_attesi = calcoloTabellaValoriAttesi(valori_osservati);
	for (int i = 0; i < valori_osservati.size(); i++)
	{
		for (int j = 0; j < valori_osservati[i].size(); j++)
		{
			risultato_chi_quadrato += calcoloPotenza(valori_osservati[i][j] - tabella_valori_attesi[i][j], 2) / tabella_valori_attesi[i][j];
		}
	}
	this->risultato_corrente = { vector<vector<float>>{valori_osservati}, "Distribuzione chi quadrato", risultato_chi_quadrato };
	return risultato_chi_quadrato;
}

vector<vector<float>> OperazioniStatistiche::calcoloTabellaValoriAttesi(vector<vector<float>> valori_osservati)
{
	vector<float> totale_riga;
	vector<float> totale_colonna;
	for (int i = 0; i < valori_osservati.size(); i++)
	{
		float totale_riga_i = 0.0f;
		for (int j = 0; j < valori_osservati[i].size(); j++)
		{
			totale_riga_i += valori_osservati[i][j];
			if (totale_colonna.size() <= j)
			{
				totale_colonna.push_back(valori_osservati[i][j]);
			}
			else
			{
				totale_colonna[j] += valori_osservati[i][j];
			}
		}
		totale_riga.push_back(totale_riga_i);
	}
	float totale_valori_osservati = calcoloSommatoria(totale_riga);
	vector<vector<float>> tabella_valori_attesi;
	for (int i = 0; i < valori_osservati.size(); i++)
	{
		for (int j = 0; j < valori_osservati[i].size(); j++)
		{
			tabella_valori_attesi[i][j] = totale_riga[i] * totale_colonna[j] / totale_valori_osservati;
		}
	}
	this->risultato_corrente = { vector<vector<float>>{valori_osservati}, "tabella valori attesi", tabella_valori_attesi };
	return tabella_valori_attesi;
}

void OperazioniStatistiche::aggiungiCalcoloStoricoOperazioniStatistiche()
{
	this->storico_risultati_calcoli_statistici.push_back(this->risultato_corrente);
}

void OperazioniStatistiche::inizializzaStoricoOperazioniStatistiche(int indice_calcolo_storico)
{

}

void OperazioniStatistiche::leggiStoricoOperazioniStatistiche()
{

}