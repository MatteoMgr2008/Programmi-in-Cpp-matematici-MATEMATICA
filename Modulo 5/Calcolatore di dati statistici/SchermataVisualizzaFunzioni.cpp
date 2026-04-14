#include "SchermataVisualizzaFunzioni.h"
#include <vector>
#include <string>

using namespace std;

// Funzione per visualizzare la descrizione di un'operazione base con un titolo colorato di verde e una descrizione in colore normale accanto
void StrutturaDescrizioneOperazioneBase(const char* titolo_operazione, const char* descrizione_operazione) {
    ImGui::Bullet();
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), titolo_operazione); // colore verde per il titolo dell'operazione
    ImGui::SameLine(0, 4);
    ImGui::Text(descrizione_operazione);
}

// Funzione per visualizzare la descrizione di un'operazione statistica con un titolo colorato di giallo e una descrizione in colore normale accanto
void StrutturaDescrizioneOperazioneStatistica(const char* titolo_operazione, const char* descrizione_operazione) {
    ImGui::Bullet();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), titolo_operazione); // colore giallo per il titolo dell'operazione
    ImGui::SameLine(0, 4);
    ImGui::Text(descrizione_operazione);
}

void SchermataVisualizzaFunzioni(bool& visualizza_funzioni_statistiche) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    ImGui::Begin("Lista delle funzioni base e statistiche esistenti", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Pulsante per tornare alla Homepage
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounding di base dei bordi del pulsante
    if (ImGui::Button(ICON_FA_ARROW_LEFT "  Torna alla Homepage")) {
        visualizza_funzioni_statistiche = false;
    }
    ImGui::PopStyleVar();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

	// Testo introduttivo alla lista delle operazioni di base e statistiche esistenti
    ImGui::Text("Elenco delle operazioni matematiche di base e statistiche esistenti in questo software:");
    ImGui::Spacing();

	// Sezione per la lista delle operazioni di base
    if (ImGui::CollapsingHeader("Operazioni di base")) {
        StrutturaDescrizioneOperazioneBase("Potenza:", "calcola la potenza dato un valore base e un esponente.");
        StrutturaDescrizioneOperazioneBase("Radice:", "calcola la radice di un numero dato un indice.");
        StrutturaDescrizioneOperazioneBase("Logaritmo:", "calcola il logaritmo di un argomento rispetto a una data base.");
        StrutturaDescrizioneOperazioneBase("Sommatoria:", "calcola la somma di una serie di valori float.");
        StrutturaDescrizioneOperazioneBase("Produttoria:", "calcola il prodotto di una serie di valori float.");
        StrutturaDescrizioneOperazioneBase("Valore assoluto:", "restituisce il valore numerico senza il suo segno.");
    }

	// Sezione per la lista delle operazioni statistiche
    if (ImGui::CollapsingHeader("Operazioni statistiche")) {
        StrutturaDescrizioneOperazioneStatistica("Media aritmetica:", "calcola la somma dei valori divisa per il loro numero totale.");
        StrutturaDescrizioneOperazioneStatistica("Media geometrica:", "calcola la radice n-esima del prodotto di n valori.");
        StrutturaDescrizioneOperazioneStatistica("Media armonica:", "reciproco della media aritmetica dei reciproci dei valori.");
        StrutturaDescrizioneOperazioneStatistica("Media quadratica:", "media basata sul quadrato dei valori.");
        StrutturaDescrizioneOperazioneStatistica("Media ponderata:", "media in cui ogni dato ha un peso associato.");
        StrutturaDescrizioneOperazioneStatistica("Deviazione standard:", "indica quanto ampiamente la serie diverge dalla media.");
        StrutturaDescrizioneOperazioneStatistica("Varianza:", "indica la variabilità dei valori della distribuzione.");
        StrutturaDescrizioneOperazioneStatistica("Mediana:", "valore centrale in una distribuzione ordinata.");
        StrutturaDescrizioneOperazioneStatistica("Moda:", "il valore o i valori piu' frequenti in una distribuzione.");
        StrutturaDescrizioneOperazioneStatistica("Frequenza assoluta:", "numero di volte in cui un valore compare nella distribuzione.");
        StrutturaDescrizioneOperazioneStatistica("Frequenza relativa:", "rapporto tra la frequenza assoluta e il numero totale di valori.");
        StrutturaDescrizioneOperazioneStatistica("Percentili:", "calcola un certo percentile dalla distribuzione.");
        StrutturaDescrizioneOperazioneStatistica("Scarto semplice medio:", "media delle differenze assolute dei valori rispetto alla media.");
        StrutturaDescrizioneOperazioneStatistica("Scarto quadratico medio:", "radice quadrata della media degli scarti al quadrato.");
        StrutturaDescrizioneOperazioneStatistica("Distribuzione gaussiana:", "distribuzione di probabilità normale.");
        StrutturaDescrizioneOperazioneStatistica("Varianza ponderata:", "media ponderata dei quadrati degli scarti dei valori dalla media ponderata.");
        StrutturaDescrizioneOperazioneStatistica("Deviazione standard ponderata:", "radice quadrata della varianza ponderata.");
        StrutturaDescrizioneOperazioneStatistica("Mediana ponderata:", "valore centrale che tiene conto delle frequenze associate a ciascun valore.");
        StrutturaDescrizioneOperazioneStatistica("Covarianza:", "misura la relazione lineare tra due variabili, positiva o negativa.");
        StrutturaDescrizioneOperazioneStatistica("Coefficente di correlazione di Bravais-Pearson:", "coefficiente che indica la forza della correlazione lineare tra due variabili.");
        StrutturaDescrizioneOperazioneStatistica("Coefficiente angolare regressione Y su X:", "pendenza della retta che approssima la relazione lineare di Y rispetto a X.");
        StrutturaDescrizioneOperazioneStatistica("Coefficiente angolare regressione X su Y:", "pendenza della retta che approssima la relazione lineare di X rispetto a Y.");
        StrutturaDescrizioneOperazioneStatistica("Intercetta regressione Y su X:", "valore di Y quando X e' uguale a zero sulla retta di regressione.");
        StrutturaDescrizioneOperazioneStatistica("Intercetta regressione X su Y:", "valore di X quando Y e' uguale a zero sulla retta di regressione.");
        StrutturaDescrizioneOperazioneStatistica("Distribuzione chi quadrato:", "misura la differenza tra i valori osservati e i valori attesi in una tabella.");
        StrutturaDescrizioneOperazioneStatistica("Tabella valori attesi:", "matrice dei valori teorici attesi calcolati dai totali di riga e colonna.");
        StrutturaDescrizioneOperazioneStatistica("Tabella probabilità:", "matrice delle probabilità associate a ciascuna cella di una tabella.");
        StrutturaDescrizioneOperazioneStatistica("Distribuzione marginale:", "totali di ciascuna riga e colonna di una tabella.");
        StrutturaDescrizioneOperazioneStatistica("Probabilità marginale:", "probabilità associate ai totali di ciascuna riga e colonna.");
    }

	// Fine della lista delle operazioni di base e statistiche esistenti
    ImGui::End();
}