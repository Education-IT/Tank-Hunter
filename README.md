![Tank-hunter the game](https://raw.githubusercontent.com/Education-IT/Tank-Hunter/main/banner.png)
## Projekt zaliczeniowy na przedmiot - ***Grafika komputerowa*** - **UAM**

> **Zrealizowano w piątym semestrze studiów informatycznych.**

 Tank-Hunter: jest to trójwymiarowa scena, stworzona za pomocą języka C++ i biblioteki grafiki czasu rzeczywistego: OpenGl. Scena ta, przedstawia strefę działań wojennych w której to możemy wcielić się w rolę żołnierza, wyposażonego w latarkę oraz RPG7. Zaimplementowane zostało poruszanie jak i rozglądanie się. Z założenia - projekt ma przypominać swoją budową, gry z gatunku FPS. Do utworzenia projektu zostały wykorzystane darmowe pliki ```.obj / .png / .jpg```  


![C++](https://img.shields.io/badge/C++-00599C.svg?style=for-the-badge&logo=C++&logoColor=white) ![OpenGL](https://img.shields.io/badge/OpenGL-5586A4.svg?style=for-the-badge&logo=OpenGL&logoColor=white)  [ ![enter image description here](https://img.shields.io/badge/website-000000?style=for-the-badge&logo=About.me&logoColor=white)](https://education-it.pl/)


## Projekt zawiera:


Menu główne w którym możemy dokonać kilku zmian w ustawieniach - wpływających na rozgrywkę:
* Włączenie / wyłączenie mgły. (szary kolor)
* Noc / zachód słońca  -> W zależności od wyboru, czeka nas inny: skybox, koloru światła kierunkowego i jego pozycja.

* Latarka - Spotlight (białe światło) - możliwość włączenia/wyłączenia za pomocą LPM. Animacja wyjmowania/chowania/chodzenia. 
* RPG7 - możliwość celowania za pomocą PPM. Możliwość przeładowania za pomocą LPM. Animacja wyjmowania/chowania/chodzenia.
 
* Zaimplementowane poruszanie się i rozglądanie - zarówna za pomocą klawiatury i myszki, jak i za pomocą gamepada. Po wyborze gry za pomocą PADa, następuje funkcja sprawdzająca, czy na pewno został od podłączony - jeśli nie -> przełączamy się na tryb klawiatury i myszki. 

* Animacja skoku

*  Dodatkowe obiekty w ruchu -> Myśliwce latające nad naszą mapą.

* Animacje powstały na skutek wymyślonych i zaimplementowanych prze zemnie funckji które działając na zmiennych globalnych i czasie - zmieniają odpowiednie macierze translacji i rotacji.
 

