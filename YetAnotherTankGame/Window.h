#pragma once
#include <SFML/Graphics.hpp>


typedef sf::RenderWindow CWindow;

void InitializeWindow();
void DestroyWindow();

CWindow &Window();
