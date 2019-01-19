#include <SFML/Graphics.hpp>
#include "Audio.h"
#include <iostream>
#include <cmath>
#include <string>

using namespace sf;

const double PI = 3.141592653589793238463;

AudioFile<double> audioFile;
double specter[15000][2];
float freqs[] = { 65, 69, 74, 78, 83, 87, 92.5, 98, 104, 110, 117, 123,
				  131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
				  262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
				  523, 554, 587, 622, 659, 698.5, 740, 784, 831, 880, 932, 988 };

int main() 
{
	RenderWindow window(VideoMode(400, 400), "Notes recognition");
	
	// Background
	Texture background;
	if (!background.loadFromFile("bg.png"))
		std::cout << "Erreur lors du chargement du background" << std::endl;
	Sprite sBackground;
	sBackground.setTexture(background);
	
	// Textes
	Font font;
	if (!font.loadFromFile("INVASION2000.ttf"))
		std::cout << "Erreur lors du chargement de la police" << std::endl;
	
	Text tfreq;
	tfreq.setFont(font);
	tfreq.setCharacterSize(38);
	tfreq.setFillColor(Color(62, 98, 124));
	tfreq.setPosition(sf::Vector2f(206.0f, 166.0f));
	Text freqF;
	freqF.setFont(font);
	freqF.setCharacterSize(34);
	freqF.setFillColor(Color(62, 98, 124));
	freqF.setPosition(sf::Vector2f(178.0f, 254.0f));

	// Son
	audioFile.load("noteswav/revoice.wav");

	int channel = 0;
	double c = 0;
	double F = 15000;
	int N = audioFile.getNumSamplesPerChannel();

	while (window.isOpen()) 
	{
		Event event;
		
		while (window.pollEvent(event)) 
		{
			if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) 
			{
				window.close();
				break;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::F)) 
		{
			// Transformée de Fourier discrète
			for (int i = 0; i < N; i++)
			{
				c += audioFile.samples[channel][i];
			}
			c /= N;
			
            for (int f = 0; f < F; f++) 
			{
				double a = 0;
				double b = 0;
				for (int n = 0; n < N; n++) 
				{
					a += audioFile.samples[channel][n] * cos((f + 1) * n * 2 * PI / (double)N);
					b += audioFile.samples[channel][n] * sin((f + 1) * n * 2 * PI / (double)N);
				}
				a *= 2.0 / (double)N;
				b *= 2.0 / (double)N;
				specter[f][0] = (f + 1) / (double)N * audioFile.getSampleRate();
				specter[f][1] = log10(abs(a / 2 + b / 2 + c));
				
				if (specter[f][0] > 500)
					break;
			}
			
            // Fréquence de la fondamentale 
			double ampMax = specter[0][1];
			double freq = specter[0][0];
			
			for (int c = 0; c < F; c++) 
			{
				if (specter[c][0] < 500) 
				{
					if (specter[c][1] > ampMax) 
					{
						ampMax = specter[c][1];
						freq = specter[c][0];
					}
				}
				else
					break;
			}

			int difMin = abs(int(freq) - freqs[0]);
			int freqFinal = freq;
			
			for (int c = 1; c < 48; c++) 
			{
				int difAbs = abs(int(freq) - freqs[c]);
				if (difAbs < difMin) 
				{
					difMin = difAbs;
					freqFinal = freqs[c];
				}
			}
			std::cout << freqFinal << std::endl;

			if (freqFinal == 65 || freqFinal == 131 || freqFinal == 262)
			{
				tfreq.setString("Do");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 69 || freqFinal == 139 || freqFinal == 277)
			{
				tfreq.setString("Do diese");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 74 || freqFinal == 147 || freqFinal == 294)
			{
				tfreq.setString("Re");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 78 || freqFinal == 156 || freqFinal == 311)
			{
				tfreq.setString("Re diese");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 83 || freqFinal == 165 || freqFinal == 330)
			{
				tfreq.setString("Mi");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 87 || freqFinal == 175 || freqFinal == 349)
			{
				tfreq.setString("Fa");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 92.5 || freqFinal == 185 || freqFinal == 370)
			{
				tfreq.setString("Fa diese");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 98 || freqFinal == 196 || freqFinal == 392) 
			{
				tfreq.setString("Sol");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 104 || freqFinal == 208 || freqFinal == 415)
			{
				tfreq.setString("Sol diese");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 110 || freqFinal == 220 || freqFinal == 440)
			{
				tfreq.setString("La");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 117 || freqFinal == 233 || freqFinal == 466)
			{
				tfreq.setString("La diese");
				freqF.setString(std::to_string(freqFinal));
			}
			else if (freqFinal == 123 || freqFinal == 247 || freqFinal == 494)
			{
				tfreq.setString("Si");
				freqF.setString(std::to_string(freqFinal));
			}
			else
			{
				tfreq.setString("Dunno.");
				freqF.setString("?");
			}
		}

		window.clear();
		
		window.draw(sBackground);
		window.draw(tfreq);
		window.draw(freqF);

		window.display();
	}

	return 0;
}
