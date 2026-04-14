#include "SchermataVisualizzaFunzioni.h"
#include <vector>
#include <string>

using namespace std;

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

    ImGui::Text("Elenco delle Operazioni Matematiche e Statistiche supportate");
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Operazioni di Base")) {
        ImGui::BulletText("Potenza: calcola la potenza dato un valore base e un esponente.");
        ImGui::BulletText("Radice: calcola la radice di un numero dato un indice.");
        ImGui::BulletText("Logaritmo: calcola il logaritmo di un argomento rispetto a una data base.");
        ImGui::BulletText("Sommatoria: calcola la somma di una serie di valori float.");
        ImGui::BulletText("Produttoria: calcola il prodotto di una serie di valori float.");
    }

    if (ImGui::CollapsingHeader("Operazioni Statistiche")) {
        ImGui::BulletText("Media Aritmetica: calcola la somma dei valori divisa per il loro numero totale.");
        ImGui::BulletText("Media Geometrica: calcola la radice n-esima del prodotto di n valori.");
        ImGui::BulletText("Media Armonica: reciproco della media aritmetica dei reciproci dei valori.");
        ImGui::BulletText("Media Quadratica: media basata sul quadrato dei valori.");
        ImGui::BulletText("Media Ponderata: media in cui ogni dato ha un peso associato.");
        ImGui::BulletText("Deviazione Standard: indica quanto ampiamente la serie diverge dalla media.");
        ImGui::BulletText("Varianza: indica la variabilità dei valori della distribuzione.");
        ImGui::BulletText("Mediana: valore centrale in una distribuzione ordinata.");
        ImGui::BulletText("Moda: il valore o i valori più frequenti in una distribuzione.");
        ImGui::BulletText("Frequenza Assoluta/Relativa: calcolo della frequenza con cui compare ogni valore.");
        ImGui::BulletText("Percentili: calcola un certo percentile dalla distribuzione.");
        ImGui::BulletText("Scarto Semplice e Quadratico Medio: differenze misurate rispetto alla media.");
        ImGui::BulletText("Distribuzione Gaussiana: distribuzione di probabilità normale.");
    }

    ImGui::End();
}
