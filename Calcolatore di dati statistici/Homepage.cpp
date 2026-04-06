#include "Homepage.h"
#include <string>

using namespace std;

void Homepage(bool& upload_file_dati, bool& visualizza_funzioni_statistiche, bool& uscita_software) {
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

	// Spaziatura verticale dopo il titolo e il sottotitolo
	ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

	// Variabili per la posizione e dimensione dei pulsanti
	float larghezza_pulsante_standard = 900.0f; // Larghezza del pulsante
	float altezza_pulsante_standard = 50.0f; // Altezza del pulsante
	float distanza_pulsanti_standard = 10.0f; // Spazio verticale tra i pulsanti
	float posizione_X_pulsante_standard = (ImGui::GetWindowSize().x - larghezza_pulsante_standard) * 0.5f; // Posizione X centrata del pulsante
    
	// Stile generale dei pulsanti
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f)); // Colore di base del pulsante
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f)); // Colore di base del pulsante quando è hovered
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.3f, 0.7f, 1.0f)); // Colore di base del pulsante quando è active
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounding di base dei bordi del pulsante
    ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    ImGui::PopStyleColor(3); // Ripristina i colori originali

	// Pulsante 1 (verde): "Caricamento file CSV/XLSX per acquisizione dati"
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.6f, 0.2f, 1.0f)); // Verde (colore normale)
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.8f, 0.3f, 1.0f)); // Verde acceso (colore hovered)
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.4f, 0.1f, 1.0f)); // Verde scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button("Carica il file CSV/XLSX per l'acquisizione dei dati da analizzare", ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        upload_file_dati = true; // Imposta la variabile true per il caricamento del file dati CSV/XLSX
    }
    ImGui::PopStyleColor(3); // Ripristina i colori originali

	// Spaziatura tra i pulsanti
    ImGui::Dummy(ImVec2(0.0f, distanza_pulsanti_standard));

	// Pulsante 2 (Arancione): "Visualizzazione della lista di funzioni base e statistiche esistenti"
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.6f, 0.0f, 1.0f)); // Arancione (colore normale)
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.7f, 0.2f, 1.0f)); // Arancione acceso (colore hovered)
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.5f, 0.0f, 1.0f)); // Arancione scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button("Visualizza la lista di funzioni base e statistiche esistenti", ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        visualizza_funzioni_statistiche = true; // Imposta la variabile true per la visualizzazione delle funzioni statistiche
    }
    ImGui::PopStyleColor(3); // Ripristina i colori originali

    // Spaziatura tra i pulsanti
    ImGui::Dummy(ImVec2(0.0f, distanza_pulsanti_standard));

	// Pulsante 3 (fucsia): "Uscita dal software"
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.5f, 1.0f)); // Fucsia (colore normale)
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.7f, 1.0f)); // Fucsia acceso (colore hovered)
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.4f, 1.0f)); // Fucsia scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button("Esci dal software", ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        uscita_software = true; // Imposta la variabile true per permettere l'uscita dal software
	}
    ImGui::PopStyleColor(3); // Ripristina i colori originali

	// Ripristina lo stile originale dei pulsanti (rounding)
    ImGui::PopStyleVar();
    
	// Comando di conclusione della schermata Homepage
    ImGui::End();
}