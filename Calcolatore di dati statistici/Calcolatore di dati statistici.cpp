#include "imgui.h"
#include "HeptaSlabFont.h"
#include <io.h>
#include <cstdio>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>

using namespace std;

// Funzione principale per le applicazioni Windows (sostituisce il main)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Inizializza GLFW (necessario prima di usarlo)
    glfwInit();
    // Ottiene la risoluzione del monitor primario
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Crea una finestra GLFW con dimensione dello schermo e titolo
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Calcolatore di dati statistici", NULL, NULL);
    glfwMakeContextCurrent(window); // Imposta il contesto OpenGL su questa finestra
    glfwSwapInterval(1); // Attiva V-Sync (sincronizzazione verticale)

    // Massimizza la finestra
    glfwMaximizeWindow(window);

    // Inizializza il contesto ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Ottiene l'oggetto IO (può essere usato per input, configurazioni, ecc.)

    // Variabile da mettere sopra, tra le altre globali o prima del ciclo
    bool darkMode = true; // Tema scuro di default

    // Caricamento dei font di tutto il programma
    ImFontConfig font_default_config;
    font_default_config.FontDataOwnedByAtlas = false;
    ImFont* font_default = io.Fonts->AddFontFromMemoryTTF(font_hepta_slab, grandezza_font_hepta_slab, 25.0f, &font_default_config);
    io.FontDefault = font_default;

    // Inizializza i backend per GLFW e OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130"); // Specifica la versione di GLSL da usare

    // Ciclo principale dell'applicazione (finché la finestra non viene chiusa dall'utente)
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Gestisce gli eventi in input (mouse, tastiera, ecc...)

        // Inizio di un nuovo frame per ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Finestra di test
        ImGui::Begin("Test del font");
        ImGui::Text("Testo di prova con il font HeptaSlab!");
        ImGui::End();

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