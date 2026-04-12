#include "OperazioniStatistiche.h"

using namespace std;

OperazioniStatistiche::OperazioniStatistiche()
{
	this->risultato_corrente = { vector<float>{}, "", 0.0f };
}

// La media aritmetica è la somma dei valori divisa per il numero totale di valori

float OperazioniStatistiche::calcoloMediaAritmetica(vector<float> valori)
{	
	float risultato_media_aritmetica = calcoloSommatoria(valori) / valori.size();
	this->risultato_corrente = { valori, "media aritmetica", risultato_media_aritmetica };
	return risultato_media_aritmetica;
}

// La media geometrica è la radice n-esima del prodotto dei valori, 
// dove n è il numero totale di valori
float OperazioniStatistiche::calcoloMediaGeometrica(vector<float> valori)
{
	float risultato_media_geometrica = pow(calcoloProduttoria(valori), 1.0f / valori.size());
	this->risultato_corrente = { valori, "media geometrica", risultato_media_geometrica };
	return risultato_media_geometrica;
}

// La media armonica è il numero totale di valori diviso per la somma 
// dei reciproci dei valori

float OperazioniStatistiche::calcoloMediaArmonica(vector<float> valori)
{
	vector<float> valori_originali = valori;
	for (int i = 0; i < valori.size(); i++)
	{
		valori[i] = 1.0f / valori[i]; // reciproco del valore
	}
	float risultato_media_armonica = valori.size() / calcoloSommatoria(valori);
	this->risultato_corrente = { valori_originali, "media armonica", risultato_media_armonica };
	return risultato_media_armonica;
}

// La media quadratica è la radice quadrata della media aritmetica 
// dei quadrati dei valori
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

// La media ponderata è la somma dei prodotti dei valori per i rispettivi pesi
// divisa per la somma dei pesi

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

// La varianza ponderata è la media ponderata dei quadrati degli scarti dei 
// valori dalla media ponderata

float OperazioniStatistiche::calcoloVarianzaPonderata(vector<float> valori, vector<float> pesi)
{
	if (valori.empty() || pesi.empty() || valori.size() != pesi.size())
	{
		this->risultato_corrente = { pair<vector<float>, vector<float>>{valori, pesi}, "varianza ponderata", 0.0f };
		return 0.0f;
	}

	float media = calcoloMediaPonderata(valori, pesi);

	float sommaPesi = 0.0f;
	float sommaScarti = 0.0f;

	for (int i = 0; i < valori.size(); i++)
	{
		float diff = valori[i] - media;
		float quadrato = diff * diff; 

		sommaScarti += quadrato * pesi[i];
		sommaPesi += pesi[i];
	}

	float varianza = (sommaPesi != 0.0f) ? (sommaScarti / sommaPesi) : 0.0f;
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori, pesi}, "varianza ponderata", varianza };

	return varianza;
}

// La deviazione standard ponderata è la radice quadrata della varianza ponderata
float OperazioniStatistiche::calcoloDeviazioneStandardPonderata(vector<float> valori, vector<float> pesi)
{
	float risultato_deviazione_standard_ponderata = calcoloRadice(calcoloVarianzaPonderata(valori, pesi), 2.0f);
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori, pesi}, "deviazione standard ponderata", risultato_deviazione_standard_ponderata };
	return risultato_deviazione_standard_ponderata;
}

// La mediana ponderata è il valore che divide la distribuzione dei valori in due parti uguali,
// tenendo conto delle frequenze associate a ciascun valore ( questo è utile quando i dati sono raggruppati in 
// classi o quando si hanno frequenze diverse per ciascun valore )
float OperazioniStatistiche::calcoloMedianaPonderata(vector<float> valori, vector<float> frequenze)
{
	if (valori.empty() || frequenze.empty() || valori.size() != frequenze.size())
	{
		this->risultato_corrente = { pair<vector<float>, vector<float>>{valori, frequenze}, "mediana ponderata", 0.0f };
		return 0.0f;
	}

	vector<pair<float, float>> valori_frequenze;
	for (int i = 0; i < valori.size(); i++)
	{
		valori_frequenze.push_back({ valori[i], frequenze[i] });
	}

	sort(valori_frequenze.begin(), valori_frequenze.end(), [](const auto& a, const auto& b) {
		return a.first < b.first;
	});

	float totale_frequenze = 0.0f;
	for (const auto& coppia : valori_frequenze)
	{
		totale_frequenze += coppia.second;
	}

	float soglia = totale_frequenze / 2.0f;
	float cumulata = 0.0f;
	float risultato_mediana_ponderata = valori_frequenze.back().first;
	for (const auto& coppia : valori_frequenze)
	{
		cumulata += coppia.second;
		if (cumulata >= soglia)
		{
			risultato_mediana_ponderata = coppia.first;
			break;
		}
	}

	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori, frequenze}, "mediana ponderata", risultato_mediana_ponderata };
	return risultato_mediana_ponderata;
}

