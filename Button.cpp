#include "../DFRobot_RGBLCD.h"
#include <wiringPi.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include "funkcja2.cpp"
#include "funkcja4.cpp"
#include <sstream>
#include <curl/curl.h>
#include <locale>


int full = 0;
int pressLeft = 0;
int pressRight = 0;
int pressUp = 0;
int pressDown = 0;
int pressSelect = 0;
std::string url = "https://rozklady.mpk.krakow.pl/?lang=PL&linia=";
const char* przystanki_tab;
tablica przystanki;



// Global LCD object
DFRobot_RGBLCD lcd(16, 2);

// WYSWIETL
void handleRightButton() {
    pressRight = 1;
}

//RESET
void handleSelectButton() {
    pressSelect = 1;
}

//ZWIEKSZ
void handleUpButton() {
    pressUp = 1;
}

void handleDownButton() {
    pressDown = 1;
}

void handleLeftButton() {

    pressLeft = 1;
}

int main() {
    // Setup WiringPi
    wiringPiSetup();
    std::setlocale(LC_ALL, "pl_PL.UTF-8");

    // Initialize LCD
    lcd.init();

    // Configure pin modes
    pinMode(27, INPUT);   // RIGHT button
    pinMode(0, INPUT);    // SELECT button
    pinMode(1, INPUT);    // UP button
    pinMode(24, INPUT);   // DOWN button
    pinMode(28, INPUT);   // LEFT button

    // Setup interrupt handlers
    // Using RISING mode (when button is pressed)
    wiringPiISR(27, INT_EDGE_RISING, &handleSelectButton);
    wiringPiISR(0, INT_EDGE_RISING, &handleUpButton);
    wiringPiISR(1, INT_EDGE_RISING, &handleDownButton);
    wiringPiISR(24, INT_EDGE_RISING, &handleLeftButton);
    wiringPiISR(28, INT_EDGE_RISING, &handleRightButton);

    // Initial display message
    lcd.setCursor(0, 0);
    lcd.print("Device Ready");
    delay(100);

    lcd.clear();
    lcd.print("Wybor Linii: ");
    int wynik = 0;
    int liczba2 = 0;
    int liczba = 0;
    int dziesiatki = 0;
    int setki = 0;
    int jednosci = 0;
    int miejsce = 0;
    std::vector<int> liczby;

    // Keep the program running
    while(1) {

	if(miejsce > 2){
		miejsce = 0;
	}

	if(pressDown){
		if(full == 1){
			delay(300);
			lcd.clear();
			lcd.print("Linia nr: ");
			lcd.setCursor(10,0);
			lcd.print(wynik);

			if(liczba > 0){
				liczba--;
			}
			pressDown = 0;
			lcd.setCursor(0,1);
			przystanki_tab = przystanki.przystanek[liczba].c_str();
			lcd.print(przystanki_tab);
		}
	}

	if(pressRight){
		if(full == 0){
			delay(200);
			lcd.setCursor(0,0);
			lcd.print("Wybor Linii: ");
			lcd.setCursor(miejsce,1);
			lcd.print((liczba));
			delay(200);
			liczba2 = liczba;
			liczba++;
			if(liczba > 9){
				liczba = 0;
			}
			pressRight = 0;
		} else{
			delay(300);
			lcd.clear();
			lcd.print("Linia nr: ");
                	lcd.setCursor(10,0);
                	lcd.print(wynik);
			pressRight = 0;
			std::cout << liczba << std::endl;
			lcd.setCursor(0,1);
			przystanki.przystanek[liczba] = zamienPolskieZnaki(przystanki.przystanek[liczba]);
			std::cout << przystanki.przystanek[liczba] << std::endl;
			lcd.print(przystanki.przystanek[liczba].c_str());

			while(1){
				for (int positionCounter = 0; positionCounter < przystanki.przystanek[liczba].size(); positionCounter++) {
			        	lcd.scrollDisplayLeft();
					delay(400);
				}
				if(pressRight == 1){
					break;
				}
				if(pressLeft){
					lcd.print("RESET");
					delay(200);
					liczba = 0;
					miejsce = 0;
					lcd.clear();
					pressLeft = 0;
					full = 0;
					break;
				}


			}
			liczba++;

		}
	}




	if(pressSelect){
		delay(200);
		if(full == 0){
			liczby.push_back(liczba2);
			std::cout<< (liczba2) << std::endl;
			miejsce +=1;
		}
		pressSelect = 0;
	}


	if(pressLeft){
		lcd.print("RESET");
		delay(200);
		liczba = 0;
		miejsce = 0;
		lcd.clear();
		liczby.clear();
		pressLeft = 0;
		full = 0;
	}


	if(liczby.size() == 3){
		full = 1;
		liczba = 0;
		wynik = 100*liczby[0] + 10*liczby[1] + 1*liczby[2];
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Linia nr: ");
		lcd.setCursor(10,0);
		lcd.print(wynik);
		liczby.clear();
		std::string wy = std::to_string(wynik);
		std::cout << url+wy << std::endl;
		przystanki = pobierz(url+wy);

    		for(int i = 0;i<przystanki.dlugosc;i++){
        		std::cout << przystanki.przystanek[i] << std::endl;
    		};
    		std::cout << std::endl << przystanki.error << std::endl;

		lcd.setCursor(0,1);
		lcd.print("Linia wybrana");

	};
}

    return 0;
}
