# Calcolatore di dati statistici

🇮🇹 [Italiano](#italiano) | 🇬🇧 [English](#english)

<a id="italiano"></a>
## 🇮🇹 Italiano

### Descrizione

Programma in C++ con interfaccia grafica, realizzato per **Matematica** (Modulo 5, statistica e probabilità). Il software si chiama **Calcolatore di dati statistici** e il suo slogan è *"Analizza e calcola dati statistici in modo semplice, veloce e preciso!"*.

È un'applicazione desktop per Windows costruita con [Dear ImGui](https://github.com/ocornut/imgui) (backend GLFW + OpenGL 3). Dalla homepage si può:

1. **Caricare un file CSV di dati da analizzare**: si sceglie una colonna e il programma calcola sommatoria, produttoria, medie (aritmetica, geometrica, armonica, quadratica), varianza, deviazione standard, frequenze e altre operazioni. Disegna anche grafici: dispersione tra due colonne, rette di regressione (Y su X e X su Y, con covarianza e coefficiente di Bravais-Pearson), istogramma e grafico a torta.
2. **Caricare un file CSV per il calcolo delle probabilità**: legge una tabella di contingenza bivariata e mostra frequenze osservate, probabilità per cella, probabilità marginali di righe e colonne, valori attesi, chi quadro (χ²), moda e mediana marginali, statistiche per caratteristica.
3. **Visualizzare la lista delle funzioni** base e statistiche disponibili, con una breve descrizione di ciascuna.
4. **Usare singolarmente le funzioni**: un menu a tendina con 26 operazioni (potenza, radice, logaritmo, valore assoluto, medie, varianza, mediana, moda, percentile, gaussiana, covarianza, correlazione di Bravais-Pearson, chi quadrato, tabella dei valori attesi, distribuzione e probabilità marginale, ecc.), con inserimento manuale dei valori.
5. **Uscire dal software**, con un popup di conferma.

Nel footer della homepage ci sono i link alla repository e al profilo GitHub dell'autore.

### Contenuto della repository

Tutto si trova nella cartella `Modulo 5/`:

| Percorso | Descrizione |
|---|---|
| `Calcolatore di dati statistici.slnx` | Soluzione di Visual Studio |
| `Calcolatore di dati statistici/Calcolatore di dati statistici.cpp` | Punto di ingresso (`WinMain`): crea la finestra GLFW, carica i font, gestisce il passaggio tra le schermate e il popup di uscita |
| `Calcolatore di dati statistici/Homepage.cpp` / `.h` | Homepage con titolo, slogan, i 5 pulsanti principali e i crediti |
| `Calcolatore di dati statistici/OperazioniBase.cpp` / `.h` | Classe `OperazioniBase`: potenza, radice, logaritmo, sommatoria, produttoria, valore assoluto |
| `Calcolatore di dati statistici/OperazioniStatistiche.cpp` / `.h` | Classe `OperazioniStatistiche` (eredita da `OperazioniBase`): medie, varianza, deviazione standard, mediana, moda, frequenze, percentile, gaussiana, covarianza, regressione, chi quadrato, tabelle di probabilità e marginali |
| `Calcolatore di dati statistici/SchermataUploadDati.cpp` / `.h` | Schermata di caricamento del CSV di dati, con tabella dei risultati e grafici |
| `Calcolatore di dati statistici/SchermataUploadProbabilita.cpp` / `.h` | Schermata di caricamento della tabella di contingenza e calcolo delle probabilità |
| `Calcolatore di dati statistici/SchermataVisualizzaFunzioni.cpp` / `.h` | Elenco descrittivo delle operazioni disponibili |
| `Calcolatore di dati statistici/SchermataTestFunzioni.cpp` / `.h` | Schermata per provare una singola operazione inserendo i valori a mano |
| `Calcolatore di dati statistici/HeptaSlabFont.h` | Font Hepta Slab incorporato come array di byte |
| `Calcolatore di dati statistici/IconsFontAwesome7.h` | Definizioni delle icone Font Awesome 7 |
| `Calcolatore di dati statistici/Calcolatore di dati statistici.rc`, `Resource.h`, `.ico` | Risorse Windows (icona del programma) |
| `Calcolatore di dati statistici/framework.h`, `targetver.h`, `Calcolatore di dati statistici.h` | Header generati dal modello di progetto di Visual Studio |
| `Calcolatore di dati statistici/imgui/` | Sorgenti della libreria Dear ImGui (versione 1.92.7 WIP) e dei suoi backend |
| `Calcolatore di dati statistici/Resources/Fonts/` | Font Hepta Slab (vari pesi) e Font Awesome 7 |
| `Calcolatore di dati statistici/Resources/Icons/`, `Resources/Images/` | Logo del software in formato `.ico` e `.png` |
| `Calcolatore di dati statistici/*.vcxproj`, `*.vcxproj.filters`, `imgui.ini` | File di progetto di Visual Studio e impostazioni della finestra di ImGui |
| `Tabelle Excel CSV-XLSX template/` | File CSV/XLSX di esempio: dati sui fumatori e sui pazienti, tabelle degli esercizi 1 e 2 della simulazione e della verifica del modulo 5 |
| `Versione del software compilata/` | Eseguibile già compilato (`Calcolatore di dati statistici.exe`) con `glfw3.dll` e le DLL di runtime di debug di Visual C++ |

### Tecnologie

- C++20
- [Dear ImGui](https://github.com/ocornut/imgui) per l'interfaccia grafica
- GLFW e OpenGL 3 (backend di ImGui)
- API Win32 (`WinMain`, finestra di dialogo per scegliere i file)
- Font Hepta Slab e Font Awesome 7

### Requisiti

- Windows
- Per compilare: Visual Studio con il carico di lavoro "Sviluppo di applicazioni desktop con C++" (il progetto usa il set di strumenti `v145`)
- La libreria **GLFW**, che **non** è inclusa nella repository: deve essere installata e resa disponibile al progetto (header `GLFW/glfw3.h` e libreria da collegare)

### Come compilare ed eseguire

**Versione già compilata**

Apri `Modulo 5/Versione del software compilata/` ed esegui `Calcolatore di dati statistici.exe`. Le DLL necessarie sono nella stessa cartella.

**Da sorgente**

1. Apri `Modulo 5/Calcolatore di dati statistici.slnx` con Visual Studio.
2. Assicurati che GLFW sia configurato (percorsi di include e libreria).
3. Scegli la configurazione (es. `Debug` / `x64`) e avvia con **F5**.

Il programma carica il font delle icone da `Resources\Fonts\` con un percorso relativo, quindi va avviato con la cartella del progetto come directory di lavoro (è l'impostazione predefinita quando lo si avvia da Visual Studio). La cartella `Versione del software compilata/` non contiene `Resources\Fonts\`, quindi avviando l'eseguibile da lì le icone potrebbero non essere visualizzate.

### Formato dei file CSV

- **Analisi dei dati**: la prima riga contiene i nomi delle colonne, le righe successive i valori numerici. I campi sono separati da `;` e i decimali vanno scritti con il punto (es. `10.84`).
- **Calcolo delle probabilità**: tabella di contingenza con la prima cella vuota, le intestazioni delle colonne sulla prima riga e le intestazioni delle righe nella prima colonna, separate da `;` (es. `;Fumatore;Ex-fumatore;Non-Fumatore`).

Esempi pronti si trovano in `Modulo 5/Tabelle Excel CSV-XLSX template/`. Nota: `Esempio dati statistici pazienti.csv` usa la virgola come separatore e `Tabella esercizio 1 della verifica del modulo 5.csv` usa la virgola per i decimali, quindi questi due file non vengono letti correttamente così come sono.

### Autore

[MatteoMgr2008](https://github.com/MatteoMgr2008)

---

<a id="english"></a>
## 🇬🇧 English

### Description

A C++ program with a graphical interface, made for **Mathematics** class (Module 5, statistics and probability). The software is called **Calcolatore di dati statistici** ("Statistical data calculator") and its tagline is *"Analizza e calcola dati statistici in modo semplice, veloce e preciso!"* ("Analyze and calculate statistical data simply, quickly and accurately!").

It is a Windows desktop application built with [Dear ImGui](https://github.com/ocornut/imgui) (GLFW + OpenGL 3 backend). The interface is in Italian. From the homepage you can:

1. **Load a CSV data file to analyze**: you pick a column and the program computes the sum, product, means (arithmetic, geometric, harmonic, quadratic), variance, standard deviation, frequencies and other operations. It also draws charts: a scatter plot of two columns, regression lines (Y on X and X on Y, with covariance and the Bravais-Pearson coefficient), a histogram and a pie chart.
2. **Load a CSV file for probability calculations**: it reads a bivariate contingency table and shows observed frequencies, per-cell probabilities, marginal probabilities for rows and columns, expected values, chi-square (χ²), marginal mode and median, and per-feature statistics.
3. **View the list of available functions**, both basic and statistical, each with a short description.
4. **Use the functions one at a time**: a drop-down menu with 26 operations (power, root, logarithm, absolute value, means, variance, median, mode, percentile, Gaussian, covariance, Bravais-Pearson correlation, chi-square, expected values table, marginal distribution and probability, etc.), with values typed in by hand.
5. **Exit the software**, with a confirmation popup.

The homepage footer has links to the repository and to the author's GitHub profile.

### Repository contents

Everything is inside the `Modulo 5/` folder:

| Path | Description |
|---|---|
| `Calcolatore di dati statistici.slnx` | Visual Studio solution |
| `Calcolatore di dati statistici/Calcolatore di dati statistici.cpp` | Entry point (`WinMain`): creates the GLFW window, loads the fonts, switches between screens and handles the exit popup |
| `Calcolatore di dati statistici/Homepage.cpp` / `.h` | Homepage with title, tagline, the 5 main buttons and credits |
| `Calcolatore di dati statistici/OperazioniBase.cpp` / `.h` | `OperazioniBase` class: power, root, logarithm, sum, product, absolute value |
| `Calcolatore di dati statistici/OperazioniStatistiche.cpp` / `.h` | `OperazioniStatistiche` class (inherits from `OperazioniBase`): means, variance, standard deviation, median, mode, frequencies, percentile, Gaussian, covariance, regression, chi-square, probability and marginal tables |
| `Calcolatore di dati statistici/SchermataUploadDati.cpp` / `.h` | Screen for loading the data CSV, with the results table and charts |
| `Calcolatore di dati statistici/SchermataUploadProbabilita.cpp` / `.h` | Screen for loading the contingency table and computing probabilities |
| `Calcolatore di dati statistici/SchermataVisualizzaFunzioni.cpp` / `.h` | Descriptive list of the available operations |
| `Calcolatore di dati statistici/SchermataTestFunzioni.cpp` / `.h` | Screen for trying a single operation with manually entered values |
| `Calcolatore di dati statistici/HeptaSlabFont.h` | Hepta Slab font embedded as a byte array |
| `Calcolatore di dati statistici/IconsFontAwesome7.h` | Font Awesome 7 icon definitions |
| `Calcolatore di dati statistici/Calcolatore di dati statistici.rc`, `Resource.h`, `.ico` | Windows resources (program icon) |
| `Calcolatore di dati statistici/framework.h`, `targetver.h`, `Calcolatore di dati statistici.h` | Headers generated by the Visual Studio project template |
| `Calcolatore di dati statistici/imgui/` | Source of the Dear ImGui library (version 1.92.7 WIP) and its backends |
| `Calcolatore di dati statistici/Resources/Fonts/` | Hepta Slab fonts (several weights) and Font Awesome 7 |
| `Calcolatore di dati statistici/Resources/Icons/`, `Resources/Images/` | Software logo as `.ico` and `.png` |
| `Calcolatore di dati statistici/*.vcxproj`, `*.vcxproj.filters`, `imgui.ini` | Visual Studio project files and ImGui window settings |
| `Tabelle Excel CSV-XLSX template/` | Sample CSV/XLSX files: smokers and patients data, tables for exercises 1 and 2 of the Module 5 practice test and test |
| `Versione del software compilata/` | Prebuilt executable (`Calcolatore di dati statistici.exe`) with `glfw3.dll` and the Visual C++ debug runtime DLLs |

### Technologies

- C++20
- [Dear ImGui](https://github.com/ocornut/imgui) for the graphical interface
- GLFW and OpenGL 3 (ImGui backend)
- Win32 API (`WinMain`, file picker dialog)
- Hepta Slab and Font Awesome 7 fonts

### Requirements

- Windows
- To build: Visual Studio with the "Desktop development with C++" workload (the project uses the `v145` toolset)
- The **GLFW** library, which is **not** included in the repository: it has to be installed and made available to the project (the `GLFW/glfw3.h` header and the library to link)

### How to build and run

**Prebuilt version**

Open `Modulo 5/Versione del software compilata/` and run `Calcolatore di dati statistici.exe`. The required DLLs are in the same folder.

**From source**

1. Open `Modulo 5/Calcolatore di dati statistici.slnx` in Visual Studio.
2. Make sure GLFW is set up (include and library paths).
3. Pick a configuration (e.g. `Debug` / `x64`) and start with **F5**.

The program loads the icon font from `Resources\Fonts\` using a relative path, so it has to run with the project folder as its working directory (this is the default when you start it from Visual Studio). The `Versione del software compilata/` folder does not contain `Resources\Fonts\`, so the icons may not show up when you run the executable from there.

### CSV file format

- **Data analysis**: the first row holds the column names and the following rows hold numeric values. Fields are separated by `;` and decimals must use a dot (e.g. `10.84`).
- **Probability calculations**: a contingency table with an empty first cell, column headers on the first row and row headers in the first column, separated by `;` (e.g. `;Fumatore;Ex-fumatore;Non-Fumatore`).

Ready-made examples are in `Modulo 5/Tabelle Excel CSV-XLSX template/`. Note: `Esempio dati statistici pazienti.csv` uses commas as separators and `Tabella esercizio 1 della verifica del modulo 5.csv` uses commas for decimals, so these two files are not read correctly as they are.

### Author

[MatteoMgr2008](https://github.com/MatteoMgr2008)
