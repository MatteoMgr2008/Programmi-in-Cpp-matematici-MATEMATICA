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
    //ThemeManager& themeManager = ThemeManager::getInstance();
    //int theme = themeManager.getCurrentThemeInt();

	// Visualizza il titolo del programma
	string titoloSoftware = "Calcolatore di dati statistici";
	float scala_font_titolo = 1.7f; // questo valore serve per regolare la grandezza del font del titolo

	ImGui::SetWindowFontScale(scala_font_titolo); // imposta la scala del font per il titolo
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Usa il font predefinito (già impostato)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.15f, 0.05f, 1.0f)); // Colore rosso per il titolo
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(titoloSoftware.c_str()).x) * 0.5f);
    ImGui::Text("%s", titoloSoftware.c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::SetWindowFontScale(1.0f); // ripristina la scala normale

	// Sottotitolo o slogan del programma (indica brevemente la funzione del software)
    string sottotitoloSoftware = "Analizza e calcola dati statistici in modo semplice, veloce e preciso!";
    float scala_font_sottotitolo = 1.2f; // questo valore serve per regolare la grandezza del font del sottotitolo

    ImGui::SetWindowFontScale(scala_font_sottotitolo); // imposta la scala del font per il sottotitolo
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Usa il font predefinito (già impostato)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Colore bianco per il sottotitolo
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(sottotitoloSoftware.c_str()).x) * 0.5f);
    ImGui::Text("%s", sottotitoloSoftware.c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::SetWindowFontScale(1.0f); // ripristina la scala normale

	// Comando di conclusione della schermata Homepage
    ImGui::End();
}