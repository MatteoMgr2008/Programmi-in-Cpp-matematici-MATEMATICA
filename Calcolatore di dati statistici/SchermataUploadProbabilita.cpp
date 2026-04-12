#include "SchermataUploadProbabilita.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <cmath>
#include "OperazioniStatistiche.h"

using namespace std;

// ── struttura per la tabella bivariata ──────────────────────────────────────
struct TabellaBivariata {
    vector<string>          intestazioni_colonne; // es. Fumatore, NonFumatore
    vector<string>          intestazioni_righe;   // es. M, F
    vector<vector<float>>   valori;               // frequenze osservate
};

struct StatisticheDescrittive {
    float media = 0.0f;
    float mediana = 0.0f;
    float moda = 0.0f;
    float varianza = 0.0f;
    float deviazione_standard = 0.0f;
    float scarto_semplice_medio = 0.0f;
};

// ── lettura CSV bivariato ────────────────────────────────────────────────────
inline TabellaBivariata LeggiCSVBivariato(const string& percorso)
{
    TabellaBivariata tb;
    ifstream file(percorso);
    if (!file.is_open()) return tb;

    string linea;
    bool prima_riga = true;

    while (getline(file, linea))
    {
        // rimuovi \r finale (file Windows)
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();

        stringstream ss(linea);
        string cella;
        vector<string> celle;

        while (getline(ss, cella, ';'))
        {
            cella.erase(0, cella.find_first_not_of(" \t"));
            cella.erase(cella.find_last_not_of(" \t") + 1);
            celle.push_back(cella);
        }

        if (celle.empty()) continue;

        if (prima_riga)
        {
            // celle[0] = cella vuota (angolo), celle[1..] = intestazioni colonne
            for (int j = 1; j < (int)celle.size(); j++)
                tb.intestazioni_colonne.push_back(celle[j]);
            prima_riga = false;
        }
        else
        {
            tb.intestazioni_righe.push_back(celle[0]);
            vector<float> riga;
            for (int j = 1; j < (int)celle.size(); j++)
            {
                try { riga.push_back(stof(celle[j])); }
                catch (...) { riga.push_back(0.0f); }
            }
            tb.valori.push_back(riga);
        }
    }
    return tb;
}

// ── apri esplora file (già presente nel tuo progetto, lasciala inline) ───────
inline string ApriEsploraFileProbabilita()
{
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
    if (GetOpenFileNameA(&ofn) == TRUE) return ofn.lpstrFile;
    return "";
}

