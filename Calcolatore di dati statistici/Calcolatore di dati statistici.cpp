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

// Schermate (pagine) del programma
#include "Homepage.h"
#include "SchermataTestFunzioni.h"
#include "SchermataVisualizzaFunzioni.h"
#include "SchermataUploadDati.h"

// Fonts del programma
#include "HeptaSlabFont.h"
#include "IconsFontAwesome7.h"

// Icone del programma
#include "resource.h" // Per includere le icone dal file "Calcolatore di dati statistici.rc"

// Spazio dei nomi standard (per evitare di scrivere std:: prima di ogni cosa)
using namespace std;

// Funzione principale per le applicazioni Windows (sostituisce il main)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Inizializza GLFW (necessario prima di usarlo)
    glfwInit();
    // Ottiene la risoluzione del monitor primario
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Crea una finestra GLFW con dimensione dello schermo e titolo
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Software calcolatore di dati statistici", NULL, NULL);
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
        else if (uscita_software == true) {
            // Mostra la schermata per l'uscita dal software
			// TODO: implementare la schermata di uscita dal software (con conferma di uscita SI/NO)
		}
        else {
			// Mostra la schermata principale (homepage)
			Homepage(upload_file_dati, visualizza_funzioni_statistiche, uscita_software, test_funzioni_statistiche);
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