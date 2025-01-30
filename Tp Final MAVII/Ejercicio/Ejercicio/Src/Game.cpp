#include "Game.h"
#include "Box2DHelper.h"
#include <print>
#include <iostream>
#include <string>


// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara

	level = -1; //NIVEL EN EL QUE EMPIEZA
	ragdolls = 0; //CONTADOR DE RAGDOLLS 

	if (!font.loadFromFile("ARCADE_N.TTF")) {
		return;
	}
	
	counter.setFont(font);
	counter.setColor(Color::White);

	play.setFont(font);
	play.setColor(Color::White);

	controles.setFont(font);
	controles.setColor(Color::White);

	bodyY = 85.0f; //POSICION INICIAL DEL CAÑON
	

	InitPhysics(); // Inicializa la simulación de física
}

void Game::Loop()
{
	while (wnd->isOpen() && level < 1)
	{	
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		//CheckCollitions(); // Verifica colisiones (a implementar)
		//UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}

	while (wnd->isOpen() && level > 0) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		counter.setString(std::to_string(ragdolls));

		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		CheckPositions();
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
}


void Game::DrawGame()
{
	// Función para dibujar los elementos del juego (a implementar)
	sf::RectangleShape controlador(Vector2f(3, 10));
	if (level > 0 && level < 3.5) {
		controlador.setFillColor(sf::Color::Magenta);
		controlador.setPosition(controlCannon->GetPosition().x + 5, controlCannon->GetPosition().y - 4.9);
		controlador.setRotation(90);
		wnd->draw(controlador);
		controlCannon->SetTransform(b2Vec2(controlCannon->GetPosition().x, bodyY), 0.0f);
	}
	
	wnd->draw(counter);
	if (level < 1 || level == 3.5) {
		wnd->draw(play);
		wnd->draw(controles);
	}
	
}

void Game::DoEvents()
{
	if (Keyboard::isKeyPressed(Keyboard::S)) //POSICION (Y) DEL CAÑON
		bodyY = bodyY + 0.5f;
	if (Keyboard::isKeyPressed(Keyboard::W))// W y S PARA SUBIR Y BAJAR
		bodyY = bodyY - 0.5f;
	if (Keyboard::isKeyPressed(Keyboard::R))// R PARA REINTENTAR
		InitPhysics();

	Event evt;

	while (wnd->pollEvent(evt) && level > 0 && level < 3.5)//UNA VEZ EMPEZADO EL JUEGO
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
			break;
			case Event::MouseButtonPressed: //RAGDOLLS CON CLICK
			    Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
			ragdolls += 1;

			b2Body* chest = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1.5, 4.5, 1.0f, 0.1f, 0.1f); //PECHO
			chest->SetTransform(b2Vec2(15.0f, bodyY), 0.0f);

			b2Body* rightLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1, 4.5, 1.0f, 0.5f, 0.1f);
			rightLeg->SetTransform(b2Vec2(16.5f, bodyY + 4.5f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(1.2f, 2.25f), //PIERNA DERECHA
				rightLeg, rightLeg->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.01f, 0.1f, 1.0f);
			
			b2Body* leftLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1, 4.5, 1.0f, 0.5f, 0.1f);
			leftLeg->SetTransform(b2Vec2(13.5f, bodyY + 4.5f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(-1.2f, 2.25f), //PIERNA IZQUIERDA
				leftLeg, leftLeg->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.01f, 0.1f, 1.0f);
			

			b2Body* leftArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 0.5, 4.5, 1.0f, 0.5f, 0.1f);
			leftArm->SetTransform(b2Vec2(13.5f, bodyY), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(-1.2f, -2.25f), //BRAZO IZQUIERDO
				leftArm, leftArm->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.01f, 0.1f, 1.0f);

		
			b2Body* rightArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 0.5, 4.5, 1.0f, 0.5f, 0.1f);
			rightArm->SetTransform(b2Vec2(16.5f, bodyY), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(1.2f, -2.25f), //BRAZO DERECHO
				rightArm, rightArm->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.0f, 0.1f, 1.0f);

			
			b2Body* head = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1.5, 1.5, 1.0f, 0.5f, 0.1f);
			head->SetTransform(b2Vec2(16.0f, bodyY - 2.5f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(0.0f, -2.5f), //CABEZA
				head, head->GetWorldCenter(), 0.0f, 0.1f, 1.0f);

			
			chest->SetAwake(true);
			//controlBody->ApplyForceToCenter(b2Vec2(pos.x * 3000000.0f, pos.x * 3000000.0f), true);
			chest->SetLinearVelocity(b2Vec2( 300.0f, 0.0f));
			    break;
		}
	}

	while (wnd->pollEvent(evt) && level < 1)
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
			break;
		case Event::MouseButtonPressed:
			level += 1;
			InitPhysics();
			break;
		}
		
	}
		
}

