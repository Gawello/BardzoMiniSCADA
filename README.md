# Bardzo Mini SCADA (Qt6/C++17)

> Minimalny projekt „mini-SCADA”: dziedziczenie, polimorfizm, szablony, przeładowania metod oraz integracja z Qt (Qt Widgets + Qt Charts).
> Aplikacja generuje co sekundę przykładowe dane (ciśnienie, temperatura, stan zaworu), wyświetla **trend** na wykresie i **ostatnie wartości** w panelu tekstowym.

---

## Spis treści

* [Wymagania](#wymagania)
* [Budowanie i uruchamianie](#budowanie-i-uruchamianie)
* [Skrót działania](#skrót-działania)
* [Architektura](#architektura)
* [Format danych](#format-danych)
* [Dokumentacja klas i metod](#dokumentacja-klas-i-metod)

  * [`Probka`](#probka)
  * [`Dane<T>` – klasa bazowa (abstrakcyjna)](#danet--klasa-bazowa-abstrakcyjna)
  * [`ZbiorDanych<T>` – magazyn próbek](#zbiordanycht--magazyn-próbek)
  * [`WykresLiniowy<T>` – wizualizacja przebiegów](#wykresliniowyt--wizualizacja-przebiegów)
  * [`Wyswietlacz<T>` – panel ostatnich wartości](#wyswietlaczyt--panel-ostatnich-wartości)
  * [`OknoGlowne` – spoiwo aplikacji](#oknoglowne--spoiwo-aplikacji)
  * [`main.cpp`](#maincpp)
* [Konfiguracja i parametry](#konfiguracja-i-parametry)

---

## Wymagania

* **Qt 6** z modułami: `Core`, `Gui`, `Widgets`, **`Charts`** (np. 6.9.x).
* Kompilator C++ z obsługą **C++17**.
* CMake ≥ 3.16.

> Uwaga (Linux/Arch): moduł **Qt6Charts** musi być w tej samej wersji co `qt6-base`. Jeżeli budowanie krzyczy o braku `Qt6::Charts`, doinstaluj pakiet „charts” i/lub wyrównaj wersje Qt6.

---

## Budowanie i uruchamianie

```bash
# w katalogu projektu (z plikiem CMakeLists.txt)
mkdir build && cd build
cmake ..
cmake --build . -j
./bardzo_mini_SCADA_wrzesien
```

Minimalny `CMakeLists.txt` (używany w tym repo) wymusza Qt6 i linkuje `Qt6::Charts`.

---

## Skrót działania

* Co **1 sekundę** generator (slot w `OknoGlowne`) tworzy 3 próbki:

  * `Ciśnienie [bar]` \~ 10 ± losowy szum,
  * `Temperatura [°C]` \~ 25 ± lekki szum,
  * `Zawór (0/1)` – przełącza się co sekundę.
* Każda próbka jest rozsyłana do wszystkich obiektów, które **dziedziczą** po `Dane<Probka>` (polimorfizm):

  * **`ZbiorDanych<Probka>`** – przechowuje okno czasowe próbek,
  * **`WykresLiniowy<Probka>`** – rysuje serie na wykresie (oś X to czas),
  * **`Wyswietlacz<Probka>`** – pokazuje ostatnie wartości w etykietach.
* Oś X to **QDateTimeAxis** (HH\:mm\:ss), zakres okna: **ostatnie 10 minut**.
* Oś Y skaluje się automatycznie do min/max napływających wartości.

---

## Architektura

```
             +-----------------+      dodaj(T)      +----------------------+
 QTimer ---> |  OknoGlowne     |  ───────────────>  |  vector<Dane<Probka>*>|
 (1 Hz)      |  (generator)    |                    +----------+-----------+
             +-----------------+                               |
                       |                                       |
                       |                                     (polimorfizm)
           +-----------+-----------+                           |
           |           |           |                           v
           v           v           v           +-------------------+  +---------------------+  +----------------------+
     ZbiorDanych    WykresLiniowy  Wyswietlacz  | ZbiorDanych<Probka>|  | WykresLiniowy<Probka>|  | Wyswietlacz<Probka> |
 (magazyn okienkowy)    (wykres)     (labelki)  +-------------------+  +---------------------+  +----------------------+
```

Wszystkie „odbiorniki” **dziedziczą** po wspólnej, szablonowej bazie `Dane<T>` i są używane przez wskaźnik/referencję do klasy bazowej (polimorfizm runtime).

---

## Format danych

```cpp
struct Probka {
    double t;        // znacznik czasu w milisekundach od epoki (ms since epoch)
    double wartosc;  // mierzona/pokazywana wartość
    QString etykieta;// nazwa sygnału (np. "Temperatura [°C]")
};
```

> `operator<<` jest przeładowany dla `Probka`, by ładnie logować do `std::ostream`.

---

## Dokumentacja klas i metod

### `Probka`

* **Plik**: `Typy.hpp`
* **Cel**: prosty typ opisujący jedną próbkę danych.
* **Pola**:

  * `double t` – czas (ms od epoki; kompatybilne z `QDateTime::fromMSecsSinceEpoch`),
  * `double wartosc`,
  * `QString etykieta`.
* **Operatory**:

  * `friend std::ostream& operator<<(std::ostream&, const Probka&)` – do logów/konsoli.

---

### `Dane<T>` – klasa bazowa (abstrakcyjna)

* **Plik**: `Dane.hpp`
* **Cel**: wspólny interfejs dla wszystkich obiektów, które **przyjmują próbki** typu `T`.
  Pozwala używać pochodnych polimorficznie.
* **Najważniejsze metody (czysto wirtualne)**:

  ```cpp
  virtual std::string nazwa() const = 0;
  virtual void dodaj(const T& v) = 0;       // podstawowa ścieżka dodania
  virtual void dodaj(T&& v);                 // przeładowanie (domyślnie przekierowuje do l-wartości)
  virtual std::size_t rozmiar() const = 0;   // ile elementów obiekt przechowuje/obsłużył
  virtual std::optional<T> ostatnia() const = 0; // ostatnia próbka (jeśli sensownie dostępna)
  ```
* **Cechy**:

  * **Szablon**: `T` może być dowolną strukturą (tu używamy `Probka`).
  * **Przeładowanie**: `dodaj(T&&)` pokazuje przenoszenie; pochodne mogą je nadpisać.

---

### `ZbiorDanych<T>` – magazyn próbek

* **Plik**: `ZbiorDanych.hpp`
* **Dziedziczy**: `Dane<T>`
* **Cel**: okienkowe przechowywanie próbek w `std::vector` o stałej pojemności (domyślnie 600).
* **Konstruktor**:

  ```cpp
  explicit ZbiorDanych(std::string nazwa, std::size_t capacity = 600);
  ```
* **Metody**:

  * `std::string nazwa() const` – identyfikacja w GUI/logach,
  * `void dodaj(const T& v)` – dodaje próbkę; gdy bufor pełny, usuwa najstarszą,
  * `void dodaj(T&& v)` – wersja r-wartości,
  * `std::size_t rozmiar() const` – liczba elementów w buforze,
  * `std::optional<T> ostatnia() const` – zwraca ostatnią próbkę (jeśli dostępna),
  * `const std::vector<T>& dane() const` – bezpośredni dostęp do bufora (tylko do odczytu).
* **Wewnętrznie**:

  * `std::mutex` + `std::scoped_lock` – prosta ochrona na wypadek przyszłego wielowątkowego źródła.

---

### `WykresLiniowy<T>` – wizualizacja przebiegów

* **Plik**: `WykresLiniowy.hpp`
* **Dziedziczy**: `Dane<T>`
* **Cel**: rysowanie **linii trendu** dla wielu sygnałów (po jednej serii na unikalną `etykieta`).
* **Wersja typu**: W tej implementacji zakładamy `T == Probka` (są `static_assert`).
* **Konstruktor**:

  ```cpp
  explicit WykresLiniowy(const std::string& nazwa, QWidget* parent = nullptr);
  ```
* **Interfejs**:

  * `std::string nazwa() const` – nazwa,
  * `QChartView* widzet() const` – zwraca wskaźnik na widżet wykresu (do wstawienia w layout),
  * `void dodaj(const T& v)` – dodaje punkt do właściwej serii;

    * jeśli seria `etykieta` nie istnieje, jest tworzona „leniwe” (`QLineSeries`).
  * `std::size_t rozmiar() const` – liczba dodanych punktów,
  * `std::optional<T> ostatnia() const` – `std::nullopt` (wykres nie trzyma ostatniej próbki; to rola magazynu).
* **Szczegóły osi**:

  * **Oś X** – `QDateTimeAxis`, format `HH:mm:ss`, okno **ostatnie 10 min**,
  * **Oś Y** – `QValueAxis` z prostym **auto-skalowaniem** min/max (z marginesem 5%).

---

### `Wyswietlacz<T>` – panel ostatnich wartości

* **Plik**: `Wyswietlacz.hpp`
* **Dziedziczy**: `Dane<T>`
* **Cel**: prosta prezentacja **ostatniej** wartości dla każdego sygnału (`etykieta`) w `QLabel`.
* **Konstruktor**:

  ```cpp
  explicit Wyswietlacz(const std::string& nazwa, QWidget* parent = nullptr);
  ```
* **Interfejs**:

  * `std::string nazwa() const`,
  * `QWidget* widzet() const` – gotowy panel z etykietami,
  * `void dodaj(const T& v)` – aktualizuje (lub tworzy) etykietę dla `v.etykieta`,
  * `std::size_t rozmiar() const` – liczba przetworzonych próbek,
  * `std::optional<T> ostatnia() const` – ostatnia próbka, jeśli była dodana.
* **Wewnętrznie**:

  * `std::map<QString, QLabel*>` – jedna etykieta na sygnał.

---

### `OknoGlowne` – spoiwo aplikacji

* **Pliki**: `OknoGlowne.hpp/.cpp`
* **Dziedziczy**: `QMainWindow`
* **Cel**: tworzy obiekty pochodne `Dane<Probka>`, buduje GUI, uruchamia **generator** z `QTimer`.
* **Pola**:

  ```cpp
  std::vector<Dane<Probka>*> odbiorcy_;         // polimorficzna lista odbiorców
  std::unique_ptr<ZbiorDanych<Probka>> magazyn_;
  std::unique_ptr<WykresLiniowy<Probka>> wykres_;
  std::unique_ptr<Wyswietlacz<Probka>> wyswietlacz_;
  QTimer timer_; // 1 Hz
  ```
* **Kluczowe miejsce**: `generujPrzykladoweDane()` – co sekundę:

  1. liczy `t = QDateTime::currentMSecsSinceEpoch()` (ms),
  2. generuje `cisnienie`, `temperatura`, `zawor` (ten ostatni przełącza 0/1),
  3. **rozsyła** `dodaj(...)` każdą próbkę do wszystkich `odbiorcy_`,
  4. odświeża statusbar (ile próbek w magazynie).

---

### `main.cpp`

* Tworzy `QApplication`, instancję `OknoGlowne`, ustawia rozmiar okna i woła `app.exec()`.

---

## Konfiguracja i parametry

* **Częstotliwość generacji**: w `OknoGlowne.cpp` (`timer_.start(1000);`) – zmień na inny interwał ms.
* **Pojemność magazynu**: `ZbiorDanych<Probka>("Magazyn", /*capacity*/600)`
* **Okno czasu na osi X**: w `WykresLiniowy::dodaj` (`axisX_->setRange(now.addSecs(-600), now);`)
* **Format czasu**: `axisX_->setFormat("HH:mm:ss");` – np. `"mm:ss"` lub `"HH:mm"`.

---

## Pogląd aplikacji

<img width="1265" height="766" alt="image" src="https://github.com/user-attachments/assets/0a1961f5-af57-44f3-91e1-c5248381dde7" />
