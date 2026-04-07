#include "SchermataUploadDati.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include "OperazioniStatistiche.h"

using namespace std;

struct CsvData {
    vector<string> headers;
    vector<vector<float>> columns;
};

// Funzione di utilità per aprire Esplora File
string ApriEsploraFile() {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "File CSV\0*.csv\0Tutti i file\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        return ofn.lpstrFile;
    }
    return "";
}

// Funzione di utilità per leggere il file CSV tabellare
CsvData LeggiFileCSVTabellare(const string& percorso_file) {
    CsvData dataset;
    ifstream file(percorso_file);
    if (!file.is_open()) {
        return dataset;
    }

    string linea;
    bool is_first_line = true;

    while (getline(file, linea)) {
        stringstream ss(linea);
        string cella;
        int col_idx = 0;
        
        while (getline(ss, cella, ',')) {
            // Rimuovo spazi in eccesso ai lati
            cella.erase(0, cella.find_first_not_of(" \t\r\n"));
            cella.erase(cella.find_last_not_of(" \t\r\n") + 1);

            if (is_first_line) {
                // Leggiamo gli header
                dataset.headers.push_back(cella.empty() ? "Feature " + to_string(col_idx + 1) : cella);
                dataset.columns.push_back(vector<float>());
            } else {
                // Leggiamo i dati se ci sono colonne a sufficienza
                if (col_idx < dataset.columns.size()) {
                    try {
                        float numero = stof(cella);
                        dataset.columns[col_idx].push_back(numero);
                    } catch (...) {
                        // Dato non valido, saltiamo o mettiamo 0. Per ora lo ignoriamo.
                    }
                }
            }
            col_idx++;
        }
        is_first_line = false;
    }
    return dataset;
}