void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
	
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(200.0f, 200.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::InitPhysics()
{
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D


	if (level == -1) { // PANTALLA DE INICIO
		counter.setString("Ragdoll VS Barrels");
		counter.setPosition(-40, 40);
		counter.setCharacterSize(10);

		play.setString("Da click para empezar");
		play.setPosition(-20, 60);
		play.setCharacterSize(7);
	}
	if (level == 0) { //PANTALLA CON INFO
		
		counter.setString("El objetivo es tumbar los barriles");
		counter.setPosition(-35, 50);
		counter.setCharacterSize(5);

		play.setString("Da click para Disparar ragdolls");
		play.setPosition(-27, 60);
		play.setCharacterSize(5);
		
		controles.setString("W y S para moverte - R para reintentar");
		controles.setPosition(-45, 80);
		controles.setCharacterSize(5);

		wnd->clear();
	}

	if (level > 0 && level < 3.5) { //SI YA NO ES MENOR A 1 DIBUJA EL MAPA

		controlCannon = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 10);
		controlCannon->SetTransform(b2Vec2(7.0f, 90.0f), 7.0f);

		counter.setPosition(-15,-20);
		counter.setCharacterSize(10);

		b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
		groundBody->SetTransform(b2Vec2(50.0f, 150.0f), 0.0f);

		b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
		leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

		b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
		rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

		b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
		topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

		if (level == 1) //NIVEL 1
		{

			//PLATAFORMAS
			b2Body* box1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box1->SetTransform(b2Vec2(50.0f, 40.0f), 0.0f);

			b2Body* box2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box2->SetTransform(b2Vec2(50.0f, 60.0f), 0.0f);

			b2Body* box3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box3->SetTransform(b2Vec2(50.0f, 80.0f), 0.0f);

			b2Body* box4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box4->SetTransform(b2Vec2(50.0f, 20.0f), 0.0f);


			//BARRILES A TUMBAR 
			barrel1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel1->SetTransform(b2Vec2(50.0f, 37.5), 0.0f);

			barrel2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel2->SetTransform(b2Vec2(50.0f, 57.5f), 0.0f);

			barrel3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel3->SetTransform(b2Vec2(50.0f, 77.5f), 0.0f);

			barrel4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel4->SetTransform(b2Vec2(50.0f, 17.5f), 0.0f);

		}

		if (level == 2) //NIVEL 2
		{

			//OBSTACULOS
			b2Body* obs1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 5);
			obs1->SetTransform(b2Vec2(20.0f, 33.0f), 0.0f);

			b2Body* obs2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 5);
			obs2->SetTransform(b2Vec2(42.0f, 77.0f), 0.0f);


			b2Body* obs3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 10);
			obs3->SetTransform(b2Vec2(77.0f, 72.0f), 0.0f);

			b2Body* obs4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 5);
			obs4->SetTransform(b2Vec2(62.0f, 29.0f), 0.0f);


			//PLATAFORMAS
			b2Body* box1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box1->SetTransform(b2Vec2(50.0f, 40.0f), 0.0f);

			b2Body* box2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box2->SetTransform(b2Vec2(50.0f, 60.0f), 0.0f);


			b2Body* box3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box3->SetTransform(b2Vec2(80.0f, 80.0f), 0.0f);

			b2Body* box4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box4->SetTransform(b2Vec2(80.0f, 20.0f), 0.0f);


			//BARRILES A TUMBAR 
			barrel1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel1->SetTransform(b2Vec2(50.0f, 37.5), 0.0f);

			barrel2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel2->SetTransform(b2Vec2(50.0f, 57.5f), 0.0f);

			Box2DHelper::CreatePulleyJoint(phyWorld, barrel1, barrel1->GetWorldCenter(),
				b2Vec2(95.0f, 77.5f), barrel2, barrel2->GetWorldCenter(), b2Vec2(95.0f, 17.5f), 1.0f);

			barrel3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel3->SetTransform(b2Vec2(80.0f, 77.5f), 0.0f);

			barrel4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel4->SetTransform(b2Vec2(80.0f, 17.5f), 0.0f);

			Box2DHelper::CreatePulleyJoint(phyWorld, barrel3, barrel3->GetWorldCenter(),
				b2Vec2(95.0f, 77.5f), barrel4, barrel4->GetWorldCenter(), b2Vec2(95.0f, 17.5f), 1.0f);

		}

		if (level == 3) //NIVEL 3
		{

			//OBSTACULOS

			b2Body* obs = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 10);
			obs->SetTransform(b2Vec2(65.0f, 35.0f), 0.0f);

			b2Body* obs1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 25);
			obs1->SetTransform(b2Vec2(40.0f, 10.0f), 0.0f);

			b2Body* obs2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 30, 3);
			obs2->SetTransform(b2Vec2(50.0f, 31.0f), 0.0f);

			b2Body* obs3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 10);
			obs3->SetTransform(b2Vec2(74.8f, 57.0f), -35.0f);
			b2Body* obs4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 20);
			obs4->SetTransform(b2Vec2(70.2f, 50.0f), 0.0f);


			b2Body* obs5 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 3);
			obs5->SetTransform(b2Vec2(50.0f, 70.0f), 0.0f);

			b2Body* obs6 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 3);
			obs6->SetTransform(b2Vec2(60.0f, 77.0f), 0.0f);

			b2Body* obs7 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 3);
			obs7->SetTransform(b2Vec2(25.0f, 73.0f), 0.0f);

			b2Body* obs8 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 3);
			obs8->SetTransform(b2Vec2(45.0f, 63.0f), 0.0f);

			//PLATAFORMAS

			b2Body* box1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 30, 3);
			box1->SetTransform(b2Vec2(50.0f, 40.0f), 0.0f);

			b2Body* box2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box2->SetTransform(b2Vec2(80.0f, 60.0f), 0.0f);

			b2Body* box3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box3->SetTransform(b2Vec2(80.0f, 80.0f), 0.0f);

			b2Body* box4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 30, 3);
			box4->SetTransform(b2Vec2(59.5f, 20.0f), 0.0f);


			//BARRILES A TUMBAR 

			barrel1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel1->SetTransform(b2Vec2(50.0f, 17.5f), 0.0f);

			barrel2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel2->SetTransform(b2Vec2(50.0f, 37.5), 0.0f);

			Box2DHelper::CreatePulleyJoint(phyWorld, barrel1, barrel1->GetWorldCenter(),
				b2Vec2(95.0f, 17.5f), barrel2, barrel2->GetWorldCenter(), b2Vec2(95.0f, 37.5), 1.0f);


			barrel3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel3->SetTransform(b2Vec2(80.0f, 57.5f), 0.0f);

			barrel4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel4->SetTransform(b2Vec2(80.0f, 77.5f), 0.0f);


			Box2DHelper::CreateRevoluteJoint(phyWorld, barrel3, b2Vec2(80.0f, 67.5f), barrel4, 1.0f, 1.0f, 1.0f, 1.0f, false, false);

		}
		
	}
}

