#include "Asteroid.h"
#include "SmallAsteroid.h"
#include "Asteroids.h"
#include "Life.h"
#include "PowerUpShield.h"
#include "AddedShield.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include "Player.h"
#include "Allien.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
	mSmallAsteroidCount = 0;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);

	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);

	//Create an ambiet light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);

	Animation *explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "explosion_fs.png");
	Animation *life_anim = AnimationManager::GetInstance().CreateAnimationFromFile("life", 58,624, 58, 78, "life2.png");
	Animation *shield_anim = AnimationManager::GetInstance().CreateAnimationFromFile("shield", 64, 64, 64, 64, "Shield_Power.png");
	Animation *addshield_anim = AnimationManager::GetInstance().CreateAnimationFromFile("addedshield", 128, 128, 128, 128, "Add_Shield.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "asteroid1_fs.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 128, 128, 128, 128, "spaceship_fs.png");

	// Create a spaceship and add it to the world
	mGameWorld->AddObject(CreateSpaceship());
	// Create some asteroids and add them to the world
	CreateAsteroids(2);
	CreateLife();
	CreateShield();
	mGameWorld->AddObject(CreateAllien());

	//Create the GUI
	CreateGUI();

	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);

	// Start the game
	GameSession::Start();
}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	switch (key)
	{
	case ' ':
		mSpaceship->Shoot();
		break;
	default:
		break;
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
		// If up arrow key is pressed start applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(10); break;
		// If left arrow key is pressed start rotating anti-clockwise
	case GLUT_KEY_LEFT: mSpaceship->Rotate(90); break;
		// If right arrow key is pressed start rotating clockwise
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90); break;
        //If down arrow key is pressed start applying backward thrust
	case GLUT_KEY_DOWN:mSpaceship->Thrust(-10); break;
		// Default case - do nothing
	default: break;
	}
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	switch (key)
	{
		// If up arrow key is released stop applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
		//If down arrow key is release stop applying backward thrust
	case GLUT_KEY_DOWN:mSpaceship->Thrust(0); break;
		// If left arrow key is released stop rotating
	case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
		// If right arrow key is released stop rotating
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
		// Default case - do nothing
	default: break;
	}
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////
////Screen 

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
	if (object->GetType() == GameObjectType("Asteroid"))
	{
		shared_ptr<GameObject>explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		CreateSmallAsteroids(3,object);
		mAsteroidCount--;
     		
	}

	if (object->GetType() == GameObjectType("SmallAsteroid"))
	{
		shared_ptr<GameObject>explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		mSmallAsteroidCount--;
		if (mAsteroidCount <= 0 && mSmallAsteroidCount <= 0)
		{
			SetTimer(500, START_NEXT_LEVEL);
		}
	}

	if (object->GetType() == GameObjectType("Allien"))
	{
		shared_ptr<GameObject>explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
	}

	if (object->GetType() == GameObjectType("Life"))
	{
		
		std::ostringstream msg_stream;
		msg_stream << "Lives: " << mPlayer.GotLife();
		// Get the lives left message as a string
		std::string lives_msg = msg_stream.str();
		mLivesLabel->SetText(lives_msg);
	}

	if (object->GetType()==GameObjectType("PowerUpShield")){
		AddShield();
	}

}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
		int num_asteroids = 2 + 2 * mLevel;
		mGameWorld->AddObject(mAllienShip);

		CreateAsteroids(num_asteroids);
		CreateLife();
		CreateShield();
	}

}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite>spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetSprite(spaceship_sprite);
	mSpaceship->SetScale(0.1f);

	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mSpaceship;
}


shared_ptr<GameObject>Asteroids::CreateExplosion()
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite>explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject>explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
}

//Createasteroids
void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite>asteroid_sprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject>asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}

}

//The small asteroids appear where the big one is destroyed
void Asteroids::CreateSmallAsteroids(const uint small_asteroids, shared_ptr<GameObject> object)
{
	mSmallAsteroidCount += small_asteroids;
	for (uint i = 0; i < small_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite>asteroid_sprite = make_shared<Sprite>(anim_ptr->GetWidth()/2, anim_ptr->GetHeight()/2, anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject>small_asteroid = make_shared<SmallAsteroid>();
		small_asteroid->SetBoundingShape(make_shared<BoundingSphere>(small_asteroid->GetThisPtr(), 5.0f));
		small_asteroid->SetSprite(asteroid_sprite);
		small_asteroid->SetScale(0.2f);
		small_asteroid->SetPosition(object->GetPosition());
		mGameWorld->AddObject(small_asteroid);
		
	}

}

//Create Life
void Asteroids::CreateLife()
{
	
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("life");
		shared_ptr<Sprite>life_sprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		life_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject>life = make_shared<Life>();
		life->SetBoundingShape(make_shared<BoundingSphere>(life->GetThisPtr(), 5.0f));
		life->SetSprite(life_sprite);
		life->SetScale(0.2f);
		mGameWorld->AddObject(life);
	

}

//Create Shield
void Asteroids::CreateShield()
{

	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("shield");
	shared_ptr<Sprite>shield_sprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	shield_sprite->SetLoopAnimation(true);
	shared_ptr<GameObject>shield = make_shared<PowerUpShield>();
	shield->SetBoundingShape(make_shared<BoundingSphere>(shield->GetThisPtr(), 5.0f));
	shield->SetSprite(shield_sprite);
	shield->SetScale(0.2f);
	mGameWorld->AddObject(shield);

}

//Add the shield
void Asteroids::AddShield(){
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("addedshield");
	shared_ptr<Sprite>shield_sprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	shared_ptr<GameObject>shield_pick_up = make_shared<AddedShield>(mSpaceship);
	shield_pick_up->SetBoundingShape(make_shared<BoundingSphere>(shield_pick_up->GetThisPtr(), 10.0f));
	shield_pick_up->SetSprite(shield_sprite);
	shield_pick_up->SetScale(0.2f);
	mGameWorld->AddObject(shield_pick_up);
}

shared_ptr<Allien> Asteroids::CreateAllien()
{


	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mAllienShip = make_shared<Allien>();
	mAllienShip->SetBoundingShape(make_shared<BoundingSphere>(mAllienShip->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceship->SetBulletShape(bullet_shape);
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite>allien_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mAllienShip->SetSprite(allien_sprite);
	mAllienShip->SetScale(0.1f);
	mAllienShip->SetAllien(mSpaceship);
	return mAllienShip;
}

//Create GUI
void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	//Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAME OVER"));
	//Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	//Set the vertical aligment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	//Set the visibility of the label to(hidden)
	mGameOverLabel->SetVisible(false);
	//Add the GUILabel to the GUIContainer
	shared_ptr<GUIComponent>game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));
}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	shared_ptr<GameObject>explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);
	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);
	if (lives_left > 0){ SetTimer(1000, CREATE_NEW_PLAYER); }
	if (lives_left == 0){ SetTimer(500, SHOW_GAME_OVER);
	mGameOverLabel->SetVisible(true);
	}

}