void SchermataUploadDati(bool& upload_file_dati) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Caricamento File CSV", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Torna alla Homepage")) {
        upload_file_dati = false;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Inserisci il percorso assoluto del file CSV da cui acquisire i dati.");
    ImGui::Text("I valori numerici (float) devono essere separati da virgole o punti e virgola.");
    
    ImGui::Spacing();

    static char percorsoFile[512] = "";
    ImGui::InputText("Percorso File CSV", percorsoFile, sizeof(percorsoFile));

    ImGui::SameLine();
    if (ImGui::Button("Sfoglia...")) {
        string percorso = ApriEsploraFile();
        if (!percorso.empty()) {
            strcpy_s(percorsoFile, sizeof(percorsoFile), percorso.c_str());
        }
    }

    static CsvData datasetCaricato;
    static int colonnaSelezionata = 0;
    static string statoCaricamento = "";
    static OperazioniStatistiche ops; // L'oggetto per effettuare i calcoli statistici
    static int operazioneSelezionata = 0;
    static string risultatoTesto = "";

    if (ImGui::Button("Carica ed Estrai Dati")) {
        datasetCaricato = LeggiFileCSVTabellare(string(percorsoFile));
        if (datasetCaricato.headers.empty()) {
            statoCaricamento = "Errore: Impossibile leggere il file, percorso errato o file vuoto/non valido.";
            risultatoTesto = "";
            operazioneSelezionata = 0;
            colonnaSelezionata = 0;
        } else {
            statoCaricamento = "File letto con successo! " + to_string(datasetCaricato.headers.size()) + " feature(s) caricate in memoria.";
            risultatoTesto = "";
            operazioneSelezionata = 0;
            colonnaSelezionata = 0;
        }
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", statoCaricamento.c_str());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (!datasetCaricato.headers.empty()) {
        ImGui::Text("Seleziona la caratteristica (colonna) su cui effettuare i calcoli:");
        
        // Creiamo dinamicamente l'array dei nomi delle colonne per ImGui::Combo
        vector<const char*> nomiColonne;
        for (const auto& header : datasetCaricato.headers) {
            nomiColonne.push_back(header.c_str());
        }
        
        ImGui::Combo("Feature", &colonnaSelezionata, nomiColonne.data(), nomiColonne.size());
        ImGui::Spacing();

        // Controllo di sicurezza sull'indice della colonna
        if (colonnaSelezionata < 0 || colonnaSelezionata >= datasetCaricato.columns.size()) {
            colonnaSelezionata = 0;
        }

        vector<float>& datiScelti = datasetCaricato.columns[colonnaSelezionata];

        ImGui::Text("Valori pronti all'uso per '%s' (N=%zu). Seleziona l'operazione:", datasetCaricato.headers[colonnaSelezionata].c_str(), datiScelti.size());

        const char* nomiOperazioni[] = {
            "Scelta operazione...",
            "01) Sommatoria",
            "02) Produttoria",
            "03) Media Aritmetica",
            "04) Media Geometrica",
            "05) Media Armonica",
            "06) Media Quadratica",
            "07) Deviazione Standard",
            "08) Varianza",
            "09) Mediana",
            "10) Moda",
            "11) Frequenza Assoluta",
            "12) Frequenza Relativa"
        };

        ImGui::Combo("Scegli un'operazione", &operazioneSelezionata, nomiOperazioni, IM_ARRAYSIZE(nomiOperazioni));

        if (operazioneSelezionata != 0 && !datiScelti.empty()) {
            ImGui::Spacing();
            if (ImGui::Button("Calcola Risultato sui Dati Caricati")) {
                try {
                    if (operazioneSelezionata == 1) risultatoTesto = to_string(ops.calcoloSommatoria(datiScelti));
                    else if (operazioneSelezionata == 2) risultatoTesto = to_string(ops.calcoloProduttoria(datiScelti));
                    else if (operazioneSelezionata == 3) risultatoTesto = to_string(ops.calcoloMediaAritmetica(datiScelti));
                    else if (operazioneSelezionata == 4) risultatoTesto = to_string(ops.calcoloMediaGeometrica(datiScelti));
                    else if (operazioneSelezionata == 5) risultatoTesto = to_string(ops.calcoloMediaArmonica(datiScelti));
                    else if (operazioneSelezionata == 6) risultatoTesto = to_string(ops.calcoloMediaQuadratica(datiScelti));
                    else if (operazioneSelezionata == 7) risultatoTesto = to_string(ops.calcoloDeviazioneStandard(datiScelti));
                    else if (operazioneSelezionata == 8) risultatoTesto = to_string(ops.calcoloVarianza(datiScelti));
                    else if (operazioneSelezionata == 9) risultatoTesto = to_string(ops.calcoloMediana(datiScelti));
                    else if (operazioneSelezionata == 10) risultatoTesto = to_string(ops.calcoloModa(datiScelti));
                    else if (operazioneSelezionata == 11) {
                        auto freq = ops.calcoloFrequenzaAssoluta(datiScelti);
                        stringstream ss;
                        ss << "\n";
                        for (auto const& val_count : freq) {
                            ss << "   Dato " << val_count.first << " -> " << val_count.second << " volte\n";
                        }
                        risultatoTesto = ss.str();
                    }
                    else if (operazioneSelezionata == 12) {
                        auto freqAss = ops.calcoloFrequenzaAssoluta(datiScelti);
                        auto freqRel = ops.calcoloFrequenzaRelativa(freqAss);
                        stringstream ss;
                        ss << "\n";
                        for (auto const& val_rel : freqRel) {
                            // freqRel in OperazioniStatistiche.h ritorna un map<float, int> 
                            // che probabilmente rappresenta la percentuale intera
                            ss << "   Dato " << val_rel.first << " -> " << val_rel.second << "%\n";
                        }
                        risultatoTesto = ss.str();
                    }
                } catch (...) {
                    risultatoTesto = "Errore matematico durante il calcolo.";
                }
            }
        }
        
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Risultato Elaborazione: %s", risultatoTesto.c_str());
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Grafico Relazionale (Scatter Plot)
        static int colonnaX = 0;
        static int colonnaY = 0;
        if (colonnaX >= datasetCaricato.columns.size()) colonnaX = 0;
        if (colonnaY >= datasetCaricato.columns.size()) colonnaY = 0;

        ImGui::Text("Grafico Relazionale (Confronto tra 2 Feature)");
        ImGui::Combo("Asse X (Es. Età)", &colonnaX, nomiColonne.data(), nomiColonne.size());
        ImGui::Combo("Asse Y (Es. Peso)", &colonnaY, nomiColonne.data(), nomiColonne.size());

        if (ImGui::Button("Disegna Grafico di Dispersione")) {
            ImGui::OpenPopup("ScatterPlotPopup");
        }

        if (ImGui::BeginPopupModal("ScatterPlotPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Confronto tra '%s' (X) e '%s' (Y)", datasetCaricato.headers[colonnaX].c_str(), datasetCaricato.headers[colonnaY].c_str());
            
            vector<float>& vecX = datasetCaricato.columns[colonnaX];
            vector<float>& vecY = datasetCaricato.columns[colonnaY];

            if (vecX.size() > 0 && vecX.size() == vecY.size()) {
                float min_x = *min_element(vecX.begin(), vecX.end());
                float max_x = *max_element(vecX.begin(), vecX.end());
                float min_y = *min_element(vecY.begin(), vecY.end());
                float max_y = *max_element(vecY.begin(), vecY.end());

                if (min_x == max_x) { min_x -= 1; max_x += 1; }
                if (min_y == max_y) { min_y -= 1; max_y += 1; }

                ImVec2 graph_size(400, 300);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImVec2 p = ImGui::GetCursorScreenPos();
                
                // Disegna sfondo grafico
                draw_list->AddRectFilled(p, ImVec2(p.x + graph_size.x, p.y + graph_size.y), IM_COL32(50, 50, 50, 255));
                draw_list->AddRect(p, ImVec2(p.x + graph_size.x, p.y + graph_size.y), IM_COL32(200, 200, 200, 255));

                // Disegna i punti
                for (size_t i = 0; i < vecX.size(); i++) {
                    float norm_x = (vecX[i] - min_x) / (max_x - min_x);
                    float norm_y = 1.0f - ((vecY[i] - min_y) / (max_y - min_y)); // Invertito perché Y va verso il basso
                    
                    ImVec2 pos(p.x + norm_x * graph_size.x, p.y + norm_y * graph_size.y);
                    draw_list->AddCircleFilled(pos, 3.0f, IM_COL32(100, 200, 255, 255));
                }

                ImGui::Dummy(graph_size);
                ImGui::Text("X = %s (Min: %.2f, Max: %.2f)", datasetCaricato.headers[colonnaX].c_str(), min_x, max_x);
                ImGui::Text("Y = %s (Min: %.2f, Max: %.2f)", datasetCaricato.headers[colonnaY].c_str(), min_y, max_y);
            } else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Errore: Dimensioni delle colonne incompatibili o vuote.");
            }

            if (ImGui::Button("Chiudi Grafico", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (!datiScelti.empty()) {
            static int modalitaGrafici = 1; // 0 = Valori Singoli, 1 = Intervalli (Binning)
            ImGui::Text("Impostazioni Visualizzazione per la singola feature '%s':", datasetCaricato.headers[colonnaSelezionata].c_str());
            ImGui::RadioButton("Raggruppa in intervalli", &modalitaGrafici, 1); ImGui::SameLine();
            ImGui::RadioButton("Singoli valori (frequenza)", &modalitaGrafici, 0);

            static int numCustomBins = 4;
            if (modalitaGrafici == 1) {
                ImGui::SliderInt("Numero di Intervalli", &numCustomBins, 2, 20);
            }

            // Ordiniamo i dati per la singola feature selected
            vector<float> datiOrdinati = datiScelti;
            sort(datiOrdinati.begin(), datiOrdinati.end());

            float min_val = datiOrdinati.front();
            float max_val = datiOrdinati.back();

            // Preveniamo crash di divisione per zero in caso di intervallo nullo
            if (min_val == max_val) {
                min_val -= 1.0f;
                max_val += 1.0f;
            }

            ImGui::Spacing();
            static int featureX_Histo = 0;
            static int featureY_Histo = 0;
            if (featureX_Histo >= datasetCaricato.columns.size()) featureX_Histo = 0;
            if (featureY_Histo >= datasetCaricato.columns.size()) featureY_Histo = 0;

            ImGui::Text("Istogramma a due Feature:");
            ImGui::Combo("Asse X (Categoria)", &featureX_Histo, nomiColonne.data(), nomiColonne.size());
            ImGui::Combo("Asse Y (Valore Bar)", &featureY_Histo, nomiColonne.data(), nomiColonne.size());
            
            ImGui::Spacing();

            // Raggruppamento per X e somma/media su Y (Per default calcoliamo la Media per ciascun valore unico di X o raggruppamento)
            vector<float>& colX = datasetCaricato.columns[featureX_Histo];
            vector<float>& colY = datasetCaricato.columns[featureY_Histo];

            if (colX.size() > 0 && colX.size() == colY.size()) {
                vector<float> histogram_bars;
                float min_y_bar = FLT_MAX;
                float max_y_bar = -FLT_MAX;
                size_t n_bars = 0;

                if (modalitaGrafici == 1) { // Raggruppa in intervalli
                    float mn_x = *min_element(colX.begin(), colX.end());
                    float mx_x = *max_element(colX.begin(), colX.end());
                    float rng = mx_x - mn_x;
                    if (rng <= 0.0f) rng = 1.0f;
                    
                    vector<float> sum_y(numCustomBins, 0.0f);
                    vector<int> count_y(numCustomBins, 0);

                    for (size_t k = 0; k < colX.size(); k++) {
                        int bin_idx = (int)(((colX[k] - mn_x) / rng) * numCustomBins);
                        if (bin_idx >= numCustomBins) bin_idx = numCustomBins - 1;
                        if (bin_idx < 0) bin_idx = 0;
                        sum_y[bin_idx] += colY[k];
                        count_y[bin_idx]++;
                    }
                    
                    for (int k = 0; k < numCustomBins; k++) {
                        float media = count_y[k] > 0 ? (sum_y[k] / count_y[k]) : 0.0f;
                        histogram_bars.push_back(media);
                        if (media < min_y_bar) min_y_bar = media;
                        if (media > max_y_bar) max_y_bar = media;
                    }
                    n_bars = numCustomBins;
                } else { // Singoli valori per ogni riga
                    // Raggruppa la feature Y facendo la media dei valori duplicati su X,
                    // oppure mostra un valore per riga. Optiamo per mostrare direttamente Y ordinato rispetto a X
                    struct PairXY { float x, y; };
                    vector<PairXY> coppie;
                    for (size_t k = 0; k < colX.size(); k++) {
                        coppie.push_back({colX[k], colY[k]});
                    }
                    // Ordina per Asse X
                    sort(coppie.begin(), coppie.end(), [](const PairXY& a, const PairXY& b) {
                        return a.x < b.x;
                    });
                    
                    for (size_t k = 0; k < coppie.size(); k++) {
                        float v = coppie[k].y;
                        histogram_bars.push_back(v);
                        if (v < min_y_bar) min_y_bar = v;
                        if (v > max_y_bar) max_y_bar = v;
                    }
                    n_bars = coppie.size();
                }

                if (min_y_bar > 0.0f) min_y_bar = 0.0f; // Di base gli istogrammi partono da 0
                if (max_y_bar < min_y_bar + 1.0f) max_y_bar = min_y_bar + 1.0f;

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Max Y: %.2f", max_y_bar);
                
                string label_istogramma = "Istogramma(" + datasetCaricato.headers[featureY_Histo] + " rispetto a " + datasetCaricato.headers[featureX_Histo] + ")";
                
                // Disegna Istogramma
                ImGui::PlotHistogram("##IstogrammaDoppio", histogram_bars.data(), (int)n_bars, 0, label_istogramma.c_str(), min_y_bar, max_y_bar, ImVec2(0, 150));
                
                ImGui::Text("Min Y: %.2f", min_y_bar);
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100);
                ImGui::Text("Barre/Righe: %zu", n_bars);
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            static float radiusPieChart = 100.0f;

            ImGui::Text("Grafico a Torta della singola Feature: %s", datasetCaricato.headers[colonnaSelezionata].c_str());
            ImGui::SliderFloat("Zoom (Raggio)", &radiusPieChart, 50.0f, 400.0f);
            
            ImGui::Spacing();

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImVec2 pie_center = ImVec2(p.x + radiusPieChart + 10.0f, p.y + radiusPieChart);

            float a_min = 0.0f;
            float a_max = 0.0f;

            if (modalitaGrafici == 1) {
                int num_bins = numCustomBins;
                vector<int> bins(num_bins, 0);
                float range = max_val - min_val;
                if (range <= 0.0f) range = 1.0f;
                
                for (float v : datiScelti) {
                    int bin_idx = (int)(((v - min_val) / range) * num_bins);
                    if (bin_idx >= num_bins) bin_idx = num_bins - 1;
                    if (bin_idx < 0) bin_idx = 0;
                    bins[bin_idx]++;
                }

                for (int i = 0; i < num_bins; i++) {
                    if (bins[i] == 0) continue;
                    a_max = a_min + ((float)bins[i] / datiScelti.size()) * (3.14159265358979323846f * 2.0f);
                    
                    float r, g, b;
                    ImGui::ColorConvertHSVtoRGB((float)i / num_bins, 0.8f, 0.8f, r, g, b);
                    ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                    // Disegna lo spicchio
                    draw_list->PathLineTo(pie_center);
                    draw_list->PathArcTo(pie_center, radiusPieChart, a_min, a_max, 20);
                    draw_list->PathFillConvex(color);
                    
                    a_min = a_max;
                }

                // Riserviamo lo spazio nell'interfaccia affinchè i controlli successivi non scavalchino il grafico
                ImGui::Dummy(ImVec2(radiusPieChart * 2.0f + 20.0f, radiusPieChart * 2.0f));
                
                // Legenda a lato
                ImGui::SameLine();
                float offset_y = p.y + (radiusPieChart * 2.0f - (num_bins * ImGui::GetTextLineHeightWithSpacing())) * 0.5f;
                if (offset_y < p.y) offset_y = p.y;
                ImGui::SetCursorPosY(offset_y);

                ImGui::BeginGroup();
                for (int i = 0; i < num_bins; i++) {
                    float bin_min = min_val + (i * range / num_bins);
                    float bin_max = min_val + ((i + 1) * range / num_bins);
                    
                    float r, g, b;
                    ImGui::ColorConvertHSVtoRGB((float)i / num_bins, 0.8f, 0.8f, r, g, b);
                    ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    draw_list->AddRectFilled(pos, ImVec2(pos.x + 12.0f, pos.y + 12.0f), color);
                    ImGui::Dummy(ImVec2(15.0f, 12.0f)); 
                    ImGui::SameLine();
                    ImGui::Text("[%.2f - %.2f]: %d (%.1f%%)", bin_min, bin_max, bins[i], (float)bins[i] / datiScelti.size() * 100.0f);
                }
                ImGui::EndGroup();
            } else {
                // Modalità Valori Singoli
                map<float, int> frequenze;
                for (float v : datiScelti) frequenze[v]++;

                int i = 0;
                int total_unique = (int)frequenze.size();
                for (auto const& val_count : frequenze) {
                    float val = val_count.first;
                    int count = val_count.second;

                    a_max = a_min + ((float)count / datiScelti.size()) * (3.14159265358979323846f * 2.0f);
                    
                    float r, g, b;
                    ImGui::ColorConvertHSVtoRGB((float)i / total_unique, 0.8f, 0.8f, r, g, b);
                    ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                    // Disegna lo spicchio
                    draw_list->PathLineTo(pie_center);
                    // Aumentiamo i segmenti dell'arco se lo spicchio è più grande
                    int segments = (int)(20.0f * ((float)count / datiScelti.size()));
                    if (segments < 3) segments = 3;

                    draw_list->PathArcTo(pie_center, radiusPieChart, a_min, a_max, segments);
                    draw_list->PathFillConvex(color);
                    
                    a_min = a_max;
                    i++;
                }

                // Spazio dedicato al grafico
                ImGui::Dummy(ImVec2(radiusPieChart * 2.0f + 20.0f, radiusPieChart * 2.0f));

                // Legenda a lato in un riquadro scrollabile se ci sono troppi valori
                ImGui::SameLine();
                float group_height = total_unique * ImGui::GetTextLineHeightWithSpacing();
                float panel_height = radiusPieChart * 2.0f;
                if (panel_height > 600.0f) panel_height = 600.0f;

                ImGui::SetCursorPosY(p.y);
                ImGui::BeginChild("PieLegend", ImVec2(0, panel_height), true);
                i = 0;
                for (auto const& val_count : frequenze) {
                    float val = val_count.first;
                    int count = val_count.second;

                    float r, g, b;
                    ImGui::ColorConvertHSVtoRGB((float)i / total_unique, 0.8f, 0.8f, r, g, b);
                    ImU32 color = ImGui::GetColorU32(ImVec4(r, g, b, 1.0f));

                    // Disegno rettangolino colorato direttamente tramite draw_list nel Child
                    ImDrawList* child_draw_list = ImGui::GetWindowDrawList();
                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    child_draw_list->AddRectFilled(pos, ImVec2(pos.x + 12.0f, pos.y + 12.0f), color);
                    ImGui::Dummy(ImVec2(15.0f, 12.0f)); 
                    ImGui::SameLine();
                    ImGui::Text("Valore %.2f: %d (%.1f%%)", val, count, (float)count / datiScelti.size() * 100.0f);
                    i++;
                }
                ImGui::EndChild();
            }
        }

        ImGui::Spacing();
        
        // Anteprima dei file
        if (ImGui::TreeNode("Mostra i dati per la feature corrente in formato testuale")) {
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 200), true);
            for (size_t i = 0; i < datiScelti.size(); i++) {
                ImGui::Text("Dato [%zu] = %f", i + 1, datiScelti[i]);
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }
    }

    ImGui::End();
}