// La deviazione standard è la radice quadrata della varianza, 
// che misura la dispersione dei dati rispetto alla media
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

// La varianza è la media dei quadrati delle deviazioni dei valori dalla media,
// che rappresenta la dispersione dei dati rispetto alla media
float OperazioniStatistiche::calcoloVarianza(vector<float> valori)
{
	float risultato_varianza = calcoloPotenza(calcoloDeviazioneStandard(valori), 2.0f);
	this->risultato_corrente = { valori, "varianza", risultato_varianza };
	return risultato_varianza;
}

// La mediana è il valore che si trova al centro di un insieme di dati ordinati,
// che divide la distribuzione dei dati in due parti uguali
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

// La moda è il valore che si presenta con maggiore frequenza in un insieme di dati,

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

// La frequenza assoluta è il numero di volte che un valore si presenta in un insieme di dati,
// che può essere rappresentato come una mappa che associa ogni valore alla sua frequenza

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

// La frequenza relativa è la frequenza assoluta di un valore divisa per il numero totale di valori,
// che rappresenta la proporzione di volte che un valore si presenta in un insieme di dati

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

// Il percentile è il valore al di sotto del quale si trova una certa percentuale di dati 
// in un insieme ordinato

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

// Lo scarto semplice medio è la media aritmetica degli scarti assoluti dei valori dalla media,
// che rappresenta la dispersione dei dati rispetto alla media in modo più semplice rispetto 
// alla deviazione standard

float OperazioniStatistiche::calcoloScartoSempliceMedio(vector<float> valori)
{
	float risultato_media_aritmetica = calcoloMediaAritmetica(valori);
	vector<float> valori_scarti_assoluti;
	for (int i = 0; i < (int)valori.size(); i++)
	{
		// Calcolo il valore assoluto
		valori_scarti_assoluti.push_back(calcoloValoreAssoluto(valori[i] - risultato_media_aritmetica));
	}
	float risultato_scarto_semplice_medio = calcoloSommatoria(valori_scarti_assoluti) / (float)valori.size();
	this->risultato_corrente = { valori, "scarto semplice medio", risultato_scarto_semplice_medio };
	return risultato_scarto_semplice_medio;
}

// E' uguale alla deviazione standard  

float OperazioniStatistiche::calcoloScartoQuadraticoMedio(vector<float> valori)
{
	float risultato_scarto_quadratico_medio = calcoloDeviazioneStandard(valori);
	this->risultato_corrente = { valori, "scarto quadratico medio", risultato_scarto_quadratico_medio };
	return risultato_scarto_quadratico_medio;
}

// La funzione gaussiana, o distribuzione normale, è una funzione di densità di probabilità 
// che descrive la distribuzione dei dati intorno alla media, con una forma a campana simmetrica.

float OperazioniStatistiche::calcoloGaussiana(float valore_media, float valore_varianza, float valore_x) 
{
	float n = 1 / calcoloRadice(2 * numbers::pi_v<long double>*calcoloPotenza(valore_varianza, 2), 2);
	float risultato_gaussiana = n * calcoloPotenza(1 / numbers::e_v<long double>, calcoloPotenza(valore_x - valore_media, 2) / (2 * calcoloPotenza(valore_varianza, 2)));
	this->risultato_corrente = { vector<float>{valore_media, valore_varianza, valore_x}, "gaussiana", risultato_gaussiana };
	return risultato_gaussiana;
}

// La covarianza è una misura della relazione lineare tra due variabili, 
// che indica se le variabili tendono a variare insieme (covarianza positiva)
// o in modo opposto (covarianza negativa)
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

float OperazioniStatistiche::calcoloCoefficienteAngolareRettaRegressioneYsuX(vector<float> valori_x, vector<float> valori_y)
{
	float varianza_x = this->calcoloVarianza(valori_x);
	float coefficiente_angolare = 0.0f;
	if (varianza_x != 0.0f)
	{
		coefficiente_angolare = this->calcoloCovarianza(valori_x, valori_y) / varianza_x;
	}
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori_x, valori_y}, "coefficiente angolare retta regressione Y su X", coefficiente_angolare };
	return coefficiente_angolare;
}

float OperazioniStatistiche::calcoloCoefficienteAngolareRettaRegressioneXsuY(vector<float> valori_x, vector<float> valori_y)
{
	float varianza_y = this->calcoloVarianza(valori_y);
	float coefficiente_angolare = 0.0f;
	if (varianza_y != 0.0f)
	{
		coefficiente_angolare = this->calcoloCovarianza(valori_x, valori_y) / varianza_y;
	}
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori_x, valori_y}, "coefficiente angolare retta regressione X su Y", coefficiente_angolare };
	return coefficiente_angolare;
}

