#include "SchermataUploadDati.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <windows.h>
#include "OperazioniStatistiche.h"

using namespace std;

// struttura per i dati CSV tabellare (colonne di float con intestazioni)
struct CsvData {
    vector<string> headers;
    vector<vector<float>> columns;
};

// Apre Esplora File e restituisce il percorso scelto (path <percorso del file>) dall'utente, oppure stringa vuota se l'utente annulla l'operazione.
inline string ApriEsploraFile() {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "File CSV\0*.csv\0Tutti i file\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE)
        return ofn.lpstrFile;
    return "";
}

// Legge un CSV tabellare con separatore ';' e restituisce una struttura CsvData contenente intestazioni e colonne di float
inline CsvData LeggiFileCSVTabellare(const string& percorso_file) {
    CsvData dataset;
    ifstream file(percorso_file);
    if (!file.is_open())
        return dataset;

    string linea;
    bool is_first_line = true;

    while (getline(file, linea)) {
        stringstream ss(linea);
        string cella;
        int col_idx = 0;

        while (getline(ss, cella, ';')) {
            cella.erase(0, cella.find_first_not_of(" \t\r\n"));
            cella.erase(cella.find_last_not_of(" \t\r\n") + 1);

            if (is_first_line) {
                dataset.headers.push_back(cella.empty() ? "Colonna " + to_string(col_idx + 1) : cella);
                dataset.columns.push_back(vector<float>());
            }
            else {
                if (col_idx < (int)dataset.columns.size()) {
                    try {
                        dataset.columns[col_idx].push_back(stof(cella));
                    }
                    catch (...) {}
                }
            }
            col_idx++;
        }
        is_first_line = false;
    }
    return dataset;
}

// Disegna sfondo + bordo del grafico e restituisce la lambda
// di conversione coordinate dati -> coordinate schermo.
static void DisegnaSfondoGrafico(ImDrawList* dl, ImVec2 p, ImVec2 sz) {
    dl->AddRectFilled(p, ImVec2(p.x + sz.x, p.y + sz.y), IM_COL32(40, 40, 40, 255));
    dl->AddRect(p, ImVec2(p.x + sz.x, p.y + sz.y), IM_COL32(180, 180, 180, 255));
}

