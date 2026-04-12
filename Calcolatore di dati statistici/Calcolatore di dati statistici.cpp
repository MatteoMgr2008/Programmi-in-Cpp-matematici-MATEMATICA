// Definizione di alcune macro per includere le funzionalità di ImGui e GLFW
#define GLFW_EXPOSE_NATIVE_WIN32

// Librerie necessarie per il programma
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <io.h>
#include <cstdio>
#include <string>

// Schermate (pagine) del programma
#include "Homepage.h"
#include "SchermataTestFunzioni.h"
#include "SchermataVisualizzaFunzioni.h"
#include "SchermataUploadDati.h"
#include "SchermataUploadProbabilita.h"

// Fonts del programma
#include "HeptaSlabFont.h"
#include "IconsFontAwesome7.h"

// Icone del programma
#include "resource.h" // Per includere le icone dal file "Calcolatore di dati statistici.rc"

// Spazio dei nomi standard (per evitare di scrivere std:: prima di ogni cosa)
using namespace std;

// Variabili globali per gestire la chiusura della finestra e mostrare il popup di conferma
GLFWwindow* finestra_principale_software = nullptr; // Puntatore alla finestra principale del software (usato per la chiusura della finestra)
bool* puntatore_uscita_software = nullptr; // Puntatore alla variabile che tiene traccia se mostrare o meno la schermata di uscita dal software (usato per mostrare il popup di conferma)

// Callback per la chiusura della finestra (viene chiamata quando l'utente clicca sulla "X" della finestra del software)
void callback_chiusura_finestra(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, GLFW_FALSE); // Impedisce la chiusura immediata della finestra (così da poter mostrare il popup di conferma)
	*puntatore_uscita_software = true; // Mostra la schermata di uscita dal software (popup di conferma)
}

