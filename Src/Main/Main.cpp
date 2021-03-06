#include "Game.h"
#include <Windows.h>

//El código en común
void playGame()
{
	Game* Juego = new Game("InitAplication.json");
	Juego->start();
	Juego->run();
	delete Juego;
}

//Versión para Debug (con consola)
int main(int argc, char* argv[])
{
	playGame();
	return 0;
}

//Versión para Release (solo ventana)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	playGame();
	return 0;
}