// Schermata principale
void SchermataUploadDati(bool& upload_file_dati) {

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Importazione file CSV per analisi dati statistici", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    // Pulsante per tornare alla Homepage
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounding di base dei bordi del pulsante
    if (ImGui::Button(ICON_FA_ARROW_LEFT "  Torna alla Homepage")) {
        upload_file_dati = false;
    }
    ImGui::PopStyleVar();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Inserisci il percorso assoluto del file CSV da cui acquisire i dati.");
    ImGui::Text("I valori numerici (float) devono essere separati da punti e virgola.");
    ImGui::Spacing();

    // Caricamento file
    static char percorsoFile[512] = "";
    ImGui::InputText("Percorso del file CSV", percorsoFile, sizeof(percorsoFile));
    ImGui::SameLine();
    if (ImGui::Button("Scegli il file CSV...")) {
        string p = ApriEsploraFile();
        if (!p.empty())
            strcpy_s(percorsoFile, sizeof(percorsoFile), p.c_str());
    }

    static CsvData              datasetCaricato;
    static int                  colonnaSelezionata = 0;
    static string               statoCaricamento = "";
    static OperazioniStatistiche ops;

    if (ImGui::Button("Carica ed estrai i dati dal file CSV")) {
        datasetCaricato = LeggiFileCSVTabellare(string(percorsoFile));
        colonnaSelezionata = 0;
        if (datasetCaricato.headers.empty()) {
            statoCaricamento = "Errore: impossibile leggere il file (percorso errato o file vuoto/non valido).";
        }
        else {
            statoCaricamento = "File analizzato con successo! " +
                to_string(datasetCaricato.headers.size()) + " colonne del file caricate in memoria.";
        }
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", statoCaricamento.c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (datasetCaricato.headers.empty()) {
        ImGui::End();
        return;
    }

    // Costruisce l'array di puntatori ai nomi colonne (usato dai Combo)
    vector<const char*> nomiColonne;
    for (const auto& h : datasetCaricato.headers)
        nomiColonne.push_back(h.c_str());

    // Combo selezione colonna per le statistiche
    ImGui::Text("Seleziona la caratteristica (colonna) su cui effettuare i calcoli:");
    ImGui::Combo("Colonna", &colonnaSelezionata, nomiColonne.data(), (int)nomiColonne.size());
    ImGui::Spacing();

    if (colonnaSelezionata < 0 || colonnaSelezionata >= (int)datasetCaricato.columns.size())
        colonnaSelezionata = 0;

    vector<float>& datiScelti = datasetCaricato.columns[colonnaSelezionata];
    ImGui::Text("Valori selezionati per '%s' (N=%zu).",
        datasetCaricato.headers[colonnaSelezionata].c_str(), datiScelti.size());

    // Tabella statistiche
    if (!datiScelti.empty()) {
        ImGui::Spacing();
        ImGui::Text("Elenco delle operazioni statistiche:");
        ImGui::Spacing();

        if (ImGui::BeginTable("Tabella dei risultati", 2,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {

            ImGui::TableSetupColumn("Operazione", ImGuiTableColumnFlags_WidthFixed, 200.0f);
            ImGui::TableSetupColumn("Risultato", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            auto AddRow = [](const char* op, const string& res) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", op);
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", res.c_str());
                };

            auto SafeCalc = [&](const char* opName, auto calcFunc) {
                try { AddRow(opName, calcFunc()); }
                catch (...) { AddRow(opName, "N/A (errore)"); }
                };

            SafeCalc("01) Sommatoria", [&] { return to_string(ops.calcoloSommatoria(datiScelti)); });
            SafeCalc("02) Produttoria", [&] { return to_string(ops.calcoloProduttoria(datiScelti)); });
            SafeCalc("03) Media aritmetica", [&] { return to_string(ops.calcoloMediaAritmetica(datiScelti)); });
            SafeCalc("04) Media geometrica", [&] { return to_string(ops.calcoloMediaGeometrica(datiScelti)); });
            SafeCalc("05) Media armonica", [&] { return to_string(ops.calcoloMediaArmonica(datiScelti)); });
            SafeCalc("06) Media quadratica", [&] { return to_string(ops.calcoloMediaQuadratica(datiScelti)); });
            SafeCalc("07) Deviazione standard", [&] { return to_string(ops.calcoloDeviazioneStandard(datiScelti)); });
            SafeCalc("08) Varianza", [&] { return to_string(ops.calcoloVarianza(datiScelti)); });

            SafeCalc("11) Frequenza assoluta", [&]() -> string {
                auto freq = ops.calcoloFrequenzaAssoluta(datiScelti);
                stringstream ss;
                for (auto const& vc : freq)
                    ss << "Dato " << vc.first << " -> " << vc.second << " volte\n";
                string r = ss.str();
                if (!r.empty()) r.pop_back();
                return r.empty() ? "N/A" : r;
                });

            SafeCalc("12) Frequenza relativa", [&]() -> string {
                auto fa = ops.calcoloFrequenzaAssoluta(datiScelti);
                auto fr = ops.calcoloFrequenzaRelativa(fa);
                stringstream ss;
                for (auto const& vr : fr)
                    ss << "Dato " << vr.first << " -> " << vr.second*100 << "%\n";
                string r = ss.str();
                if (!r.empty()) r.pop_back();
                return r.empty() ? "N/A" : r;
                });

            ImGui::EndTable();
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Grafici relazionali (scatter + regressione)
    static int colonnaX = 0;
    static int colonnaY = 0;
    if (colonnaX >= (int)datasetCaricato.columns.size()) colonnaX = 0;
    if (colonnaY >= (int)datasetCaricato.columns.size()) colonnaY = 0;

    ImGui::Text("Grafico relazionale (confronto tra 2 colonne)");
    ImGui::Combo("Asse X##rel", &colonnaX, nomiColonne.data(), (int)nomiColonne.size());
    ImGui::Combo("Asse Y##rel", &colonnaY, nomiColonne.data(), (int)nomiColonne.size());
    ImGui::Spacing();

	// Pulsante 1: solo punti nel grafico di dispersione
    if (ImGui::Button("Disegna il grafico di dispersione"))
        ImGui::OpenPopup("Popup del grafico di dispersione");

    ImGui::SameLine();

    // Pulsante 2: rette di regressione
    if (ImGui::Button("Mostra le rette di regressione"))
        ImGui::OpenPopup("Popup delle rette di regressione");

    // POPUP 1 — Scatter Plot (solo punti + punto delle medie)
    if (ImGui::BeginPopupModal("Popup del grafico di dispersione", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::Text("Dispersione: '%s' (X)  vs  '%s' (Y)",
            datasetCaricato.headers[colonnaX].c_str(),
            datasetCaricato.headers[colonnaY].c_str());

        vector<float>& vecX = datasetCaricato.columns[colonnaX];
        vector<float>& vecY = datasetCaricato.columns[colonnaY];

        if (vecX.size() > 0 && vecX.size() == vecY.size()) {

            float media_x = ops.calcoloMediaAritmetica(vecX);
            float media_y = ops.calcoloMediaAritmetica(vecY);
            float min_x = *min_element(vecX.begin(), vecX.end());
            float max_x = *max_element(vecX.begin(), vecX.end());
            float min_y = *min_element(vecY.begin(), vecY.end());
            float max_y = *max_element(vecY.begin(), vecY.end());

            if (min_x == max_x) { min_x -= 1.0f; max_x += 1.0f; }
            if (min_y == max_y) { min_y -= 1.0f; max_y += 1.0f; }

            ImVec2 screen_size = ImGui::GetIO().DisplaySize;
            ImVec2 graph_size(
                max(screen_size.x * 0.6f, 400.0f),
                max(screen_size.y * 0.5f, 300.0f));

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();

            auto Converti = [&](float x, float y) -> ImVec2 {
                return {
                    p.x + ((x - min_x) / (max_x - min_x)) * graph_size.x,
                    p.y + (1.0f - (y - min_y) / (max_y - min_y)) * graph_size.y
                };
                };

            DisegnaSfondoGrafico(draw_list, p, graph_size);

            // Punti dati
            for (size_t i = 0; i < vecX.size(); i++) {
                ImVec2 pos = Converti(vecX[i], vecY[i]);
                draw_list->AddCircleFilled(pos, 3.0f, IM_COL32(100, 200, 255, 220));
            }

            // Punto delle medie (cerchio giallo con alone)
            ImVec2 pm = Converti(media_x, media_y);
            draw_list->AddCircleFilled(pm, 6.0f, IM_COL32(255, 230, 120, 255));
            draw_list->AddCircle(pm, 10.0f, IM_COL32(255, 200, 50, 160), 16, 1.5f);

            ImGui::Dummy(graph_size);
            ImGui::Spacing();
            ImGui::Text("X = %s   (min: %.2f, max: %.2f)",
                datasetCaricato.headers[colonnaX].c_str(), min_x, max_x);
            ImGui::Text("Y = %s   (min: %.2f, max: %.2f)",
                datasetCaricato.headers[colonnaY].c_str(), min_y, max_y);
            ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.47f, 1.0f),
                "Cerchio giallo = punto medio (%.4f, %.4f)", media_x, media_y);
            ImGui::Text("N = %zu coppie di valori.", vecX.size());

        }
        else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1),
                "Errore: dimensioni delle colonne incompatibili o vuote.");
        }

        ImGui::Spacing();
        if (ImGui::Button("Chiudi", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    // POPUP 2 — Rette di regressione (punti sfumati + rette + statistiche)
    if (ImGui::BeginPopupModal("Popup delle rette di regressione", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::Text("Regressione: '%s' (X)  vs  '%s' (Y)",
            datasetCaricato.headers[colonnaX].c_str(),
            datasetCaricato.headers[colonnaY].c_str());

        vector<float>& vecX = datasetCaricato.columns[colonnaX];
        vector<float>& vecY = datasetCaricato.columns[colonnaY];

        if (vecX.size() > 0 && vecX.size() == vecY.size()) {

            float media_x = ops.calcoloMediaAritmetica(vecX);
            float media_y = ops.calcoloMediaAritmetica(vecY);
            float covarianza = ops.calcoloCovarianza(vecX, vecY);
            float coeff_bravais_pearson = ops.calcoloCoefficienteDiCorrelazioneDiBravaisPearson(vecX, vecY);
            float coeff_y_su_x = ops.calcoloCoefficienteAngolareRettaRegressioneYsuX(vecX, vecY);
            float coeff_x_su_y = ops.calcoloCoefficienteAngolareRettaRegressioneXsuY(vecX, vecY);
            float intercetta_y_su_x = ops.calcoloIntercettaRettaRegressioneYsuX(vecX, vecY);
            float intercetta_x_su_y = ops.calcoloIntercettaRettaRegressioneXsuY(vecX, vecY);
            float min_x = *min_element(vecX.begin(), vecX.end());
            float max_x = *max_element(vecX.begin(), vecX.end());
            float min_y = *min_element(vecY.begin(), vecY.end());
            float max_y = *max_element(vecY.begin(), vecY.end());

            if (min_x == max_x) { min_x -= 1.0f; max_x += 1.0f; }
            if (min_y == max_y) { min_y -= 1.0f; max_y += 1.0f; }

            ImVec2 screen_size = ImGui::GetIO().DisplaySize;
            ImVec2 graph_size(
                max(screen_size.x * 0.6f, 400.0f),
                max(screen_size.y * 0.5f, 300.0f));

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();

            auto Converti = [&](float x, float y) -> ImVec2 {
                return {
                    p.x + ((x - min_x) / (max_x - min_x)) * graph_size.x,
                    p.y + (1.0f - (y - min_y) / (max_y - min_y)) * graph_size.y
                };
                };

            DisegnaSfondoGrafico(draw_list, p, graph_size);

            // Punti dati (opacità ridotta per non coprire le rette)
            for (size_t i = 0; i < vecX.size(); i++) {
                ImVec2 pos = Converti(vecX[i], vecY[i]);
                draw_list->AddCircleFilled(pos, 2.5f, IM_COL32(100, 200, 255, 140));
            }

            // Punto delle medie
            ImVec2 pm = Converti(media_x, media_y);
            draw_list->AddCircleFilled(pm, 5.0f, IM_COL32(255, 230, 120, 255));

            // Retta Y su X  (rossa)
            float y0_ysx = coeff_y_su_x * min_x + intercetta_y_su_x;
            float y1_ysx = coeff_y_su_x * max_x + intercetta_y_su_x;
            draw_list->AddLine(Converti(min_x, y0_ysx), Converti(max_x, y1_ysx),
                IM_COL32(255, 80, 80, 255), 2.0f);

            // Retta X su Y  (viola)
            if (coeff_x_su_y != 0.0f) {
                float y0_xsy = (min_x - intercetta_x_su_y) / coeff_x_su_y;
                float y1_xsy = (max_x - intercetta_x_su_y) / coeff_x_su_y;
                draw_list->AddLine(Converti(min_x, y0_xsy), Converti(max_x, y1_xsy),
                    IM_COL32(140, 90, 255, 255), 2.0f);
            }
            else {
                draw_list->AddLine(Converti(media_x, min_y), Converti(media_x, max_y),
                    IM_COL32(140, 90, 255, 255), 2.0f);
            }

            ImGui::Dummy(graph_size);
            ImGui::Spacing();

            // Legenda colori
            ImGui::TextColored(ImVec4(1.0f, 0.31f, 0.31f, 1.0f), "[Rosso]");
            ImGui::SameLine(); ImGui::Text("Retta Y su X");
            ImGui::SameLine(); ImGui::Text("   ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.55f, 0.35f, 1.0f, 1.0f), "[Viola]");
            ImGui::SameLine(); ImGui::Text("Retta X su Y");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Statistiche
            ImGui::Text("Punto medio:             (%.4f,  %.4f)", media_x, media_y);
            ImGui::Text("Covarianza:              %.4f", covarianza);
            ImGui::Text("Coefficente di Bravais-Pearson:  %.4f", coeff_bravais_pearson);
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(1.0f, 0.31f, 0.31f, 1.0f), "Y su X:");
            ImGui::Text("  y - %.4f = %.4f * (x - %.4f)", media_y, coeff_y_su_x, media_x);
            ImGui::Text("  Forma esplicita:  y = %.4f x + %.4f", coeff_y_su_x, intercetta_y_su_x);
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.55f, 0.35f, 1.0f, 1.0f), "X su Y:");
            ImGui::Text("  x - %.4f = %.4f * (y - %.4f)", media_x, coeff_x_su_y, media_y);
            ImGui::Text("  Forma esplicita:  x = %.4f y + %.4f", coeff_x_su_y, intercetta_x_su_y);

        }
        else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1),
                "Errore: dimensioni delle colonne incompatibili o vuote.");
        }

        ImGui::Spacing();
        if (ImGui::Button("Chiudi", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // istogramma avanzato (due colonne)
    if (!datiScelti.empty()) {

        static int modalitaGrafici = 1;  // 0 = valori singoli, 1 = intervalli
        ImGui::Text("Impostazioni visualizzazione per '%s':",
            datasetCaricato.headers[colonnaSelezionata].c_str());
        ImGui::RadioButton("Raggruppa in intervalli", &modalitaGrafici, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Singoli valori (frequenza)", &modalitaGrafici, 0);

        static int numCustomBins = 4;
        if (modalitaGrafici == 1)
            ImGui::SliderInt("Numero di intervalli", &numCustomBins, 2, 20);

        vector<float> datiOrdinati = datiScelti;
        sort(datiOrdinati.begin(), datiOrdinati.end());
        float min_val = datiOrdinati.front();
        float max_val = datiOrdinati.back();
        if (min_val == max_val) { min_val -= 1.0f; max_val += 1.0f; }

        ImGui::Spacing();

        static int featureX_Histo = 0;
        static int featureY_Histo = 0;
        if (featureX_Histo >= (int)datasetCaricato.columns.size()) featureX_Histo = 0;
        if (featureY_Histo >= (int)datasetCaricato.columns.size()) featureY_Histo = 0;

        ImGui::Text("Istogramma a due colonne:");
        ImGui::Combo("Asse X (Categoria)##histo", &featureX_Histo, nomiColonne.data(), (int)nomiColonne.size());
        ImGui::Combo("Asse Y (Valore Bar)##histo", &featureY_Histo, nomiColonne.data(), (int)nomiColonne.size());

        if (ImGui::Button("Mostra il grafico istogramma avanzato"))
            ImGui::OpenPopup("Popup istogramma avanzato");

        // Popup istogramma
        if (ImGui::BeginPopupModal("Popup istogramma avanzato", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::Text("Istogramma: X='%s'  Y='%s' (media per gruppo)",
                datasetCaricato.headers[featureX_Histo].c_str(),
                datasetCaricato.headers[featureY_Histo].c_str());

            vector<float>& colX = datasetCaricato.columns[featureX_Histo];
            vector<float>& colY = datasetCaricato.columns[featureY_Histo];

            if (colX.size() > 0 && colX.size() == colY.size()) {

                vector<float>  bar_values;
                vector<string> bar_labels;
                float max_y_bar = 0.0001f;

                if (modalitaGrafici == 1) {
                    float mn_x = *min_element(colX.begin(), colX.end());
                    float mx_x = *max_element(colX.begin(), colX.end());
                    float rng = (mx_x - mn_x > 0.0f) ? (mx_x - mn_x) : 1.0f;

                    vector<float> sum_y(numCustomBins, 0.0f);
                    vector<int>   cnt_y(numCustomBins, 0);

                    for (size_t k = 0; k < colX.size(); k++) {
                        int bi = (int)(((colX[k] - mn_x) / rng) * numCustomBins);
                        bi = max(0, min(numCustomBins - 1, bi));
                        sum_y[bi] += colY[k];
                        cnt_y[bi]++;
                    }

                    for (int k = 0; k < numCustomBins; k++) {
                        float media = cnt_y[k] > 0 ? sum_y[k] / cnt_y[k] : 0.0f;
                        bar_values.push_back(media);

                        char buf[64];
                        snprintf(buf, sizeof(buf), "%.1f-%.1f",
                            mn_x + k * rng / numCustomBins,
                            mn_x + (k + 1) * rng / numCustomBins);
                        bar_labels.push_back(buf);

                        if (media > max_y_bar) max_y_bar = media;
                    }
                }
                else {
                    map<float, vector<float>> grouped;
                    for (size_t k = 0; k < colX.size(); k++)
                        grouped[colX[k]].push_back(colY[k]);

                    for (auto const& pair : grouped) {
                        float sum = 0;
                        for (float v : pair.second) sum += v;
                        float avg = sum / pair.second.size();
                        bar_values.push_back(avg);

                        char buf[32];
                        snprintf(buf, sizeof(buf), "%.1f", pair.first);
                        bar_labels.push_back(buf);

                        if (avg > max_y_bar) max_y_bar = avg;
                    }
                }

                max_y_bar *= 1.15f;

                ImVec2 screen_size = ImGui::GetIO().DisplaySize;
                ImVec2 graph_size(
                    max(screen_size.x * 0.7f, 600.0f),
                    max(screen_size.y * 0.55f, 300.0f));

                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImVec2 p = ImGui::GetCursorScreenPos();

                draw_list->AddRectFilled(p, ImVec2(p.x + graph_size.x, p.y + graph_size.y),
                    IM_COL32(30, 30, 30, 255));

                const float margin_bottom = 40.0f;
                const float margin_left = 60.0f;
                ImVec2 origin(p.x + margin_left, p.y + graph_size.y - margin_bottom);

                draw_list->AddLine(ImVec2(origin.x, p.y), origin,
                    IM_COL32(200, 200, 200, 255), 2.0f);
                draw_list->AddLine(origin, ImVec2(p.x + graph_size.x, origin.y),
                    IM_COL32(200, 200, 200, 255), 2.0f);

                char maxLabelY[32];
                snprintf(maxLabelY, sizeof(maxLabelY), "%.2f", max_y_bar);
                draw_list->AddText(ImVec2(p.x + 5.0f, p.y + 5.0f),
                    IM_COL32(255, 255, 255, 255), maxLabelY);
                draw_list->AddText(ImVec2(p.x + 5.0f, origin.y - 15.0f),
                    IM_COL32(255, 255, 255, 255), "0.00");

                int n_bars = (int)bar_values.size();
                if (n_bars > 0) {
                    float dw = graph_size.x - margin_left - 20.0f;
                    float dh = graph_size.y - margin_bottom - 20.0f;
                    float bw = dw / n_bars;
                    float bpad = bw * 0.2f;

                    for (int i = 0; i < n_bars; i++) {
                        float bar_h = (bar_values[i] / max_y_bar) * dh;
                        ImVec2 pmin(origin.x + i * bw + bpad, origin.y - bar_h);
                        ImVec2 pmax(origin.x + (i + 1) * bw - bpad, origin.y - 1.0f);

                        draw_list->AddRectFilled(pmin, pmax, IM_COL32(50, 150, 255, 255), 2.0f);
                        draw_list->AddRect(pmin, pmax, IM_COL32(200, 200, 200, 100), 2.0f);

                        if (bar_h > 15.0f) {
                            char valBuf[32];
                            snprintf(valBuf, sizeof(valBuf), "%.1f", bar_values[i]);
                            float tw = ImGui::CalcTextSize(valBuf).x;
                            float center_x = pmin.x + (pmax.x - pmin.x) * 0.5f;
                            draw_list->AddText(ImVec2(center_x - tw * 0.5f, pmin.y - 15.0f),
                                IM_COL32(255, 255, 100, 255), valBuf);
                        }

                        if (n_bars <= 15 || (i % (n_bars / 10 + 1) == 0)) {
                            float tw = ImGui::CalcTextSize(bar_labels[i].c_str()).x;
                            float center_x = pmin.x + (pmax.x - pmin.x) * 0.5f;
                            draw_list->AddText(ImVec2(center_x - tw * 0.5f, origin.y + 5.0f),
                                IM_COL32(200, 200, 200, 255), bar_labels[i].c_str());
                        }
                    }
                }

                ImGui::Dummy(graph_size);

            }
            else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1),
                    "Errore: dimensioni colonne incompatibili o vuote.");
            }

            ImGui::Spacing();
            if (ImGui::Button("Chiudi il popup grafico", ImVec2(120, 0)))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Sezione: grafico a torta
        static float radiusPieChart = 100.0f;

        ImGui::Text("Grafico a Torta — Feature: %s",
            datasetCaricato.headers[colonnaSelezionata].c_str());
        ImGui::SliderFloat("Zoom (Raggio)", &radiusPieChart, 50.0f, 400.0f);
        ImGui::Spacing();

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImVec2 pie_center = ImVec2(p.x + radiusPieChart + 10.0f, p.y + radiusPieChart);
        const float TWO_PI = 6.28318530717958647692f;

        float a_min = 0.0f;
        float a_max = 0.0f;

        if (modalitaGrafici == 1) {

            int   num_bins = numCustomBins;
            float range = max_val - min_val;
            if (range <= 0.0f) range = 1.0f;

            vector<int> bins(num_bins, 0);
            for (float v : datiScelti) {
                int bi = (int)(((v - min_val) / range) * num_bins);
                bi = max(0, min(num_bins - 1, bi));
                bins[bi]++;
            }

            for (int i = 0; i < num_bins; i++) {
                if (bins[i] == 0) continue;
                a_max = a_min + ((float)bins[i] / datiScelti.size()) * TWO_PI;

                float r, g, b;
                ImGui::ColorConvertHSVtoRGB((float)i / num_bins, 0.8f, 0.8f, r, g, b);
                ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                draw_list->PathLineTo(pie_center);
                draw_list->PathArcTo(pie_center, radiusPieChart, a_min, a_max, 20);
                draw_list->PathFillConvex(color);
                a_min = a_max;
            }

            ImGui::Dummy(ImVec2(radiusPieChart * 2.0f + 20.0f, radiusPieChart * 2.0f));
            ImGui::SameLine();

            float offset_y = p.y + (radiusPieChart * 2.0f -
                num_bins * ImGui::GetTextLineHeightWithSpacing()) * 0.5f;
            if (offset_y < p.y) offset_y = p.y;
            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, offset_y));

            ImGui::BeginGroup();
            for (int i = 0; i < num_bins; i++) {
                float bin_min = min_val + i * range / num_bins;
                float bin_max = min_val + (i + 1) * range / num_bins;

                float r, g, b;
                ImGui::ColorConvertHSVtoRGB((float)i / num_bins, 0.8f, 0.8f, r, g, b);
                ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                ImVec2 pos = ImGui::GetCursorScreenPos();
                draw_list->AddRectFilled(pos, ImVec2(pos.x + 12.0f, pos.y + 12.0f), color);
                ImGui::Dummy(ImVec2(15.0f, 12.0f));
                ImGui::SameLine();
                ImGui::Text("[%.2f - %.2f]: %d (%.1f%%)",
                    bin_min, bin_max, bins[i],
                    (float)bins[i] / datiScelti.size() * 100.0f);
            }
            ImGui::EndGroup();

        }
        else {

            map<float, int> frequenze;
            for (float v : datiScelti) frequenze[v]++;

            int i = 0;
            int total_unique = (int)frequenze.size();

            for (auto const& vc : frequenze) {
                int count = vc.second;
                a_max = a_min + ((float)count / datiScelti.size()) * TWO_PI;

                float r, g, b;
                ImGui::ColorConvertHSVtoRGB((float)i / total_unique, 0.8f, 0.8f, r, g, b);
                ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                int segments = max(3, (int)(20.0f * ((float)count / datiScelti.size())));
                draw_list->PathLineTo(pie_center);
                draw_list->PathArcTo(pie_center, radiusPieChart, a_min, a_max, segments);
                draw_list->PathFillConvex(color);

                a_min = a_max;
                i++;
            }

            ImGui::Dummy(ImVec2(radiusPieChart * 2.0f + 20.0f, radiusPieChart * 2.0f));
            ImGui::SameLine();

            float panel_height = min(radiusPieChart * 2.0f, 600.0f);
            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, p.y));
            ImGui::BeginChild("PieLegend", ImVec2(0, panel_height), true);

            i = 0;
            for (auto const& vc : frequenze) {
                float r, g, b;
                ImGui::ColorConvertHSVtoRGB((float)i / total_unique, 0.8f, 0.8f, r, g, b);
                ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                ImDrawList* cdl = ImGui::GetWindowDrawList();
                ImVec2 pos = ImGui::GetCursorScreenPos();
                cdl->AddRectFilled(pos, ImVec2(pos.x + 12.0f, pos.y + 12.0f), color);
                ImGui::Dummy(ImVec2(15.0f, 12.0f));
                ImGui::SameLine();
                ImGui::Text("Valore %.2f: %d (%.1f%%)",
                    vc.first, vc.second,
                    (float)vc.second / datiScelti.size() * 100.0f);
                i++;
            }
            ImGui::EndChild();
        }

        ImGui::Spacing();

        // ------------------------------------------------------------------
        // Anteprima dati testuali
        // ------------------------------------------------------------------
        if (ImGui::TreeNode("Mostra i dati per la feature corrente in formato testuale")) {
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 200), true);
            for (size_t i = 0; i < datiScelti.size(); i++)
                ImGui::Text("Dato [%zu] = %f", i + 1, datiScelti[i]);
            ImGui::EndChild();
            ImGui::TreePop();
        }
    }

    ImGui::End();
}