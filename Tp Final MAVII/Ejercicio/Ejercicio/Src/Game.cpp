#include "Game.h"
#include "Box2DHelper.h"
#include <print>
#include <iostream>
#include <string>
#include "b2ExSoftCircleBody.h"
//#include "Avatar.h"


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

	CanonTextura.loadFromFile("canon.png");	//CARGA LA TEXTURA DEL CAÑON

	BarrilTextura.loadFromFile("barrel.png");	//CARGA LA TEXTURA DEL BARRIL

	BoneTextura.loadFromFile("bone.png");

	BodyTextura.loadFromFile("body.png");
	
	HeadTextura.loadFromFile("head.png");

	WallTextura.loadFromFile("wall.png");
	WallRotTextura.loadFromFile("wallrot.png");

	BallTextura.loadFromFile("ball.png");

	shot = 0;

	InitPhysics(); // Inicializa la simulación de física
}

void Game::Loop()
{
	while (wnd->isOpen() && level < 1)
	{	
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		//DoEvents(); // Maneja los eventos (input del usuario)
		//CheckCollitions(); // Verifica colisiones (a implementar)
		PreGame();
		//UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}

	while (wnd->isOpen() && level >= 1) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
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
	sf::RectangleShape controlador(Vector2f(1, 1));
	if (level > 0 && level < 3.5) {
		//controlador.setFillColor(sf::Color::Magenta);
		controlador.setPosition(controlCannon->GetPosition().x, controlCannon->GetPosition().y);
		controlador.setRotation(90);
		wnd->draw(controlador);
		controlCannon->SetTransform(b2Vec2(controlCannon->GetPosition().x, bodyY), 0.0f);


		CanonAvatar->Actualizar();
		CanonAvatar->Dibujar(*wnd);
		BarrrilAvatar1->Actualizar();
		BarrrilAvatar1->Dibujar(*wnd);
		BarrrilAvatar2->Actualizar();
		BarrrilAvatar2->Dibujar(*wnd);
		BarrrilAvatar3->Actualizar();
		BarrrilAvatar3->Dibujar(*wnd);
		BarrrilAvatar4->Actualizar();
		BarrrilAvatar4->Dibujar(*wnd);

		WallAvatar1->Actualizar();
		WallAvatar1->Dibujar(*wnd);
		WallAvatar2->Actualizar();
		WallAvatar2->Dibujar(*wnd);
		WallAvatar3->Actualizar();
		WallAvatar3->Dibujar(*wnd);
		WallAvatar4->Actualizar();
		WallAvatar4->Dibujar(*wnd);

		if (shot == 1)
		{
			bone1->Actualizar();
			bone1->Dibujar(*wnd);
			bone2->Actualizar();
			bone2->Dibujar(*wnd);
			bone3->Actualizar();
			bone3->Dibujar(*wnd);
			bone4->Actualizar();
			bone4->Dibujar(*wnd);
			bodyAvatar->Actualizar();
			bodyAvatar->Dibujar(*wnd);
			headAvatar->Actualizar();
			headAvatar->Dibujar(*wnd);
		}

		if (level == 2)
		{
			CircleAvatar1->Actualizar();
			CircleAvatar1->Dibujar(*wnd);
			CircleAvatar2->Actualizar();
			CircleAvatar2->Dibujar(*wnd);
		}

		if (level == 3)
		{
			OrbAvatar->Actualizar();
			OrbAvatar->Dibujar(*wnd);
			OrbAvatar1->Actualizar();
			OrbAvatar1->Dibujar(*wnd);
			OrbAvatar2->Actualizar();
			OrbAvatar2->Dibujar(*wnd);
			OrbAvatar3->Actualizar();
			OrbAvatar3->Dibujar(*wnd);
			OrbAvatar4->Actualizar();
			OrbAvatar4->Dibujar(*wnd);
			OrbAvatar5->Actualizar();
			OrbAvatar5->Dibujar(*wnd);
			CircleAvatar1->Actualizar();
			CircleAvatar1->Dibujar(*wnd);
			CircleAvatar2->Actualizar();
			CircleAvatar2->Dibujar(*wnd);
		}
	}
	
	wnd->draw(counter);
	if (level < 1 || level == 3.5) {
		wnd->draw(play);
		wnd->draw(controles);
	}
	
}

void Game::PreGame()
{
	Event evt;

	while (wnd->pollEvent(evt))
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

void Game::DoEvents()
{
	if (Keyboard::isKeyPressed(Keyboard::S) && bodyY < 145) //POSICION (Y) DEL CAÑON
		bodyY = bodyY + 0.5f;
	if (Keyboard::isKeyPressed(Keyboard::W) && bodyY > -40)// W y S PARA SUBIR Y BAJAR
		bodyY = bodyY - 0.5f;
	if (Keyboard::isKeyPressed(Keyboard::R))// R PARA REINTENTAR
		InitPhysics();

	Event evt;

	while (wnd->pollEvent(evt))//UNA VEZ EMPEZADO EL JUEGO
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
			break;
			case Event::MouseButtonPressed: //RAGDOLLS CON CLICK

			shot = 1;
			Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
			ragdolls += 1;

			densidad = 100;
			friccion = 100;
			restitucion = 100;

			b2Body* chest = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1.5, 4.5, densidad, 0.1f, 0.1f); //PECHO
			chest->SetTransform(b2Vec2(15.0f, bodyY), 0.0f);

			b2Body* rightLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1, 4.5, densidad, 0.5f, 0.1f); //PIERNA DERECHA
			rightLeg->SetTransform(b2Vec2(16.5f, bodyY + 4.5f), 0.0f);
			

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(1.2f, 2.25f),
				rightLeg, rightLeg->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.01f, 0.1f, 1.0f);
			
			b2Body* leftLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1, 4.5, densidad, 0.5f, 0.1f); //PIERNA IZQUIERDA
			leftLeg->SetTransform(b2Vec2(13.5f, bodyY + 4.5f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(-1.2f, 2.25f),
				leftLeg, leftLeg->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.01f, 0.1f, 1.0f);
			

			b2Body* leftArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 0.5, 4.5, densidad, 0.5f, 0.1f); //BRAZO IZQUIERDO
			leftArm->SetTransform(b2Vec2(13.5f, bodyY), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(-1.2f, -2.25f),
				leftArm, leftArm->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.01f, 0.1f, 1.0f);

		
			b2Body* rightArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 0.5, 4.5, densidad, 0.5f, 0.1f); //BRAZO DERECHO
			rightArm->SetTransform(b2Vec2(16.5f, bodyY), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(1.2f, -2.25f), 
				rightArm, rightArm->GetWorldCenter() - b2Vec2(0.0f, 2.5f), 0.0f, 0.1f, 1.0f);

			
			b2Body* head = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1.5, 1.5, densidad, 0.5f, 0.1f);//CABEZA
			head->SetTransform(b2Vec2(16.0f, bodyY - 2.5f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, chest, chest->GetWorldCenter() + b2Vec2(0.0f, -2.5f), 
				head, head->GetWorldCenter(), 0.0f, 0.1f, 1.0f);

			bone1 = new Avatar(rightLeg, new Sprite(BoneTextura));
			bone2 = new Avatar(leftLeg, new Sprite(BoneTextura));
			bone3 = new Avatar(rightArm, new Sprite(BoneTextura));
			bone4 = new Avatar(leftArm, new Sprite(BoneTextura));

			bodyAvatar = new Avatar(chest, new Sprite(BodyTextura));
			headAvatar = new Avatar(head, new Sprite(HeadTextura));

			chest->SetAwake(true);
			//controlBody->ApplyForceToCenter(b2Vec2(pos.x * 3000000.0f, pos.x * 3000000.0f), true);
			chest->SetLinearVelocity(b2Vec2( 300.0f, 0.0f));
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

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para 
	debugRender->SetFlags(0x0003); // Configura el renderizador para dibujar todas las formas de debug
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

		CanonAvatar = new Avatar(controlCannon, new Sprite(CanonTextura));

		counter.setPosition(-15,-20);
		counter.setCharacterSize(10);

		b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
		groundBody->SetTransform(b2Vec2(-10.0f, 150.0f), 0.0f);

		b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
		leftWallBody->SetTransform(b2Vec2(-50.0f, 50.0f), 0.0f);

		b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
		rightWallBody->SetTransform(b2Vec2(150.0f, 50.0f), 0.0f);

		b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
		topWallBody->SetTransform(b2Vec2(50.0f, -50.0f), 0.0f);

		WallAvatar1 = new Avatar(groundBody, new Sprite(WallRotTextura)); 
		WallAvatar2 = new Avatar(leftWallBody, new Sprite(WallTextura));
		WallAvatar3 = new Avatar(rightWallBody, new Sprite(WallTextura));
		WallAvatar4 = new Avatar(topWallBody, new Sprite(WallRotTextura));

		

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
			barrel1->SetTransform(b2Vec2(50.0f, 37.6), 0.0f);

			barrel2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel2->SetTransform(b2Vec2(50.0f, 57.5f), 0.0f);

			barrel3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel3->SetTransform(b2Vec2(50.0f, 77.5f), 0.0f);

			barrel4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel4->SetTransform(b2Vec2(50.0f, 17.5f), 0.0f);

			
			BarrrilAvatar1 = new Avatar(barrel1, new Sprite(BarrilTextura)); //PONE LOS SPRITES
			BarrrilAvatar2 = new Avatar(barrel2, new Sprite(BarrilTextura));
			BarrrilAvatar3 = new Avatar(barrel3, new Sprite(BarrilTextura));
			BarrrilAvatar4 = new Avatar(barrel4, new Sprite(BarrilTextura));
		}

		if (level == 2) //NIVEL 2
		{
			
			//OBSTACULOS
			b2Body* obs1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 10);
			obs1->SetTransform(b2Vec2(20.0f, 33.0f), 0.0f);

			b2Body* obs2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 10);
			obs2->SetTransform(b2Vec2(62.0f, 50.0f), 0.0f);

			b2Body* obs3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 10);
			obs3->SetTransform(b2Vec2(70.0f, 50.0f), 0.0f);

			b2Body* obs4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 10);
			obs4->SetTransform(b2Vec2(62.0f, 29.0f), 0.0f);

			b2Body* obs5 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 30, 3);
			obs5->SetTransform(b2Vec2(50.0f, 120.0f), -35.0f);
			 
			b2Body* obs6 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 20);
			obs6->SetTransform(b2Vec2(90.0f, 110.0f), 0.0f);
			
			b2Body* obs7 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 100);
			obs7->SetTransform(b2Vec2(90.0f, 10.0f), 0.0f);

			b2Body* obs8 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 30);
			obs8->SetTransform(b2Vec2(100.0f, 63.0f), 0.0f);
			
			b2Body* obs9 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 10);
			obs9->SetTransform(b2Vec2(60.0f, 80.0f), 0.0f);
			


			b2Body* circle = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f);
			b2Vec2 center = b2Vec2(45.0f, 65.0f);
			circle->SetTransform(center, 0.0f);

			b2ExSoftCircleBodyDef def;
			def.numParts = 8; // Número de círculos internos linkeados
			def.radius = 10.0f; // Radio
			Box2DHelper::CreateDistanceJoint(phyWorld, topWallBody, b2Vec2(45.0f, -50.0f), circle, b2Vec2(45.0f, 50.0f), 50.0f, 100.0f, 100.0f);
			def.center = b2Vec2(45.0f, 65.0f);
			def.softness = 0.85f; // Blandura entre 0 y 1
			def.density = 10.0f;	// Densidad
			b2ExSoftCircleBody* body = b2ExSoftCircleBody_Create(phyWorld, &def);
			

			b2Body* circle2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f);
			b2Vec2 center2 = b2Vec2(80.0f, 20.0f);
			circle2->SetTransform(center2, 0.0f);

			b2ExSoftCircleBodyDef def2;
			def2.numParts = 8; // Número de círculos internos linkeados
			def2.radius = 10.0f; // Radio
			Box2DHelper::CreateDistanceJoint(phyWorld, topWallBody, b2Vec2(80.0f, -50.0f), circle2, b2Vec2(80.0f, 10.0f), 20.0f, 100.0f, 100.0f);
			def2.center = b2Vec2(80.0f, 15.0f);
			def2.softness = 0.85f; // Blandura entre 0 y 1
			def2.density = 10.0f;	// Densidad
			b2ExSoftCircleBody* body2 = b2ExSoftCircleBody_Create(phyWorld, &def2);
			

			//PLATAFORMAS
			b2Body* box1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box1->SetTransform(b2Vec2(120.0f, -20.0f), 0.0f);

			b2Body* box2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box2->SetTransform(b2Vec2(130.0f, 80.0f), 0.0f);


			b2Body* box3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box3->SetTransform(b2Vec2(100.0f, 120.0f), 0.0f);

			b2Body* box4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box4->SetTransform(b2Vec2(80.0f, 60.0f), 0.0f);


			//BARRILES A TUMBAR 
			barrel1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel1->SetTransform(b2Vec2(120.0f, -22.5), 0.0f);

			barrel2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel2->SetTransform(b2Vec2(130.0f, 77.5f), 0.0f);

			Box2DHelper::CreatePulleyJoint(phyWorld, barrel1, barrel1->GetWorldCenter(),
				b2Vec2(150.0f, 77.5f), barrel2, barrel2->GetWorldCenter(), b2Vec2(150.0f, 17.5f), 1.0f);

			barrel3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel3->SetTransform(b2Vec2(100.0f, 117.5f), 0.0f);

			barrel4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel4->SetTransform(b2Vec2(80.0f, 57.5f), 0.0f);

			Box2DHelper::CreatePulleyJoint(phyWorld, barrel3, barrel3->GetWorldCenter(),
				b2Vec2(95.0f, 77.5f), barrel4, barrel4->GetWorldCenter(), b2Vec2(95.0f, 57.5f), 1.0f);


			BarrrilAvatar1 = new Avatar(barrel1, new Sprite(BarrilTextura)); //PONE LOS SPRITES
			BarrrilAvatar2 = new Avatar(barrel2, new Sprite(BarrilTextura));
			BarrrilAvatar3 = new Avatar(barrel3, new Sprite(BarrilTextura));
			BarrrilAvatar4 = new Avatar(barrel4, new Sprite(BarrilTextura));

			CircleAvatar1 = new Avatar(circle, new Sprite(BallTextura));
			CircleAvatar2 = new Avatar(circle2, new Sprite(BallTextura));
		}
		
		if (level == 3) //NIVEL 3
		{

			//OBSTACULOS
			
			b2Body* obs = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 100);
			obs->SetTransform(b2Vec2(100.0f, 29.0f), 0.0f);
			
			b2Body* obs1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 10);
			obs1->SetTransform(b2Vec2(100.0f, -40.0f), 0.0f);
			
			b2Body* obs2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 50);
			obs2->SetTransform(b2Vec2(120.0f, 50.0f), 0.0f);
						
			b2Body* obs3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 40);
			obs3->SetTransform(b2Vec2(135.0f, 25.0f), -15.0f);
						
			b2Body* obs4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 20);
			obs4->SetTransform(b2Vec2(90.2f, 80.0f), 15.0f);
					
			b2Body* obs5 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 70, 3);
			obs5->SetTransform(b2Vec2(120.0f, 75.0f), 0.0f);

			b2Body* obs6 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 50);
			obs6->SetTransform(b2Vec2(90.0f, 130.0f), 0.0f);

			
			orb = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000, 0.0f, 30.0f);
			orb->SetTransform(b2Vec2(25.0f, 140.0f), 0.0f);
			orb->SetLinearVelocity(b2Vec2(0.0f, 200.0f));
			
			orb1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000, 0.0f, 30.0f);
			orb1->SetTransform(b2Vec2(35.0f, -20.0f), 0.0f);
			orb1->SetLinearVelocity(b2Vec2(0.0f, -200.0f));

			orb2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000, 0.0f, 30.0f);
			orb2->SetTransform(b2Vec2(45.0f, 140.0f), 0.0f);
			orb2->SetLinearVelocity(b2Vec2(0.0f, 200.0f));

			orb3 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000, 0.0f, 30.0f);
			orb3->SetTransform(b2Vec2(55.0f, -20.0f), 0.0f);
			orb3->SetLinearVelocity(b2Vec2(0.0f, -200.0f));

			orb4 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000, 0.0f, 30.0f);
			orb4->SetTransform(b2Vec2(65.0f, 140.0f), 0.0f);
			orb4->SetLinearVelocity(b2Vec2(0.0f, 200.0f));

			orb5 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000, 0.0f, 30.0f);
			orb5->SetTransform(b2Vec2(75.0f, -20.0f), 0.0f);
			orb5->SetLinearVelocity(b2Vec2(0.0f, -200.0f));

			b2Body* circle = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000.0f, 0.0f, 10.0f);
			circle->SetTransform(b2Vec2(-45.0f, -30.0f), 0.0f);
			Box2DHelper::CreateDistanceJoint(phyWorld, leftWallBody, b2Vec2(-50.0f, 50.0f), circle, circle->GetWorldCenter(), 20.0f, 100.0f, 100.0f);
			circle->SetLinearVelocity(b2Vec2(100.0f, 0.0f));
			b2Body* pebble = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 1000.0f, 0.0f, 10.0f);
			pebble->SetTransform(b2Vec2(-30.0f, -30.0f), 0.0f);
			b2Body* pebble1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 1000.0f, 0.0f, 10.0f);
			pebble1->SetTransform(b2Vec2(-25.0f, -30.0f), 0.0f);
			Box2DHelper::CreateRevoluteJoint(phyWorld, circle, circle->GetWorldCenter() + b2Vec2(5.0f, 0.0f), pebble, 1.0f, 350.0f, 1000.0f, 100.0f, true, true);
			Box2DHelper::CreateRevoluteJoint(phyWorld, circle, circle->GetWorldCenter() + b2Vec2(-5.0f, 0.0f), pebble1, 1.0f, 350.0f, 1000.0f, 100.0f, true, true);


			b2Body* circle2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 3, 1000.0f, 0.0f, 10.0f);
			circle2->SetTransform(b2Vec2(-40.0f, 140.0f), 0.0f);
			Box2DHelper::CreateDistanceJoint(phyWorld, leftWallBody, b2Vec2(-50.0f, 50.0f), circle2, circle2->GetWorldCenter(), 30.0f, 100.0f, 100.0f);
			circle2->SetLinearVelocity(b2Vec2(0.0f, 100.0f));
			b2Body* pebble2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 1000.0f, 0.0f, 10.0f);
			pebble2->SetTransform(b2Vec2(-25.0f, 140.0f), 0.0f);
			b2Body* pebble3 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 1000.0f, 0.0f, 10.0f);
			pebble3->SetTransform(b2Vec2(-20.0f, 140.0f), 0.0f);
			Box2DHelper::CreateRevoluteJoint(phyWorld, circle2, circle2->GetWorldCenter() + b2Vec2(5.0f, 0.0f), pebble2, 1.0f, 350.0f, 1000.0f, 100.0f, true, true);
			Box2DHelper::CreateRevoluteJoint(phyWorld, circle2, circle2->GetWorldCenter() + b2Vec2(-5.0f, 0.0f), pebble3, 1.0f, 350.0f, 1000.0f, 100.0f, true, true);

			//PLATAFORMAS

			b2Body* box4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 20, 3);
			box4->SetTransform(b2Vec2(130.0f, 0.0f), 0.0f);

			b2Body* box1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 30, 3);
			box1->SetTransform(b2Vec2(115.0f, -20.0f), 0.0f);

			b2Body* box2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box2->SetTransform(b2Vec2(130.0f, 60.0f), 0.0f);

			b2Body* box3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
			box3->SetTransform(b2Vec2(130.0f, 130.0f), 0.0f);

			
			
			
			//BARRILES A TUMBAR 

			barrel1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel1->SetTransform(b2Vec2(130.0f, -2.5f), 0.0f);

			barrel2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel2->SetTransform(b2Vec2(120.0f, -22.5), 0.0f);

			Box2DHelper::CreatePulleyJoint(phyWorld, barrel1, barrel1->GetWorldCenter(),
			b2Vec2(150.0f, -2.5f), barrel2, barrel2->GetWorldCenter(), b2Vec2(150.0f, -22.5), 1.0f);


			barrel3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel3->SetTransform(b2Vec2(130.0f, 57.5f), 0.0f);

			barrel4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
			barrel4->SetTransform(b2Vec2(130.0f, 127.5f), 0.0f);

			Box2DHelper::CreateRevoluteJoint(phyWorld, barrel3, b2Vec2(130.0f, 70.0f), barrel4, 1.0f, 1.0f, 1.0f, 1.0f, false, false);
			
			
			
			BarrrilAvatar1 = new Avatar(barrel1, new Sprite(BarrilTextura)); //PONE LOS SPRITES
			BarrrilAvatar2 = new Avatar(barrel2, new Sprite(BarrilTextura));
			BarrrilAvatar3 = new Avatar(barrel3, new Sprite(BarrilTextura));
			BarrrilAvatar4 = new Avatar(barrel4, new Sprite(BarrilTextura));

			OrbAvatar = new Avatar(orb, new Sprite(BallTextura));
			OrbAvatar1 = new Avatar(orb1, new Sprite(BallTextura));
			OrbAvatar2 = new Avatar(orb2, new Sprite(BallTextura));
			OrbAvatar3 = new Avatar(orb3, new Sprite(BallTextura));
			OrbAvatar4 = new Avatar(orb4, new Sprite(BallTextura));
			OrbAvatar5 = new Avatar(orb5, new Sprite(BallTextura));

			CircleAvatar1 = new Avatar(circle, new Sprite(BallTextura));
			CircleAvatar2 = new Avatar(circle2, new Sprite(BallTextura));

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
		if (barrel1->GetPosition().y > -2.5f && barrel2->GetPosition().y > -22.5f && barrel3->GetPosition().y > 57.5f && barrel4->GetPosition().y > 127.5f) {
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