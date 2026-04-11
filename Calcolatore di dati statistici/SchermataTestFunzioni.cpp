#include "SchermataTestFunzioni.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "OperazioniStatistiche.h"

using namespace std;

// Funzione per convertire una stringa di numeri separati da virgola in un vettore di float
vector<float> ConvertitoreListaDaFloat(const string& input) {
    vector<float> result;
    stringstream ss(input);
    string token;
    while (getline(ss, token, ',')) {
        stringstream tokenStream(token);
        float value;
        if (tokenStream >> value) {
            result.push_back(value);
        }
    }
    return result;
}

// Funzione per convertire una stringa di righe di numeri separati da virgola (separati da punto e virgola) in una matrice di float
vector<vector<float>> ConvertitoreMatriceDaFloat(const string& input) {
    vector<vector<float>> result;
    stringstream ss(input);
    string row;
    while (getline(ss, row, ';')) {
        vector<float> rowVec = ConvertitoreListaDaFloat(row);
        if (!rowVec.empty()) {
            result.push_back(rowVec);
        }
    }
    return result;
}

// Funzione per visualizzare la schermata di test delle funzioni statistiche
void SchermataTestFunzioni(bool& test_funzioni_statistiche) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Test Funzioni", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button(ICON_FA_ARROW_LEFT "  Torna alla Homepage")) {
        test_funzioni_statistiche = false;
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("In questa sezione è possibile testare le operazioni base e statistiche esistenti nel software.");
    ImGui::Spacing();

    static OperazioniStatistiche ops;
    
	// Lista delle operazioni disponibili (operazione 0 è un placeholder per il testo iniziale)
    static int operazioneSelezionata = 0;
    const char* nomiOperazioni[] = {
        "Clicca o premi qui per selezionare un'operazione dal menù a tendina",
        "01) Potenza",
        "02) Radice",
        "03) Logaritmo",
        "04) Valore assoluto",
        "05) Sommatoria",
        "06) Produttoria",
        "07) Media aritmetica",
        "08) Media geometrica",
        "09) Media armonica",
        "10) Media quadratica",
        "11) Deviazione standard",
        "12) Varianza",
        "13) Mediana",
        "14) Moda",
        "15) Frequenza assoluta e relativa",
        "16) Percentile",
        "17) Scarto semplice medio",
        "18) Scarto quadratico medio",
        "19) Gaussiana",
        "20) Covarianza",
        "21) Correlazione di Bravais-Pearson",
        "22) Distribuzione Chi Quadrato",
        "23) Tabella valori attesi"
    };

    ImGui::Combo("Scegli un'operazione dall'elenco", &operazioneSelezionata, nomiOperazioni, IM_ARRAYSIZE(nomiOperazioni));

    ImGui::Spacing();

    // Input per operazioni con due float (operazione 1-3)
    static float base = 0.0f;
    static float esponente = 0.0f;
    static float argomento = 0.0f;
    // Input per operazione valore assoluto (operazione 4)
    static float valore = 0.0f;
    // Input per operazione gaussiana (operazione 19)
    static float media_gaussiana = 0.0f;
    static float varianza_gaussiana = 0.0f;
    static float x_gaussiana = 0.0f;
    // Input per operazione percentile (operazione 16)
    static float valore_percentile = 0.0f;
    // Input liste
    static char inputValori[256] = "";
    static char inputValoriX[256] = "";
    static char inputValoriY[256] = "";
    static char inputMatrice[512] = "";

    static string risultatoTesto = "";

	// Mostra i campi di input in base all'operazione selezionata
    if (operazioneSelezionata == 1) {
        ImGui::InputFloat("Base", &base);
        ImGui::InputFloat("Esponente", &esponente);
    }
    else if (operazioneSelezionata == 2) {
        ImGui::InputFloat("Base (radicando)", &base);
        ImGui::InputFloat("Indice della radice", &esponente);
    }
    else if (operazioneSelezionata == 3) {
        ImGui::InputFloat("Base del logaritmo", &base);
        ImGui::InputFloat("Argomento", &argomento);
    }
    else if (operazioneSelezionata == 4) {
        ImGui::InputFloat("Valore", &valore);
    }
    else if (operazioneSelezionata >= 5 && operazioneSelezionata <= 15) {
        ImGui::InputText("Lista dei valori (separati da virgola)", inputValori, sizeof(inputValori));
    }
    else if (operazioneSelezionata == 16) {
        ImGui::InputText("Lista dei valori (separati da virgola)", inputValori, sizeof(inputValori));
        ImGui::InputFloat("Valore percentile (Ad esempio: 0.25, 0.5, 0.75)", &valore_percentile);
    }
    else if (operazioneSelezionata == 17 || operazioneSelezionata == 18) {
        ImGui::InputText("Lista dei valori (separati da virgola)", inputValori, sizeof(inputValori));
    }
    else if (operazioneSelezionata == 19) {
        ImGui::InputFloat("Media (µ)", &media_gaussiana);
        ImGui::InputFloat("Varianza (o)", &varianza_gaussiana);
        ImGui::InputFloat("Valore x", &x_gaussiana);
    }
    else if (operazioneSelezionata == 20 || operazioneSelezionata == 21) {
        ImGui::InputText("Lista dei valori di X (separati da virgola)", inputValoriX, sizeof(inputValoriX));
        ImGui::InputText("Lista dei valori di Y (separati da virgola)", inputValoriY, sizeof(inputValoriY));
    }
    else if (operazioneSelezionata == 22 || operazioneSelezionata == 23) {
        ImGui::TextWrapped("Inserire il formato matrice: righe separate da ';', valori separati da ','. Ad esempio: 10,20,30;40,50,60");
        ImGui::InputText("Matrice osservati", inputMatrice, sizeof(inputMatrice));
    }

    ImGui::Spacing();

	// Pulsante per calcolare il risultato in base all'operazione selezionata dal menù a tendina
    if (operazioneSelezionata != 0) {
        if (ImGui::Button("Calcola Risultato")) {
            try {
                vector<float> lista_val = ConvertitoreListaDaFloat(string(inputValori));
                vector<float> lista_val_x = ConvertitoreListaDaFloat(string(inputValoriX));
                vector<float> lista_val_y = ConvertitoreListaDaFloat(string(inputValoriY));
                vector<vector<float>> matrice = ConvertitoreMatriceDaFloat(string(inputMatrice));

                if (operazioneSelezionata == 1) {
                    risultatoTesto = to_string(ops.calcoloPotenza(base, esponente));
                }
                else if (operazioneSelezionata == 2) {
                    risultatoTesto = to_string(ops.calcoloRadice(base, esponente));
                }
                else if (operazioneSelezionata == 3) {
                    risultatoTesto = to_string(ops.calcoloLogaritmo(base, argomento));
                }
                else if (operazioneSelezionata == 4) {
                    risultatoTesto = to_string(ops.calcoloValoreAssoluto(valore));
                }
                else if (lista_val.empty() &&
                    operazioneSelezionata >= 5 && operazioneSelezionata <= 18) {
                    risultatoTesto = "Errore: lista di valori vuota o non valida.";
                }
                else if (operazioneSelezionata == 5) {
                    risultatoTesto = to_string(ops.calcoloSommatoria(lista_val));
                }
                else if (operazioneSelezionata == 6) {
                    risultatoTesto = to_string(ops.calcoloProduttoria(lista_val));
                }
                else if (operazioneSelezionata == 7) {
                    risultatoTesto = to_string(ops.calcoloMediaAritmetica(lista_val));
                }
                else if (operazioneSelezionata == 8) {
                    risultatoTesto = to_string(ops.calcoloMediaGeometrica(lista_val));
                }
                else if (operazioneSelezionata == 9) {
                    risultatoTesto = to_string(ops.calcoloMediaArmonica(lista_val));
                }
                else if (operazioneSelezionata == 10) {
                    risultatoTesto = to_string(ops.calcoloMediaQuadratica(lista_val));
                }
                else if (operazioneSelezionata == 11) {
                    risultatoTesto = to_string(ops.calcoloDeviazioneStandard(lista_val));
                }
                else if (operazioneSelezionata == 12) {
                    risultatoTesto = to_string(ops.calcoloVarianza(lista_val));
                }
                else if (operazioneSelezionata == 13) {
                    risultatoTesto = to_string(ops.calcoloMediana(lista_val));
                }
                else if (operazioneSelezionata == 14) {
                    risultatoTesto = to_string(ops.calcoloModa(lista_val));
                }
                else if (operazioneSelezionata == 15) {
                    map<float, int> freq_ass = ops.calcoloFrequenzaAssoluta(lista_val);
                    map<float, int> freq_rel = ops.calcoloFrequenzaRelativa(freq_ass);
                    string out = "Freq.Ass.: ";
                    for (auto& c : freq_ass)
                        out += to_string(c.first) + ":" + to_string(c.second) + "  ";
                    out += " | Freq.Rel.: ";
                    for (auto& c : freq_rel)
                        out += to_string(c.first) + ":" + to_string(c.second) + "  ";
                    risultatoTesto = out;
                }
                else if (operazioneSelezionata == 16) {
                    map<float, int> freq_ass = ops.calcoloFrequenzaAssoluta(lista_val);
                    map<float, int> freq_rel = ops.calcoloFrequenzaRelativa(freq_ass);
                    risultatoTesto = to_string(ops.calcoloPercentile(freq_rel, valore_percentile));
                }
                else if (operazioneSelezionata == 17) {
                    risultatoTesto = to_string(ops.calcoloScartoSempliceMedio(lista_val));
                }
                else if (operazioneSelezionata == 18) {
                    risultatoTesto = to_string(ops.calcoloScartoQuadraticoMedio(lista_val));
                }
                else if (operazioneSelezionata == 19) {
                    risultatoTesto = to_string(ops.calcoloGaussiana(media_gaussiana, varianza_gaussiana, x_gaussiana));
                }
                else if (operazioneSelezionata == 20) {
                    if (lista_val_x.empty() || lista_val_y.empty())
                        risultatoTesto = "Errore: lista X o Y vuota.";
                    else if (lista_val_x.size() != lista_val_y.size())
                        risultatoTesto = "Errore: lista X e Y devono avere la stessa lunghezza.";
                    else
                        risultatoTesto = to_string(ops.calcoloCovarianza(lista_val_x, lista_val_y));
                }
                else if (operazioneSelezionata == 21) {
                    if (lista_val_x.empty() || lista_val_y.empty())
                        risultatoTesto = "Errore: lista X o Y vuota.";
                    else if (lista_val_x.size() != lista_val_y.size())
                        risultatoTesto = "Errore: lista X e Y devono avere la stessa lunghezza.";
                    else
                        risultatoTesto = to_string(ops.calcoloCoefficienteDiCorrelazioneDiBravaisPearson(lista_val_x, lista_val_y));
                }
                else if (operazioneSelezionata == 22) {
                    if (matrice.empty())
                        risultatoTesto = "Errore: matrice vuota o non valida.";
                    else
                        risultatoTesto = to_string(ops.calcoloDistribuzioneChiQuadrato(matrice));
                }
                else if (operazioneSelezionata == 23) {
                    if (matrice.empty()) {
                        risultatoTesto = "Errore: matrice vuota o non valida.";
                    }
                    else {
                        vector<vector<float>> tabella = ops.calcoloTabellaValoriAttesi(matrice);
                        string out = "Tabella valori attesi: ";
                        for (int i = 0; i < (int)tabella.size(); i++) {
                            out += "[";
                            for (int j = 0; j < (int)tabella[i].size(); j++) {
                                out += to_string(tabella[i][j]);
                                if (j < (int)tabella[i].size() - 1) out += ", ";
                            }
                            out += "] ";
                        }
                        risultatoTesto = out;
                    }
                }
            }
            catch (...) {
                risultatoTesto = "Errore durante il calcolo.";
            }
        }
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Risultato: %s", risultatoTesto.c_str());

    ImGui::End();
}