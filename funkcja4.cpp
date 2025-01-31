#include <iostream>
#include <string>
#include <unordered_map>

std::string zamienPolskieZnaki(const std::string& tekst) {
    // Mapowanie polskich znaków na zwykłe
    std::unordered_map<std::string, char> mapaPolskieZnaki = {
        {"ą", 'a'}, {"ć", 'c'}, {"ę", 'e'}, {"ł", 'l'}, {"ń", 'n'},
        {"ó", 'o'}, {"ś", 's'}, {"ź", 'z'}, {"ż", 'z'},
        {"Ą", 'A'}, {"Ć", 'C'}, {"Ę", 'E'}, {"Ł", 'L'}, {"Ń", 'N'},
        {"Ó", 'O'}, {"Ś", 'S'}, {"Ź", 'Z'}, {"Ż", 'Z'}
    };

    std::string wynik = tekst;

    // Wyszukujemy w tekście polskie znaki i zamieniamy je na odpowiedniki
    for (const auto& para : mapaPolskieZnaki) {
        size_t pos = 0;
        // Znajdujemy wszystkie wystąpienia polskiego znaku
        while ((pos = wynik.find(para.first, pos)) != std::string::npos) {
            wynik.replace(pos, para.first.length(), 1, para.second);
            pos += 1; // Po zamianie przechodzimy do kolejnego znaku
        }
    }

    return wynik;
}

