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
	SFMLRenderer* debugRender2;


	// Tiempo de frame
	float frameTime;
	int fps;

	b2Body* controlBody;
	b2Body* controlCannon;

	b2Body* barrel1;
	b2Body* barrel2;
	b2Body* barrel3;
	b2Body* barrel4;

	b2Body* orb;
	b2Body* orb1;
	b2Body* orb2;
	b2Body* orb3;
	b2Body* orb4;
	b2Body* orb5;

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
	
	//AVATARES Y TEXTURAS

	Texture BarrilTextura;
	Texture CanonTextura;

	Avatar* CanonAvatar;
	Avatar* BarrrilAvatar1;
	Avatar* BarrrilAvatar2;
	Avatar* BarrrilAvatar3;
	Avatar* BarrrilAvatar4;


	Texture ObsTextura;
	
	Avatar* ObsAvatar;
	Avatar* ObsAvatar1;
	Avatar* ObsAvatar2;
	Avatar* ObsAvatar3;
	Avatar* ObsAvatar4;
	Avatar* ObsAvatar5;
	Avatar* ObsAvatar6;
	Avatar* ObsAvatar7;
	Avatar* ObsAvatar8;
	Avatar* ObsAvatar9;
	Avatar* ObsAvatar10;


	int shot;
	Texture BoneTextura;
	Avatar* bone1;
	Avatar* bone2;
	Avatar* bone3;
	Avatar* bone4;

	Texture HeadTextura;
	Avatar* headAvatar;
	Texture BodyTextura;
	Avatar* bodyAvatar;


	Texture WallTextura;
	Texture WallRotTextura;

	Avatar* WallAvatar1;
	Avatar* WallAvatar2;
	Avatar* WallAvatar3;
	Avatar* WallAvatar4;


	Texture BallTextura;

	Avatar* OrbAvatar;
	Avatar* OrbAvatar1;
	Avatar* OrbAvatar2;
	Avatar* OrbAvatar3;
	Avatar* OrbAvatar4;
	Avatar* OrbAvatar5;
	Avatar* CircleAvatar1;
	Avatar* CircleAvatar2;


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
