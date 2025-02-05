# PROJEKT SYSTEMY OPERACYJNE 2024/2025

## CHANGELOG

### 05.02.2025

- dodane testy

### 04.02.2025

- dodane UI w zewnętrznym oknie (terminalu, korzystając z xterm)

### 03.02.2025

- lekkie przebudowanie struktury katalogowej
- zmiana w planowanym UI, wykorzystanie ncurses, uruchamianie drugiego terminalu z UI (wstępnie, jeszcze niefunkcjonalne)
- planowanie testów

### 29.01.2025

- dodana funkcjonalność wszystkich procesów
- dodana synchronizacja semaforami
- komunikacja pomiędzy menedżerem, a wszystkimi procesami
- zostaje UI

### 26.01.2025

- kolejka komunikatów działa

### 12.01.2024

- zmiana działania listy pidów (używanie uthash.h)

### 27.12.2024

- koniec przestoju
- przebudowałem lekko program
- dodane wstępnie kolejki komunikatów + wstepna obsługa sygnałów przez głowny proces

### 24.11.2024

- wstępna szczegółowa struktura plików źródłowych
- proces dyrektora(główny), proces magazynu(dziecko), dzielona pamięć
- działa współdzielenie pamięci dyrektor-magazyn i sterowanie magazynem z dyrektora (narazie tylko bezpośrednie zamknięcie proesu magazynu)

### 21.11.2024

- utworzone repozytorium
- do testów przygotowany WSL Ubuntu
- konfiguracja vscode
- utworzenie CHANGELOG.md, opis.md, struktura katalogów
- skonfigurowana (wstępnie) kompilacja + debugging