// Funzione principale per le applicazioni Windows (sostituisce il main)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inizializza GLFW (necessario prima di usarlo)
    glfwInit();

    // Ottiene la risoluzione del monitor primario
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Crea una finestra GLFW con dimensione dello schermo e titolo
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Software calcolatore di dati statistici", NULL, NULL);
	finestra_principale_software = window; // Assegna il puntatore alla finestra principale del software alla variabile globale (usato per la chiusura della finestra tramite popup di conferma)
	glfwSetWindowCloseCallback(window, callback_chiusura_finestra); // Imposta la callback per la chiusura della finestra (così da poter mostrare il popup di conferma)
    glfwMakeContextCurrent(window); // Imposta il contesto OpenGL su questa finestra
    glfwSwapInterval(1); // Attiva V-Sync (sincronizzazione verticale)

    // Aggiunge l'icona (ovvero il logo del software) alla finestra
    HWND hwnd = glfwGetWin32Window(window);
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALCOLATOREDIDATISTATISTICI));
    if (hIcon) {
        SendMessageA(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessageA(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }

    // Massimizza la finestra
    glfwMaximizeWindow(window);

    // Inizializza il contesto ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Ottiene l'oggetto IO (può essere usato per input, configurazioni, ecc.)

	// Variabile da mettere sopra, tra le altre globali o prima del ciclo principale, per tenere traccia del tema scuro o chiaro
    bool darkMode = true; // Tema scuro di default
    
    // Caricamento dei font di tutto il programma
    ImFontConfig font_default_config;
    font_default_config.FontDataOwnedByAtlas = false;
    ImFont* font_default = io.Fonts->AddFontFromMemoryTTF(font_hepta_slab, grandezza_font_hepta_slab, 25.0f, &font_default_config);
    io.FontDefault = font_default;

	// Caricamento delle icone di tutto il programma (da font awesome)
    static const ImWchar range_caratteri_icone[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig font_icone_config;
    font_icone_config.MergeMode = true;
    font_icone_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Font Awesome 7 Free-Solid-900.otf", 25.0f, &font_icone_config, range_caratteri_icone);

    // Inizializza i backend per GLFW e OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130"); // Specifica la versione di GLSL da usare

	bool upload_file_dati = false; // Variabile per mostrare la schermata di upload dei file di dati
	bool visualizza_funzioni_statistiche = false; // Variabile per mostrare la schermata di visualizzazione delle funzioni base e statistiche
	bool uscita_software = false; // Variabile per mostrare la schermata di uscita dal software
	bool test_funzioni_statistiche = false; // Variabile per mostrare la schermata di test delle funzioni base e statistiche
	bool upload_file_probabilita = false; // Variabile per mostrare la schermata di upload dei file di probabilità

	// Assegna i puntatori alle variabili globali (usati per mostrare le schermate e per la chiusura della finestra tramite popup di conferma)
    puntatore_uscita_software = &uscita_software;

    // Ciclo principale del programma (valido finché la finestra non viene chiusa dall'utente)
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Gestisce gli eventi in input (mouse, tastiera, ecc...)

        // Inizio di un nuovo frame per ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Mostra una schermata diversa in base allo stato della variabile
        if (upload_file_dati == true) {
            // Mostra la schermata per l'upload dei file di dati
            SchermataUploadDati(upload_file_dati);
        }
        else if (visualizza_funzioni_statistiche == true) {
            // Mostra la schermata per visualizzare le funzioni statistiche
			SchermataVisualizzaFunzioni(visualizza_funzioni_statistiche);
        }
        else if (test_funzioni_statistiche == true) {
            // Mostra la schermata per testare le funzioni base e statistiche
			SchermataTestFunzioni(test_funzioni_statistiche);
		}
		else if (upload_file_probabilita == true) {
            // Mostra la schermata per l'upload dei file di probabilità
			SchermataUploadProbabilita(upload_file_probabilita);
        }
        else if (uscita_software == true) {
            // Mostra la schermata per l'uscita dal software

			// Variabili statiche per gestire l'animazione di apertura e chiusura del popup di uscita dal software
			static float trasparenza_overlay = 0.0f; // Trasparenza dell'overlay scuro dietro il popup di uscita
			static bool chiusura_popup_in_corso = false; // Variabile per indicare se l'animazione di chiusura del popup è in corso

			// Animazione di apertura del popup di uscita dal software
            if (!chiusura_popup_in_corso && trasparenza_overlay < 0.35f) {
                trasparenza_overlay += ImGui::GetIO().DeltaTime * 1.5f; // Velocità di apertura
                if (trasparenza_overlay > 0.35f) trasparenza_overlay = 0.35f;
            }

			// Animazione di chiusura del popup di uscita dal software
            if (chiusura_popup_in_corso) {
                trasparenza_overlay -= ImGui::GetIO().DeltaTime * 1.5f; // Indica la velocità di chiusura
                if (trasparenza_overlay <= 0.0f) {
                    trasparenza_overlay = 0.0f;
                    chiusura_popup_in_corso = false;
                    uscita_software = false; // Torna alla homepage solo quando l'animazione è finita
                }
            }

			// Mostra la schermata principale (homepage) sotto il popup di uscita dal software, in modo da non lasciare lo sfondo vuoto
            Homepage(upload_file_dati, visualizza_funzioni_statistiche, uscita_software, test_funzioni_statistiche, upload_file_probabilita);

			// Variabili necessarie per la schermata di uscita dal software
            string messaggio_conferma_uscita = "Sei sicuro di voler uscire dal software?";
			float grandezza_font_messaggio_uscita = 1.2f; // Grandezza del font per il messaggio di conferma dell'uscita dal software
			float grandezza_font_pulsante_uscita = 1.0f; // Grandezza del font per i pulsanti di conferma e annullamento dell'uscita dal software
			float larghezza_pulsante_uscita = 150.0f; // Larghezza dei pulsanti di uscita del popup
			float altezza_pulsante_uscita = 40.0f; // Altezza dei pulsanti di uscita del popup
			float spaziatura_pulsante_uscita = 8.0f; // Spaziatura tra i pulsanti di uscita del popup
            float larghezza_popup_uscita = 684.0f; // Larghezza del popup di uscita
            float altezza_popup_uscita = 103.0f; // Altezza del popup di uscita
            
			// Apre un popup per confermare l'uscita dal software
            ImGui::OpenPopup("Uscita dal software");

			// Imposta lo sfondo scuro e trasparente per il popup di uscita dal software
            ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0, 0.88f));

			// Imposta la posizione, dimensione e caratteristiche della finestra di uscita dal software
            ImVec2 posizione_centrale_popup = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(posizione_centrale_popup, ImGuiCond_Always, ImVec2(0.5f, 0.5f)); // Posizione centrale della finestra di uscita (0.5f, 0.5f indica che la posizione è centrata rispetto alla dimensione della finestra)
            ImGui::SetNextWindowSize(ImVec2(larghezza_popup_uscita, altezza_popup_uscita), ImGuiCond_Always); // Dimensione fissa della finestra di uscita (350x130 pixel)
            if (ImGui::BeginPopupModal("Uscita dal software", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
            {
				if (chiusura_popup_in_corso == false) {
                    // Centra il testo del messaggio nella finestra di uscita
					ImGui::SetWindowFontScale(grandezza_font_messaggio_uscita); // Imposta la grandezza del font per il messaggio di conferma dell'uscita dal software
                    float larghezza_testo_messaggio_popup = ImGui::CalcTextSize(messaggio_conferma_uscita.c_str()).x; // Calcola la larghezza del testo del messaggio di conferma dell'uscita dal software in base alla grandezza del font
                    ImGui::Spacing(); // Aggiunge uno spazio verticale per distanziare il testo dai bordi della finestra
                    ImGui::SetCursorPosX((larghezza_popup_uscita - larghezza_testo_messaggio_popup) / 2); // Centra il testo orizzontalmente nella finestra di uscita
                    ImGui::Text("%s", messaggio_conferma_uscita.c_str()); // Testo della finestra di uscita
                    ImGui::Spacing();
                    ImGui::Spacing();
            
			        // Imposta i pulsanti di conferma e annullamento per l'uscita dal software
			        ImGui::SetWindowFontScale(grandezza_font_pulsante_uscita); // imposta la grandezza del font per i pulsanti di uscita
                    ImGui::SetCursorPosX((larghezza_popup_uscita - larghezza_pulsante_uscita * 2 - spaziatura_pulsante_uscita) / 2); // Centra i bottoni orizzontalmente nella finestra di uscita
                    
                    // Pulsante "Sì, confermo" - colore verde
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.55f, 0.18f, 1.0f)); // Verde normale
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.22f, 0.70f, 0.22f, 1.0f)); // Verde più chiaro all'hover
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.13f, 0.40f, 0.13f, 1.0f)); // Verde più scuro quando premuto
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounding di base dei bordi del pulsante
                    if (ImGui::Button("Sì, confermo", ImVec2(larghezza_pulsante_uscita, altezza_pulsante_uscita))) {
				        glfwSetWindowShouldClose(window, GLFW_TRUE); // Chiude la finestra e termina il programma
			        }
                    ImGui::PopStyleColor(3); // Rimuove i 3 colori del pulsante verde
                    ImGui::PopStyleVar(); // Rimuove il rounding dei bordi del pulsante
					ImGui::SameLine(0.0f, spaziatura_pulsante_uscita); // Aggiunge una spaziatura orizzontale tra i due pulsanti

                    // Pulsante "No, annulla" - colore rosso
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.18f, 0.18f, 1.0f)); // Rosso normale
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.22f, 0.22f, 1.0f)); // Rosso più chiaro all'hover
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.55f, 0.13f, 0.13f, 1.0f)); // Rosso più scuro quando premuto
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounding di base dei bordi del pulsante
                    if (ImGui::Button("No, annulla", ImVec2(larghezza_pulsante_uscita, altezza_pulsante_uscita))) {
						chiusura_popup_in_corso = true; // Avvia l'animazione di chiusura del popup di uscita
			        }
                    ImGui::PopStyleColor(3); // Rimuove i 3 colori del pulsante rosso
                    ImGui::PopStyleVar(); // Rimuove il rounding dei bordi del pulsante
                }

			    // Termina il popup di uscita dal software
                ImGui::EndPopup();
            }

			// Rimuove lo stile del popup di uscita dal software
            ImGui::PopStyleColor();
        }
        else {
			// Mostra la schermata principale (homepage)
			Homepage(upload_file_dati, visualizza_funzioni_statistiche, uscita_software, test_funzioni_statistiche, upload_file_probabilita);
        }

        // Rendering ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h); // Ottiene la dimensione della finestra
        glViewport(0, 0, display_w, display_h); // Imposta l'area di disegno OpenGL
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Imposta il colore di sfondo (grigio scuro)
        glClear(GL_COLOR_BUFFER_BIT); // Pulisce lo schermo
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Disegna tutto con ImGui

        glfwSwapBuffers(window); // Mostra il risultato a schermo
    }

    // Pulizia e chiusura delle librerie
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window); // Chiude la finestra
    glfwTerminate(); // Termina GLFW

    return 0; // Uscita normale
}