float OperazioniStatistiche::calcoloIntercettaRettaRegressioneYsuX(vector<float> valori_x, vector<float> valori_y)
{
	float media_x = this->calcoloMediaAritmetica(valori_x);
	float media_y = this->calcoloMediaAritmetica(valori_y);
	float coefficiente_angolare = this->calcoloCoefficienteAngolareRettaRegressioneYsuX(valori_x, valori_y);
	float intercetta = media_y - coefficiente_angolare * media_x;
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori_x, valori_y}, "intercetta retta regressione Y su X", intercetta };
	return intercetta;
}

float OperazioniStatistiche::calcoloIntercettaRettaRegressioneXsuY(vector<float> valori_x, vector<float> valori_y)
{
	float media_x = this->calcoloMediaAritmetica(valori_x);
	float media_y = this->calcoloMediaAritmetica(valori_y);
	float coefficiente_angolare = this->calcoloCoefficienteAngolareRettaRegressioneXsuY(valori_x, valori_y);
	float intercetta = media_x - coefficiente_angolare * media_y;
	this->risultato_corrente = { pair<vector<float>, vector<float>>{valori_x, valori_y}, "intercetta retta regressione X su Y", intercetta };
	return intercetta;
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
				totale_colonna.push_back(valori_osservati[i][j]);
			else
				totale_colonna[j] += valori_osservati[i][j];
		}
		totale_riga.push_back(totale_riga_i);
	}

	float totale_valori_osservati = calcoloSommatoria(totale_riga);

	vector<vector<float>> tabella_valori_attesi(
		valori_osservati.size(),
		vector<float>(totale_colonna.size(), 0.0f)
	);

	for (int i = 0; i < valori_osservati.size(); i++)
	{
		for (int j = 0; j < valori_osservati[i].size(); j++)
		{
			tabella_valori_attesi[i][j] =
				totale_riga[i] * totale_colonna[j] / totale_valori_osservati;
		}
	}

	this->risultato_corrente = { vector<vector<float>>{valori_osservati}, "tabella valori attesi", tabella_valori_attesi };
	return tabella_valori_attesi;
}

vector<vector<float>> OperazioniStatistiche::calcoloTabellaProbabilita(vector<vector<float>> tabella)
{
	float totale = 0.0f;
	for (int i = 0; i < tabella.size(); i++)
		for (int j = 0; j < tabella[i].size(); j++)
			totale += tabella[i][j];

	vector<vector<float>> tabella_probabilita(
		tabella.size(),
		vector<float>(tabella[0].size(), 0.0f)
	);

	if (totale == 0.0f)
		return tabella_probabilita;

	for (int i = 0; i < tabella.size(); i++)
		for (int j = 0; j < tabella[i].size(); j++)
			tabella_probabilita[i][j] = tabella[i][j] / totale;

	this->risultato_corrente = { vector<vector<float>>{tabella}, "tabella probabilità", tabella_probabilita };
	return tabella_probabilita;
}

vector<vector<float>> OperazioniStatistiche::calcoloDistribuzioneMarginale(vector<vector<float>> tabella)
{
	vector<float> marginale_riga(tabella.size(), 0.0f);
	vector<float> marginale_colonna(tabella[0].size(), 0.0f);
	for (int i = 0; i < tabella.size(); i++)
		for (int j = 0; j < tabella[i].size(); j++)
		{
			marginale_riga[i] += tabella[i][j];
			marginale_colonna[j] += tabella[i][j];
		}
	vector<vector<float>> tabella_distribuzione_marginale = { marginale_riga, marginale_colonna };
	this->risultato_corrente = { vector<vector<float>>{tabella}, "distribuzione marginale", tabella_distribuzione_marginale };
	return tabella_distribuzione_marginale;
}

vector<vector<float>> OperazioniStatistiche::calcoloProbabilitaMarginale(vector<vector<float>> tabella)
{
	float totale = 0.0f;
	for (int i = 0; i < tabella.size(); i++)
		for (int j = 0; j < tabella[i].size(); j++)
			totale += tabella[i][j];

	if (totale == 0.0f)
		return {};

	vector<vector<float>> distribuzioneMarginale = calcoloDistribuzioneMarginale(tabella);

	vector<vector<float>> probabilitaMarginale;
	for (int i = 0; i < distribuzioneMarginale.size(); i++)
	{
		vector<float> riga;
		for (int j = 0; j < distribuzioneMarginale[i].size(); j++)
			riga.push_back(distribuzioneMarginale[i][j] / totale);
		probabilitaMarginale.push_back(riga);
	}

	this->risultato_corrente = { vector<vector<float>>{tabella}, "probabilità marginale", probabilitaMarginale };
	return probabilitaMarginale;
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