// ── helper: stampa una tabella ImGui con intestazioni ────────────────────────
static void MostraTabellaImGui(
    const char* id,
    const string& titolo,
    const vector<string>& int_col,
    const vector<string>& int_riga,
    const vector<vector<float>>& dati,
    const char* fmt = "%.4f")
{
    ImGui::Text("%s", titolo.c_str());
    int n_col = (int)int_col.size();
    int n_rig = (int)int_riga.size();

    if (ImGui::BeginTable(id, n_col + 1,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
    {
        // intestazione colonne
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        for (const auto& c : int_col)
            ImGui::TableSetupColumn(c.c_str(), ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableHeadersRow();

        // righe dati
        for (int i = 0; i < n_rig; i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", int_riga[i].c_str());
            for (int j = 0; j < n_col && j < (int)dati[i].size(); j++)
            {
                ImGui::TableSetColumnIndex(j + 1);
                ImGui::Text(fmt, dati[i][j]);
            }
        }
        ImGui::EndTable();
    }
}

static StatisticheDescrittive CalcolaStatisticheDescrittive(
    OperazioniStatistiche& operazioni_statistiche,
    const vector<float>& valori)
{
    StatisticheDescrittive statistiche;
    if (valori.empty()) return statistiche;

    statistiche.media = operazioni_statistiche.calcoloMediaAritmetica(valori);
    statistiche.mediana = operazioni_statistiche.calcoloMediana(valori);
    statistiche.moda = operazioni_statistiche.calcoloModa(valori);
    statistiche.varianza = operazioni_statistiche.calcoloVarianza(valori);
    statistiche.deviazione_standard = operazioni_statistiche.calcoloDeviazioneStandard(valori);
    statistiche.scarto_semplice_medio = operazioni_statistiche.calcoloScartoSempliceMedio(valori);
    return statistiche;
}

static void MostraTabellaStatistichePerCaratteristica(
    const char* id,
    const string& titolo,
    const vector<string>& etichette,
    const vector<StatisticheDescrittive>& statistiche)
{
    ImGui::Text("%s", titolo.c_str());
    if (ImGui::BeginTable(id, 7,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableSetupColumn("Caratteristica", ImGuiTableColumnFlags_WidthFixed, 120.0f);
        ImGui::TableSetupColumn("Media", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("Mediana", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("Moda", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("Varianza", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Dev. Std.", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Scarto Semplice Medio", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < (int)etichette.size() && i < (int)statistiche.size(); i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", etichette[i].c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.4f", statistiche[i].media);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%.4f", statistiche[i].mediana);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%.4f", statistiche[i].moda);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%.4f", statistiche[i].varianza);
            ImGui::TableSetColumnIndex(5); ImGui::Text("%.4f", statistiche[i].deviazione_standard);
            ImGui::TableSetColumnIndex(6); ImGui::Text("%.4f", statistiche[i].scarto_semplice_medio);
        }
        ImGui::EndTable();
    }
}

// ════════════════════════════════════════════════════════════════════════════
void SchermataUploadProbabilita(bool& uploadFileProbabilita)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGui::Begin("Statistica Bivariata", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Torna alla Homepage"))
        uploadFileProbabilita = false;

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    ImGui::Text("Carica un file CSV con tabella di contingenza bivariata.");
    ImGui::Text("Formato atteso:  cella vuota ; Col1 ; Col2 ; ...");
    ImGui::Text("                 Riga1       ;  v   ;  v   ; ...");
    ImGui::Spacing();

    static char percorsoFile[512] = "";
    ImGui::InputText("Percorso CSV", percorsoFile, sizeof(percorsoFile));
    ImGui::SameLine();
    if (ImGui::Button("Sfoglia..."))
    {
        string p = ApriEsploraFileProbabilita();
        if (!p.empty()) strcpy_s(percorsoFile, sizeof(percorsoFile), p.c_str());
    }

    static TabellaBivariata tb;
    static string statoCaricamento = "";

    // ── calcoli derivati (ricalcolati ogni volta che si carica un file) ──────
    static vector<vector<float>> prob_cella;
    static vector<float>         marg_riga;
    static vector<float>         marg_col;
    static vector<float>         prob_marg_riga;
    static vector<float>         prob_marg_col;
    static vector<vector<float>> valori_attesi;
    static vector<StatisticheDescrittive> statistiche_righe;
    static vector<StatisticheDescrittive> statistiche_colonne;
    static float                 chi2 = 0.0f;
    static float                 totale = 0.0f;
    static bool                  calcolato = false;

    if (ImGui::Button("Carica ed Elabora"))
    {
        tb = LeggiCSVBivariato(string(percorsoFile));
        calcolato = false;

        if (tb.valori.empty())
        {
            statoCaricamento = "Errore: file vuoto o formato non valido.";
        }
        else
        {
            int R = (int)tb.valori.size();
            int C = (int)tb.intestazioni_colonne.size();
            OperazioniStatistiche operazioni_statistiche;
            vector<vector<float>> distribuzione_marginale;
            vector<vector<float>> probabilita_marginale;

            // ── totale generale ──────────────────────────────────────────────


            // ── probabilità per cella ────────────────────────────────────────
            prob_cella = operazioni_statistiche.calcoloTabellaProbabilita(tb.valori);
            valori_attesi = operazioni_statistiche.calcoloTabellaValoriAttesi(tb.valori);

            // ── probabilità marginali ────────────────────────────────────────
            distribuzione_marginale = operazioni_statistiche.calcoloDistribuzioneMarginale(tb.valori);
            probabilita_marginale = operazioni_statistiche.calcoloProbabilitaMarginale(tb.valori);
            marg_riga = distribuzione_marginale.empty() ? vector<float>{} : distribuzione_marginale[0];
            marg_col = distribuzione_marginale.size() > 1 ? distribuzione_marginale[1] : vector<float>{};
            prob_marg_riga = probabilita_marginale.empty() ? vector<float>{} : probabilita_marginale[0];
            prob_marg_col = probabilita_marginale.size() > 1 ? probabilita_marginale[1] : vector<float>{};
            totale = marg_riga.empty() ? 0.0f : operazioni_statistiche.calcoloSommatoria(marg_riga);

            // ── chi quadro ───────────────────────────────────────────────────
            chi2 = operazioni_statistiche.calcoloDistribuzioneChiQuadrato(tb.valori);

            statistiche_righe.clear();
            for (const auto& riga : tb.valori)
                statistiche_righe.push_back(CalcolaStatisticheDescrittive(operazioni_statistiche, riga));

            statistiche_colonne.clear();
            for (int j = 0; j < C; j++)
            {
                vector<float> colonna;
                for (int i = 0; i < R; i++)
                {
                    if (j < (int)tb.valori[i].size())
                        colonna.push_back(tb.valori[i][j]);
                }
                statistiche_colonne.push_back(CalcolaStatisticheDescrittive(operazioni_statistiche, colonna));
            }

            statoCaricamento = "Elaborazione completata. Totale osservazioni: " + to_string((int)totale);
            calcolato = true;
        }
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", statoCaricamento.c_str());
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    if (!calcolato) { ImGui::End(); return; }

    int R = (int)tb.valori.size();
    int C = (int)tb.intestazioni_colonne.size();

    // ── riga 1: frequenze osservate  |  probabilità per cella ───────────────
    ImGui::Columns(2, "riga1", false);

    MostraTabellaImGui("TabellaOsservati", "Frequenze osservate",
        tb.intestazioni_colonne, tb.intestazioni_righe, tb.valori, "%.0f");

    ImGui::NextColumn();

    MostraTabellaImGui("TabellaProbCella", "Probabilita per cella  P(Xi, Yj)",
        tb.intestazioni_colonne, tb.intestazioni_righe, prob_cella);

    ImGui::Columns(1);
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // ── riga 2: probabilità marginali righe  |  probabilità marginali colonne
    ImGui::Columns(2, "riga2", false);

    // marginali righe
    ImGui::Text("Probabilita marginali per riga  P(Xi)");
    if (ImGui::BeginTable("TabMargRiga", 2,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableSetupColumn("Categoria", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("P(Xi)", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableHeadersRow();
        for (int i = 0; i < R; i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", tb.intestazioni_righe[i].c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.4f", prob_marg_riga[i]);
        }
        ImGui::EndTable();
    }

    ImGui::NextColumn();

    // marginali colonne
    ImGui::Text("Probabilita marginali per colonna  P(Yj)");
    if (ImGui::BeginTable("TabMargCol", 2,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableSetupColumn("Categoria", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("P(Yj)", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableHeadersRow();
        for (int j = 0; j < C; j++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", tb.intestazioni_colonne[j].c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.4f", prob_marg_col[j]);
        }
        ImGui::EndTable();
    }

    ImGui::Columns(1);
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // ── chi quadro ───────────────────────────────────────────────────────────
    MostraTabellaImGui("TabellaAttesi", "Valori attesi",
        tb.intestazioni_colonne, tb.intestazioni_righe, valori_attesi);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    int gdl = (R - 1) * (C - 1);
    ImGui::Text("Chi quadro (χ²)");
    ImGui::Spacing();
    if (ImGui::BeginTable("TabellaChi2", 2,
        ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableSetupColumn("Statistica", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("Valore", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("χ² calcolato");
        ImGui::TableSetColumnIndex(1); ImGui::Text("%.4f", chi2);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("Gradi di liberta (R-1)x(C-1)");
        ImGui::TableSetColumnIndex(1); ImGui::Text("%d", gdl);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("Totale osservazioni (N)");
        ImGui::TableSetColumnIndex(1); ImGui::Text("%.0f", totale);

        ImGui::EndTable();
    }

	// Calcolo moda e mediana per frequenze osservate (non per probabilità) 




    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    MostraTabellaStatistichePerCaratteristica(
        "StatisticheRighe",
        "Statistiche descrittive per ogni riga",
        tb.intestazioni_righe,
        statistiche_righe);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    MostraTabellaStatistichePerCaratteristica(
        "StatisticheColonne",
        "Statistiche descrittive per ogni colonna",
        tb.intestazioni_colonne,
        statistiche_colonne);

    ImGui::End();
}