void Game::CheckPositions() //CHECKEA SI LOS BARRILES SALIERON DE SU POSICION INICIAL, SI TODOS SE MOVIERON, BORRA TODO, CAMBIA DE NIVEL Y LO DIBUJA
{	
	if (level < 1) {

	}
	if (level == 1) {
		if (barrel1->GetPosition().y > 17.5f && barrel2->GetPosition().y > 57.5f && barrel3->GetPosition().y > 77.5f && barrel4->GetPosition().y > 17.5f) {
			level = 1.5f;
		}
	}
	if (level == 1.5f) { 
		wnd->clear();
		level = 2;
		InitPhysics();
	}
	if (level == 2) {
		if (barrel1->GetPosition().y > 37.5 && barrel2->GetPosition().y > 57.5f && barrel3->GetPosition().y > 77.5f && barrel4->GetPosition().y > 17.5f) {
			level = 2.5f;
		}
	}
	if (level == 2.5f) {
		wnd->clear();
		level = 3;
		InitPhysics();
	}
	if (level == 3) {
		if (barrel1->GetPosition().y > 37.5 && barrel2->GetPosition().y > 57.5f && barrel3->GetPosition().y > 77.5f && barrel4->GetPosition().y > 17.5f) {
			level = 3.5f;
		}
	}
	if (level == 3.5f) {
		wnd->clear();
		wnd->clear();

		controles.setString("");

		counter.setPosition(70, 60);
		counter.setCharacterSize(5);

		play.setString("Ganaste! Disparaste      Ragdolls!");
		play.setPosition(-35, 60);
		play.setCharacterSize(5);
	
		InitPhysics();
	}
}

Game::~Game(void)
{
	// Destructor de la clase Game (a implementar si es necesario)
}