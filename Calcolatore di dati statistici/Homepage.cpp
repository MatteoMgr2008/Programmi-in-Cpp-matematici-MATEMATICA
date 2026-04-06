#include "Homepage.h"
#include <string>

using namespace std;

void Homepage() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    // Inizio della schermata home (Homepage)
    ImGui::Begin("Homepage", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    // Spaziatura verticale iniziale
    ImGui::Dummy(ImVec2(0.0f, 100.0f));

    // Ottieni il theme manager
    // ThemeManager& themeManager = ThemeManager::getInstance();
    // int theme = themeManager.getCurrentThemeInt();

	// Visualizza il titolo del programma
	string titolo_software = "Calcolatore di dati statistici"; // Titolo del programma (indica il nome del software)
	float scala_font_titolo = 1.7f; // questo valore serve per regolare la grandezza del font del titolo

	ImGui::SetWindowFontScale(scala_font_titolo); // imposta la scala del font per il titolo
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Usa il font predefinito (già impostato)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.15f, 0.05f, 1.0f)); // Colore rosso per il titolo
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(titolo_software.c_str()).x) * 0.5f);
    ImGui::Text("%s", titolo_software.c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::SetWindowFontScale(1.0f); // ripristina la scala normale

    // Visualizza il sottotitolo (lo slogan) del programma
    string slogan_software = "Analizza e calcola dati statistici in modo semplice, veloce e preciso!"; // Sottotitolo o slogan del programma (indica brevemente la funzione del software)
    float scala_font_sottotitolo = 1.2f; // questo valore serve per regolare la grandezza del font del sottotitolo

    ImGui::SetWindowFontScale(scala_font_sottotitolo); // imposta la scala del font per il sottotitolo
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Usa il font predefinito (già impostato)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Colore bianco per il sottotitolo
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(slogan_software.c_str()).x) * 0.5f);
    ImGui::Text("%s", slogan_software.c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::SetWindowFontScale(1.0f); // ripristina la scala normale

	// Spaziatura verticale tra il titolo e il sottotitolo
	ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

	// Variabili per la posizione e dimensione dei pulsanti
	float larghezza_pulsante_standard = 300.0f; // Larghezza del pulsante
	float altezza_pulsante_standard = 50.0f; // Altezza del pulsante
	float distanza_pulsanti_standard = 20.0f; // Spazio verticale tra i pulsanti
	float posizione_X_pulsante_standard = (ImGui::GetWindowSize().x - larghezza_pulsante_standard) * 0.5f; // Posizione X centrata del pulsante
    
	// Stile generale dei pulsanti
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f)); // Colore di base del pulsante
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f)); // Colore di base del pulsante quando è hovered
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.3f, 0.7f, 1.0f)); // Colore di base del pulsante quando è active
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounding di base dei bordi del pulsante

	// Pulsante 1: "Caricamento file CSV/XLSX"

	// Pulsante 2: "Visualizzazione della lista di funzioni statistiche esistenti"

	// Pulsante 3: "Uscita dal software"

	// Comando di conclusione della schermata Homepage
    ImGui::End();
}