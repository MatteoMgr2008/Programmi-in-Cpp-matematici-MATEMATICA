#include "SchermataTestFunzioni.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "OperazioniStatistiche.h"

using namespace std;

// Converti la stringa contenente numeri separati da virgole o spazi in vector<float>
vector<float> ParseFloatList(const string& input) {
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

void SchermataTestFunzioni(bool& test_funzioni_statistiche) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Test Funzioni", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Torna alla Homepage")) {
        test_funzioni_statistiche = false;
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Area di Test delle Operazioni Base e Statistiche");

    static OperazioniStatistiche ops; // L'oggetto per effettuare i calcoli statistici

    static int operazioneSelezionata = 0;
    const char* nomiOperazioni[] = {
        "Scelta operazione...",
        "01) Potenza",
        "02) Radice",
        "03) Logaritmo",
        "04) Sommatoria",
        "05) Produttoria",
        "06) Media Aritmetica",
        "07) Media Geometrica",
        "08) Media Armonica",
        "09) Media Quadratica",
        "10) Deviazione Standard",
        "11) Varianza",
        "12) Mediana",
        "13) Moda"
    };

    ImGui::Combo("Scegli un'operazione", &operazioneSelezionata, nomiOperazioni, IM_ARRAYSIZE(nomiOperazioni));

    ImGui::Spacing();

    static char inputValori[256] = ""; // per liste (es. 1.5, 2.0, 3.1)
    static float floatParam1 = 0.0f; // per ex. base
    static float floatParam2 = 0.0f; // per ex. esponente
    static float risultatoFormattatoText = 0.0f;
    static string risultatoTesto = "";

    // Input form context depending on selected operazione
    if (operazioneSelezionata >= 1 && operazioneSelezionata <= 3) {
        // Operazioni che richiedono 2 parametri (Base + Argomento/Esponente)
        ImGui::InputFloat("Parametro 1 (es. Base)", &floatParam1);
        ImGui::InputFloat("Parametro 2 (es. Esponente/Argomento)", &floatParam2);
    } 
    else if (operazioneSelezionata >= 4) {
        // Operazioni che richiedono una lista di valori float
        ImGui::InputText("Lista (separati da virgola: 1, 2.5, 3)", inputValori, sizeof(inputValori));
    }

    ImGui::Spacing();

    if (operazioneSelezionata != 0) {
        if (ImGui::Button("Calcola Risultato")) {
            vector<float> lista_val;
            if (operazioneSelezionata >= 4) {
                lista_val = ParseFloatList(string(inputValori));
            }

            try {
                if (operazioneSelezionata == 1) risultatoTesto = to_string(ops.calcoloPotenza(floatParam1, floatParam2));
                else if (operazioneSelezionata == 2) risultatoTesto = to_string(ops.calcoloRadice(floatParam1, floatParam2));
                else if (operazioneSelezionata == 3) risultatoTesto = to_string(ops.calcoloLogaritmo(floatParam1, floatParam2));
                
                else if (lista_val.empty() && operazioneSelezionata >= 4) {
                    risultatoTesto = "Errore: lista di valori vuota o non valida.";
                }
                else if (operazioneSelezionata == 4) risultatoTesto = to_string(ops.calcoloSommatoria(lista_val));
                else if (operazioneSelezionata == 5) risultatoTesto = to_string(ops.calcoloProduttoria(lista_val));
                else if (operazioneSelezionata == 6) risultatoTesto = to_string(ops.calcoloMediaAritmetica(lista_val));
                else if (operazioneSelezionata == 7) risultatoTesto = to_string(ops.calcoloMediaGeometrica(lista_val));
                else if (operazioneSelezionata == 8) risultatoTesto = to_string(ops.calcoloMediaArmonica(lista_val));
                else if (operazioneSelezionata == 9) risultatoTesto = to_string(ops.calcoloMediaQuadratica(lista_val));
                else if (operazioneSelezionata == 10) risultatoTesto = to_string(ops.calcoloDeviazioneStandard(lista_val));
                else if (operazioneSelezionata == 11) risultatoTesto = to_string(ops.calcoloVarianza(lista_val));
                else if (operazioneSelezionata == 12) risultatoTesto = to_string(ops.calcoloMediana(lista_val));
                else if (operazioneSelezionata == 13) risultatoTesto = to_string(ops.calcoloModa(lista_val));
            } catch (...) {
                risultatoTesto = "Errore durante il calcolo.";
            }
        }
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Risultato: %s", risultatoTesto.c_str());

    ImGui::End();
}