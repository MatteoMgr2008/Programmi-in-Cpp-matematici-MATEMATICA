#include "Homepage.h"
#include <string>

using namespace std;

void Homepage(bool& upload_file_dati, bool& visualizza_funzioni_statistiche, bool& uscita_software, bool& test_funzioni_statistiche, bool& upload_file_probabilita) {
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

	// Variabili per il testo dei pulsanti e calcolo della loro larghezza
    string testo_pulsante_1 = "Carica un file CSV per l'acquisizione di dati da analizzare"; // Testo del pulsante 1 (indica la funzione del pulsante)
    float larghezza_testo_pulsante_1 = ImGui::CalcTextSize(testo_pulsante_1.c_str()).x + 20.0f; // Calcola la larghezza del testo del pulsante 1
	string testo_pulsante_2 = "Carica un file CSV per calcolo delle probabilità"; // Testo del pulsante 2 (indica la funzione del pulsante)
	float larghezza_testo_pulsante_2 = ImGui::CalcTextSize(testo_pulsante_2.c_str()).x + 20.0f; // Calcola la larghezza del testo del pulsante 2
    string testo_pulsante_3 = "Visualizza la lista di funzioni base e statistiche esistenti"; // Testo del pulsante 3 (indica la funzione del pulsante)
    float larghezza_testo_pulsante_3 = ImGui::CalcTextSize(testo_pulsante_3.c_str()).x + 20.0f; // Calcola la larghezza del testo del pulsante 3
    string testo_pulsante_4 = "Utilizza singolarmente le funzioni base e statistiche esistenti"; // Testo del pulsante 4 (indica la funzione del pulsante)
	float larghezza_testo_pulsante_4 = ImGui::CalcTextSize(testo_pulsante_4.c_str()).x + 20.0f; // Calcola la larghezza del testo del pulsante 4
    string testo_pulsante_5 = "Esci dal software"; // Testo del pulsante 5 (indica la funzione del pulsante)
    float larghezza_testo_pulsante_5 = ImGui::CalcTextSize(testo_pulsante_5.c_str()).x + 20.0f; // Calcola la larghezza del testo del pulsante 5

	// Variabili per la posizione e dimensione dei pulsanti
	float larghezza_pulsante_standard = max({ larghezza_testo_pulsante_1, larghezza_testo_pulsante_2, larghezza_testo_pulsante_3, larghezza_testo_pulsante_4, larghezza_testo_pulsante_5 }); // Larghezza del pulsante standard basata sulla larghezza del testo più lungo tra i tre pulsanti
	float altezza_pulsante_standard = 50.0f; // Altezza del pulsante standard
	float distanza_pulsanti_standard = 10.0f; // Spazio verticale standard tra i pulsanti
	float posizione_X_pulsante_standard = (ImGui::GetWindowSize().x - larghezza_pulsante_standard) * 0.5f; // Posizione X centrata del pulsante standard
    
	// Stile generale dei pulsanti
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f)); // Colore di base del pulsante
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f)); // Colore di base del pulsante quando è hovered
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.3f, 0.7f, 1.0f)); // Colore di base del pulsante quando è active
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounding di base dei bordi del pulsante
    ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    ImGui::PopStyleColor(3); // Ripristina i colori originali

	// Pulsante 1 (verde): "Caricamento di un file CSV per l'acquisizione di dati"
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.6f, 0.2f, 1.0f)); // Verde (colore normale)
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.8f, 0.3f, 1.0f)); // Verde acceso (colore hovered)
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.4f, 0.1f, 1.0f)); // Verde scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button(testo_pulsante_1.c_str(), ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        upload_file_dati = true; // Imposta la variabile true per il caricamento del file dati CSV/XLSX
    }
	if (ImGui::IsItemHovered()) {
		string testo_tooltip_pulsante_1 = "Carica un file CSV o XLSX contenente dei dati da analizzare. Il file deve essere formattato correttamente per essere accettato dal software."; // Testo del tooltip per il pulsante 1 (fornisce ulteriori dettagli sulla funzione del pulsante)
		float larghezza_tooltip_pulsante_1 = ImGui::CalcTextSize(testo_tooltip_pulsante_1.c_str()).x + 20.0f; // Calcola la larghezza del tooltip per il pulsante 1 in base al testo
        ImGui::SetNextWindowSize(ImVec2(larghezza_tooltip_pulsante_1, 0), ImGuiCond_Always); // Imposta la larghezza del tooltip per il pulsante 1
        ImGui::BeginTooltip(); // Inizia la finestra del tooltip per il pulsante 1
        ImGui::SetCursorPosX((larghezza_tooltip_pulsante_1 - ImGui::CalcTextSize(testo_tooltip_pulsante_1.c_str()).x) * 0.5f); // Posizione X centrata del tooltip rispetto al pulsante 1
		ImGui::Text("%s", testo_tooltip_pulsante_1.c_str()); // Visualizza il testo del tooltip per il pulsante 1
		ImGui::EndTooltip(); // Termina la finestra del tooltip per il pulsante 1
    }
    ImGui::PopStyleColor(3); // Ripristina i colori originali

	// Spaziatura tra i pulsanti
    ImGui::Dummy(ImVec2(0.0f, distanza_pulsanti_standard));

	// Pulsante 2 (Arancione): "Caricamento di un file CSV per il calcolo delle probabilità"
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.40f, 0.00f, 1.0f)); // Arancione (colore normale)
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.95f, 0.50f, 0.05f, 1.0f)); // Arancione acceso (colore hovered)
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.65f, 0.28f, 0.00f, 1.0f)); // Arancione scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button(testo_pulsante_2.c_str(), ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        upload_file_probabilita = true; // Imposta la variabile true per il caricamento del file probabilità CSV
    }
    if (ImGui::IsItemHovered()) {
        string testo_tooltip_pulsante_2 = "Carica un file CSV contenente dei dati per il calcolo delle probabilità. Il file deve essere formattato correttamente per essere accettato dal software."; // Testo del tooltip per il pulsante 2 (fornisce ulteriori dettagli sulla funzione del pulsante)
        float larghezza_tooltip_pulsante_2 = ImGui::CalcTextSize(testo_tooltip_pulsante_2.c_str()).x + 20.0f; // Calcola la larghezza del tooltip per il pulsante 2 in base al testo
        ImGui::SetNextWindowSize(ImVec2(larghezza_tooltip_pulsante_2, 0), ImGuiCond_Always); // Imposta la larghezza del tooltip per il pulsante 2
        ImGui::BeginTooltip(); // Inizia la finestra del tooltip per il pulsante 2
        ImGui::SetCursorPosX((larghezza_tooltip_pulsante_2 - ImGui::CalcTextSize(testo_tooltip_pulsante_2.c_str()).x) * 0.5f); // Posizione X centrata del tooltip rispetto al pulsante 2
        ImGui::Text("%s", testo_tooltip_pulsante_2.c_str()); // Visualizza il testo del tooltip per il pulsante 2
        ImGui::EndTooltip(); // Termina la finestra del tooltip per il pulsante 2
    }   
	ImGui::PopStyleColor(3); // Ripristina i colori originali
	
    // Spaziatura tra i pulsanti
    ImGui::Dummy(ImVec2(0.0f, distanza_pulsanti_standard));

	// Pulsante 3 (Giallo): "Visualizzazione della lista di funzioni base e statistiche esistenti"
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.6f, 0.0f, 1.0f)); // Giallo (colore normale)
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.7f, 0.2f, 1.0f)); // Giallo acceso (colore hovered)
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.5f, 0.0f, 1.0f)); // Giallo scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button(testo_pulsante_3.c_str(), ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        visualizza_funzioni_statistiche = true; // Imposta la variabile true per la visualizzazione delle funzioni statistiche
    }
    if (ImGui::IsItemHovered()) {
        string testo_tooltip_pulsante_3 = "Visualizza la lista delle funzioni di base e statistiche esistenti. Seleziona una funzione in particolare per visualizzarne i dettagli."; // Testo del tooltip per il pulsante 3 (fornisce ulteriori dettagli sulla funzione del pulsante)
		float larghezza_tooltip_pulsante_3 = ImGui::CalcTextSize(testo_tooltip_pulsante_3.c_str()).x + 20.0f; // Calcola la larghezza del tooltip per il pulsante 3 in base al testo
        ImGui::SetNextWindowSize(ImVec2(larghezza_tooltip_pulsante_3, 0), ImGuiCond_Always); // Imposta la larghezza del tooltip per il pulsante 3
        ImGui::BeginTooltip(); // Inizia la finestra del tooltip per il pulsante 3
        ImGui::SetCursorPosX((larghezza_tooltip_pulsante_3 - ImGui::CalcTextSize(testo_tooltip_pulsante_3.c_str()).x) * 0.5f); // Posizione X centrata del tooltip rispetto al pulsante 3
        ImGui::Text("%s", testo_tooltip_pulsante_3.c_str()); // Visualizza il testo del tooltip per il pulsante 3
        ImGui::EndTooltip(); // Termina la finestra del tooltip per il pulsante 3
    }
    ImGui::PopStyleColor(3); // Ripristina i colori originali

    // Spaziatura tra i pulsanti
    ImGui::Dummy(ImVec2(0.0f, distanza_pulsanti_standard));

	// Pulsante 4 (Celeste): "Utilizza singolarmente le funzioni base e statistiche"
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.8f, 1.0f)); // Celeste (colore normale)
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.7f, 1.0f, 1.0f)); // Celeste chiaro (colore hovered)
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.4f, 0.6f, 1.0f)); // Celeste scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button(testo_pulsante_4.c_str(), ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        test_funzioni_statistiche = true;
	}
    if (ImGui::IsItemHovered()) {
        string testo_tooltip_pulsante_4 = "Testa le funzionalità di calcolo inserendo dei valori a piacimento."; // Testo del tooltip per il pulsante 4 (fornisce ulteriori dettagli sulla funzione del pulsante)
		float larghezza_tooltip_pulsante_4 = ImGui::CalcTextSize(testo_tooltip_pulsante_4.c_str()).x + 20.0f; // Calcola la larghezza del tooltip per il pulsante 4 in base al testo
	    ImGui::SetNextWindowSize(ImVec2(larghezza_tooltip_pulsante_4, 0), ImGuiCond_Always); // Imposta la larghezza del tooltip per il pulsante 4
        ImGui::BeginTooltip(); // Inizia la finestra del tooltip per il pulsante 4
        ImGui::SetCursorPosX((larghezza_tooltip_pulsante_4 - ImGui::CalcTextSize(testo_tooltip_pulsante_4.c_str()).x) * 0.5f); // Posizione X centrata del tooltip rispetto al pulsante 4
        ImGui::Text("%s", testo_tooltip_pulsante_4.c_str()); // Visualizza il testo del tooltip per il pulsante 4
		ImGui::EndTooltip(); // Termina la finestra del tooltip per il pulsante 4
    }
    ImGui::PopStyleColor(3); // Ripristina i colori originali

    // Spaziatura tra i pulsanti
    ImGui::Dummy(ImVec2(0.0f, distanza_pulsanti_standard));

	// Pulsante 5 (Fucsia): "Uscita dal software"
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.5f, 1.0f)); // Fucsia (colore normale)
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.7f, 1.0f)); // Fucsia acceso (colore hovered)
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.4f, 1.0f)); // Fucsia scuro (colore active)
	ImGui::SetCursorPosX(posizione_X_pulsante_standard); // Posizione X centrata del pulsante
    if (ImGui::Button(testo_pulsante_5.c_str(), ImVec2(larghezza_pulsante_standard, altezza_pulsante_standard))) {
        uscita_software = true; // Imposta la variabile true per permettere l'uscita dal software
	}
    if (ImGui::IsItemHovered()) {
        string testo_tooltip_pulsante_5 = "Esci dal software. Verrà chiuso il software e tutte le operazioni in corso verranno subito interrotte."; // Testo del tooltip per il pulsante 5 (fornisce ulteriori dettagli sulla funzione del pulsante)
		float larghezza_tooltip_pulsante_5 = ImGui::CalcTextSize(testo_tooltip_pulsante_5.c_str()).x + 20.0f; // Calcola la larghezza del tooltip per il pulsante 5          in base al testo
        ImGui::SetNextWindowSize(ImVec2(larghezza_tooltip_pulsante_5, 0), ImGuiCond_Always); // Imposta la larghezza del tooltip per il pulsante 5
        ImGui::BeginTooltip(); // Inizia la finestra del tooltip per il pulsante 5
        ImGui::SetCursorPosX((larghezza_tooltip_pulsante_5 - ImGui::CalcTextSize(testo_tooltip_pulsante_5.c_str()).x) * 0.5f); // Posizione X centrata del tooltip rispetto al pulsante 5
        ImGui::Text("%s", testo_tooltip_pulsante_5.c_str()); // Visualizza il testo del tooltip per il pulsante 5
        ImGui::EndTooltip(); // Termina la finestra del tooltip per il pulsante 5
	}
    ImGui::PopStyleColor(3); // Ripristina i colori originali

	// Ripristina lo stile originale dei pulsanti (rounding)
    ImGui::PopStyleVar();
    
	// Comando di conclusione della schermata Homepage
    ImGui::End();
}