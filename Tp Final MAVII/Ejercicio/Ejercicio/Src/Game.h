#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include "Avatar.h"
#include <list>

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World* phyWorld;
	SFMLRenderer* debugRender;

	// Tiempo de frame
	float frameTime;
	int fps;

	b2Body* controlBody;
	b2Body* controlCannon;

	b2Body* barrel1;
	b2Body* barrel2;
	b2Body* barrel3;
	b2Body* barrel4;

	int allBarrels;

	String msg; //MENSAJE PRUEBA

	float bodyY;
	float densidad;
	float friccion;
	float restitucion;

	float level;

	int ragdolls;

	Font font;
	Text counter;

	Text play;

	Text controles;

	//Avatar one;

	//b2Body* oneb;
	
	Texture BarrilTextura;
	Texture CanonTextura;

	Avatar* CanonAvatar;
	Avatar* BarrrilAvatar1;
	Avatar* BarrrilAvatar2;
	Avatar* BarrrilAvatar3;
	Avatar* BarrrilAvatar4;

public:
	//Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void CheckCollitions();
	void CreateEnemy(int x, int y);
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
	void CheckPositions();
	void PreGame();
};
