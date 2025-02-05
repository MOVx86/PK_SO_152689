# PROJEKT SYSTEMY OPERACYJNE 2024/2025

## PK INFORMATYKA NST - TYMON GONTARZ

### TEMAT - 3 - FABRYKA

<div style="text-align: justify">
Fabryka posiada 2 stanowiska produkcyjne A i B. Na każdym stanowisku składane są wyroby z podzespołów X, Y i Z.
Podzespoły przechowywane są w magazynie o pojemności M jednostek. Podzespół X zajmuje jedną jednostkę magazynową, podzespół Y dwie, a podzespół Z trzy jednostki.
Podzespoły pobierane są z magazynu, przenoszone na stanowisko produkcyjne i montowane. Z podzespołów X, Y i Z po ich połączeniu powstaje jeden produkt, po czym pobierane są następne podzespoły z magazynu. Jednocześnie trwają dostawy podzespołów do fabryki.
Podzespoły pochodzą z 3 niezależnych źródeł i dostarczane są w nieokreślonych momentach czasowych. Fabryka przyjmuje do magazynu maksymalnie dużo podzespołów dla zachowania płynności produkcji.
</div>

<div style="text-align: justify">
Magazyn kończy pracę po otrzymaniu polecenia_1 od dyrektora. Fabryka kończy pracę po otrzymaniu polecenia_2 od dyrektora.
Fabryka i magazyn kończą pracę jednocześnie po otrzymaniu polecenia_3 od dyrektora – aktualny stan magazynu zapisany w pliku, po ponownym uruchomieniu stan magazynu jest odtwarzany z pliku.
Fabryka i magazyn kończą pracę jednocześnie po otrzymaniu polecenia_4 od dyrektora – stan magazynu nie jest zapamiętany, po ponownym uruchomieniu magazyn jest pusty.
</div>

<div style="text-align: justify">
Napisz program dla procesów dyrektor oraz dostawca i monter reprezentujących odpowiednio: dostawców produktów X, Y i Z oraz pracowników na stanowiskach A i B.
</div>

Link GitHub: [https://github.com/MOVx86/PK_SO_152689]

## WYKONYWANE TESTY

### TEST 1 - poprawne operowanie na dynamicznej liście PID

<div style="text-align: justify">
Program testowy deklaruje i inicjuje dynamiczną listę PID, następnie weryfikuje dane początkowe w liście.
Następnie do listy jest dodawany nowy PID, ponownie weryfikowana jest poprawność danych zawartych w liście.
Na koniec lista zostaje uwolniona/zresetowana, ponownie weryfikowana jest poprawność danych zawartych w liście.
Jeżeli wszystkie etapy przeszły poprawnie, TEST 1 zakończył się sukcesem.
</div>

### TEST 2 - poprawne działanie funkcji odpowiedzialnych za interfejs

<div style="text-align: justify">
Program testowy symuluje wypisanie listy PID do pliku tekstowego, następnie deklaruje i inicjuje strukturę listy PID’ów i czyta plik tekstowy, aby wypełnić listę PID’ów przykładowymi danymi. W trakcie tych operacji, weryfikowane jest czy plik tekstowy został poprawnie otwarty, czy dane zostały poprawnie zapisane i odczytane.
Następnie inicjowane jest ncurses, tworzone jest testowe okno ncurses. Weryfikowane jest, czy okno zostało poprawnie utworzone.
Kolejnym krokiem jest wprowadzenie danych do okna ncurses czytając PID’y z listy PID’ów. Przy każdym wprowadzeniu danych do okna weryfikowane jest, czy operacja się powiodła.
Następnie okno jest zawierane w ramce. Weryfikowane jest, czy operacja box() się powiodła.
Na koniec usuwane jest okno i kończona jest praca ncurses.
Jeżeli wszystkie etapy przeszły poprawnie, TEST 2 zakończył się sukcesem.
</div>

### TEST 3 - poprawna obsługa sygnałów

<div style="text-align: justify">
Program przypisuje funkcję odpowiedzialną za obsługę sygnałów do przykładowego sygnału (jeden z sygnałów obsługiwanych przez główny program). Następnie do programu wysyłany jest sygnał, który ma być obsłużony.
Po przyjęciu sygnału, weryfikowana jest zawartość danych, które mają być zmodyfikowane na podstawie otrzymanego sygnału.
Jeżeli wszystkie etapy przeszły poprawnie, TEST 3 zakończył się sukcesem.
</div>

### TEST 4 (dodatkowy) - stress-test, uruchomienie głównego programu po skompilowaniu przy wyłączonych funkcjach sleep()

<div style="text-align: justify">
Główny program wykonawczy jest skompilowany ze zmienną preprocesora USE_SLEEP = 0, wyłączając wszystkie funkcje sleep() w programie. Sprawdzana jest stabilność programu i synchronizacji.
Test wykonywany jest dodatkowo, poza głównym poleceniem testowania i kompilacji (oddzielny plik wykonawczy nosleep_fabryka).
</div>