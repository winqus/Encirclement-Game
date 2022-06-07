#include <iostream>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "EncirclementBase.h"


using namespace enc;
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::unique_ptr;
using std::shared_ptr;

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define WINDOW_STYLE (sf::Style::Titlebar | sf::Style::Close)

const Vector2 center(SCREEN_WIDTH / 2., SCREEN_HEIGHT / 2.);

sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Encirclement", WINDOW_STYLE, sf::ContextSettings(0U, 0U, 4U, 1U, 1U, 0U, false));

sf::Clock deltaClock;



const string baseDirPath = "..\\";

const map<string, string> textureFilePathMap = {
	{"error",			baseDirPath + "Sprites\\" + "ErrorSprite.png"},
	{"player",			baseDirPath + "Sprites\\" + "PlayerSprite.png"},
	{"turret",			baseDirPath + "Sprites\\" + "TurretSprite.png"},
	{"enemyBullet",		baseDirPath + "Sprites\\" + "EnemyBulletSprite3.png"},
	{"playerBullet",	baseDirPath + "Sprites\\" + "PlayerBulletSprite.png"}
};
const map<string, string> fontFilePathMap = {
	{"default",	baseDirPath + "Fonts\\" + "MajorMonoDisplay-Regular.ttf"},
	{"title",	baseDirPath + "Fonts\\" + "MajorMonoDisplay-Regular.ttf"}
};
const map<string, string> soundFilePathMap = {
	{"playerShoot",	baseDirPath + "Sounds\\" + "laser5fast.wav"},
	{"playerHit",	baseDirPath + "Sounds\\" + "quickLasers.wav"},
	{"enemyShoot",	baseDirPath + "Sounds\\" + "laser5.wav"}
};
map<string, shared_ptr<sf::Texture>> textures;
map<string, shared_ptr<sf::Font>> fonts;
map<string, shared_ptr<sf::SoundBuffer>> sounds;

static GameStats gameStats;

float DeltaTime();
void LoadTextures();
void LoadFonts();
void LoadSounds();
void DrawGameObjectWithSprite(const GameObject& gameObject, sf::Sprite& sprite);
void DrawColliders(const std::vector<GameObject*>& gameObjects);
void DrawArena(const Arena& arena);
void DrawDotAt(Vector2 position, float radius);
void UpdateViewSize(sf::View& view, float size);
sf::Text CreateText(string text, int size, sf::Color color, bool isBold, Vector2 position);
sf::Texture& GetTexture(string name);
sf::Font& GetFont(string name);
void PlayRequestedSounds();

void Game();

string gameState = "menu";
const sf::Color greyColor(61, 61, 61);
sf::Color baseColor = sf::Color::White;
BulletPool bulletPool;
Player player = Player(10, 0.07f);
Arena gameArena = Arena(100.f, 1, 0.5f, 150.0f, 20.0f); // (100.f, 100.0f, 10.0f) - radius, gap, rot speed
sf::View defaultView(sf::Vector2f(SCREEN_WIDTH / 2., SCREEN_HEIGHT / 2.), sf::Vector2f(SCREEN_WIDTH / 2. + 50.0f, SCREEN_HEIGHT / 2. + 50.0f));
sf::View gameView;
sf::Sound sound1, sound2, sound3;
bool customScroll;
float initialGameViewSize;

int main() {
	srand((unsigned)time(0));

	LoadTextures(); 
	LoadFonts();
	LoadSounds();
	baseColor = greyColor;
	window.setFramerateLimit(60);

	player.transform.scale(Vector2(.25f, .25f)); // 0.25f
	player.transform.position(center);
	player.AddCollider(6.0f, Vector2::Zero, true);
	gameArena.transform.position(center);
	gameArena.SetTarget(&player);
	gameArena.SetBulletPool(&bulletPool);
	gameArena.SetTurretPreferences(1.0f, 50.0f, 20.0f, 1, 45.0f, 0.8f);

	customScroll = false;

	/* Create turrets */
	//for (int i = 0; i < 3; i++) {
	//			//reloadSpeed, bulletSpeed, bulletSpeedVariation, damage, shootingAngle, accuracy
	//	Enemy newTurret(1.0f, 50.0f, 20.0f, 1, 10.0f, 0.9f); 
	//	//Enemy newTurret = gameArena.TurretBasedOnDefaultPreferences();
	//	gameArena.AddTurret(newTurret);
	//}


	Collider::UpdateAllColliders();
	gameArena.Update(0.0f);
	bulletPool.Update(0.0f);

	gameView.setCenter(sf::Vector2f(SCREEN_WIDTH / 2., SCREEN_HEIGHT / 2.));
	gameView.setSize(sf::Vector2f(SCREEN_WIDTH / 2. + 100.0, SCREEN_HEIGHT / 2. + 100.0));
	initialGameViewSize = (float)gameArena.GetRadius() * 2.0f + textures["turret"]->getSize().x;
	UpdateViewSize(gameView, initialGameViewSize);
	window.setView(defaultView);
	

	while (window.isOpen()) {
		Game();
	}


	return 0;
}

void Game() {


	if (player.GetHealth() - GameStats::GetTagHitCount("player") <= 0) {
		gameState = "gameOver";
		player.autofire = false;
		gameArena.engage = false;
		window.setMouseCursorVisible(true);
		window.setMouseCursorGrabbed(false);
	}
	// Handling events
	sf::Event event;
	while (window.pollEvent(event)) {
		/* GENERAL */
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				std::cout << "the escape key was pressed" << std::endl;
			}
			if (event.key.code == sf::Keyboard::Space)
			{
				std::cout << "the space key was pressed" << std::endl;
			}
		}
		if (event.type == sf::Event::MouseMoved)
		{
			Vector2 targetPos = Vector2((float)event.mouseMove.x, (float)event.mouseMove.y);
			player.transform.LookAt(targetPos);
		}
		/* MENU */
		if (gameState == "menu") {
			window.setView(defaultView);
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					gameState = "game";
					gameArena.engage = true;
					baseColor = sf::Color::White;
					window.setMouseCursorVisible(false);
					window.setMouseCursorGrabbed(true);
				}
			}
		}

		/* GAME */
		if (gameState == "game") {
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					float fSizeChange = 100.f * event.mouseWheelScroll.delta;
					if (event.mouseWheelScroll.delta == 1 && gameView.getSize().y <= gameArena.GetRadius() * 3.) //10000.f
						gameView.setSize(gameView.getSize().x + fSizeChange, gameView.getSize().y + fSizeChange);
					else if (event.mouseWheelScroll.delta == -1 && gameView.getSize().y >= 300.f)
						gameView.setSize(gameView.getSize().x + fSizeChange, gameView.getSize().y + fSizeChange);

					customScroll = true;
					window.setView(gameView);
				}
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left)
					player.autofire = false;
				else if (event.mouseButton.button == sf::Mouse::Right)
					customScroll = false;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left) {
					player.autofire = true;					
				}
			}
			if (player.autofire) {
				player.target = Vector2((float)event.mouseButton.x, (float)event.mouseButton.y);
			}
		}

	}
	if (player.autofire) {
		player.ShootBullet(&bulletPool, player.target, gameArena.GetRadius() * 3.0f);
	}
	// Background to black
	window.clear(sf::Color::Black);
	//Views
	if (customScroll == false && gameState == "game") {
		UpdateViewSize(gameView, (float)gameArena.GetRadius() * 2.0f + textures["turret"]->getSize().x);
		window.setView(gameView);
	}
		

	/* PHYSICS UPDATES */
	float deltaTime = DeltaTime();
	Collider::UpdateAllColliders();
	player.Update(deltaTime);
	bulletPool.Update(deltaTime);
	gameArena.Update(deltaTime);
	
	/*SOUNDS*/
	PlayRequestedSounds();
	



	//****************
	// DRAWING
	//****************
	if (gameState == "game" || gameState == "gameOver") {
		
		float defaultFontSize = 24;
		float fontSize = defaultFontSize * (float)(gameView.getSize().x / initialGameViewSize);
		if (fontSize < defaultFontSize)
			fontSize = defaultFontSize;
		sf::Text rotCountText = CreateText(
			"ROTATIONS " + std::to_string(GameStats::arenaRotations),
			fontSize, sf::Color(61, 61, 61, 150), true, Vector2(center.x, center.y + 30.0f)
		);
		sf::Text healthText = CreateText(
			"LIFEPOINTS " + std::to_string(player.GetHealth() - GameStats::GetTagHitCount("player")),
			fontSize, sf::Color(255, 0, 0, 80), true, Vector2(center.x, center.y + 40.0f + fontSize/2.)
		);
		rotCountText.setScale(0.5f, 0.5f);
		healthText.setScale(0.5f, 0.5f);
		window.draw(rotCountText);
		window.draw(healthText);
	}

	/* ARENA */
	DrawArena(gameArena);

	/* PLAYER */
	sf::Sprite playerSprite;
	playerSprite.setTexture(GetTexture("player"));
	playerSprite.setColor(baseColor);
	DrawGameObjectWithSprite(player, playerSprite);

	/* TURRETS */
	for (auto turret : gameArena.GetTurrets()) {
		sf::Sprite turretSprite;
		turretSprite.setTexture(GetTexture("turret"));
		turretSprite.setColor(baseColor);
		DrawGameObjectWithSprite(*turret, turretSprite);
	}

	/* BULLETS */
	sf::Sprite playerBulletSprite, enemyBulletSprite;
	playerBulletSprite.setTexture(GetTexture("playerBullet"));
	enemyBulletSprite.setTexture(GetTexture("enemyBullet"));
	for (const Bullet* pBullet : bulletPool.GetBullets()) {
		if(pBullet->GetTag() == "playerBullet")
			DrawGameObjectWithSprite(*pBullet, playerBulletSprite);
		else
			DrawGameObjectWithSprite(*pBullet, enemyBulletSprite);
	}

	//DrawColliders(GetGameObjects());

	if (gameState == "menu") {
		sf::Text titleText = CreateText("ENCIRCLEMENT", 42, sf::Color::Yellow, true, Vector2(center.x, center.y - 100.0f));
		sf::Text infoText1 = CreateText("left click to start", 24, sf::Color::White, true, Vector2(center.x, center.y - 50.0f));
		sf::Text infoText2 = CreateText("scroll to zoom", 16, sf::Color::White, false, Vector2(center.x, center.y + 20.0f));
		sf::Text infoText3 = CreateText("right click to reset zoom", 16, sf::Color::White, false, Vector2(center.x, center.y + 50.0f));

		baseColor = sf::Color(61, 61, 61);

		window.draw(titleText);
		window.draw(infoText1);
		window.draw(infoText2);
		window.draw(infoText3);
	}
	else if (gameState == "gameOver") {
		baseColor = greyColor;

		sf::Text gameOverText = CreateText("GAME OVER", 42, sf::Color::Red, true, Vector2(center.x, center.y - 100.0f));
		window.draw(gameOverText);
	}
	window.display();
}


float DeltaTime() {
	return deltaClock.restart().asSeconds();
}

void LoadTextures() {
	size_t count = 0U;
	for (auto itr = textureFilePathMap.begin(); itr != textureFilePathMap.end(); itr++) {
		const string& key = itr->first,
			filePath = itr->second;
		textures[key] = std::make_shared<sf::Texture>(sf::Texture());
		if (!textures[key]->loadFromFile(filePath)) {
			std::cout << "error while loading texture for <" << key << ">\n";
			continue;
		}
		textures[key]->setSmooth(true);
		count++;
	}
	cout << "Loaded " << count << " textures." << endl;
}
void LoadFonts() {
	size_t count = 0U;
	for (auto itr = fontFilePathMap.begin(); itr != fontFilePathMap.end(); itr++) {
		const string& key = itr->first,
			filePath = itr->second;
		fonts[key] = std::make_shared<sf::Font>(sf::Font());
		if (!fonts[key]->loadFromFile(filePath)) {
			std::cout << "error while loading font for <" << key << ">\n";
			continue;
		}
		count++;
	}
	cout << "Loaded " << count << " fonts." << endl;
}
void LoadSounds() {
	size_t count = 0U;
	for (auto itr = soundFilePathMap.begin(); itr != soundFilePathMap.end(); itr++) {
		const string& key = itr->first,
			filePath = itr->second;
		sounds[key] = std::make_shared<sf::SoundBuffer>(sf::SoundBuffer());
		if (!sounds[key]->loadFromFile(filePath)) {
			std::cout << "error while loading sound for <" << key << ">\n";
			continue;
		}
		count++;
	}
	cout << "Loaded " << count << " sounds." << endl;
}
sf::Texture& GetTexture(string name) {
	if (textures.count(name) > 0) {
		return *textures[name].get();
	}
	return *textures["error"].get();
}
sf::Font& GetFont(string name) {
	if (fonts.count(name) > 0) {
		return *fonts[name].get();
	}
	return *fonts["default"].get();
}
void PlayRequestedSounds() {
	std::queue<string>& soundRequests = GameSoundMaster::GetAllSoundRequests();
	while (!soundRequests.empty()) {
		string soundName = soundRequests.front();
		soundRequests.pop();
		if (sounds.count(soundName) <= 0)
			continue;

		if (soundName == "playerShoot") {
			sound1.setBuffer(*sounds[soundName].get());
			sound1.play();
		}
		else if(soundName == "enemyShoot") {
			sound2.setBuffer(*sounds[soundName].get());
			sound2.play();
		}
		else {
			sound3.setBuffer(*sounds[soundName].get());
			sound3.play();
		}

	}
}
void DrawGameObjectWithSprite(const GameObject& gameObject, sf::Sprite& sprite) {
	if (gameObject.isActive == false)
		return;
	sprite.setOrigin(sprite.getTexture()->getSize().x / 2.f, sprite.getTexture()->getSize().y / 2.f);
	sprite.setPosition(gameObject.transform.position().x, gameObject.transform.position().y);
	sprite.setScale(gameObject.transform.scale().x, gameObject.transform.scale().y);
	sprite.setRotation(gameObject.transform.GlobalRotation());
	window.draw(sprite);
}

void DrawColliders(const std::vector<GameObject*>& gameObjects) {
	for (const GameObject* pObj : gameObjects) {
		if (pObj == nullptr || pObj->isActive == false)
			continue;
		const Collider* pCollider = pObj->GetCollider();
		if (pCollider == nullptr || pCollider->Enabled() == false)
			continue;

		sf::CircleShape circle(pCollider->GetRadius(), 90U);
		circle.setFillColor(sf::Color(0, 0, 0, 0));
		circle.setOutlineColor(sf::Color(30, 215, 96, 255));
		circle.setOutlineThickness(1.f);
		circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
		Vector2 pos = pObj->transform.position() + pCollider->localPosition();
		circle.setPosition(pos.x, pos.y);
		window.draw(circle);
	}
}

void DrawArena(const Arena& arena) {
	sf::CircleShape circle(arena.GetRadius(), 180U);
	circle.setFillColor(sf::Color(0, 0, 0, 0));
	circle.setOutlineColor(baseColor);
	circle.setOutlineThickness(1.f);
	circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setPosition(arena.transform.position().x, arena.transform.position().y);
	window.draw(circle);
}
void DrawDotAt(Vector2 position, float radius) {
	sf::CircleShape circle(radius, 20U);
	circle.setFillColor(sf::Color(255, 0, 0, 255));
	circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setPosition(position.x, position.y);
	window.draw(circle);
}
void UpdateViewSize(sf::View& view, float size) {
	view.setSize(sf::Vector2f(size, size));
}
sf::Text CreateText(string text, int size, sf::Color color, bool isBold, Vector2 position) {
	sf::Text newText;
	newText.setFont(GetFont("default"));
	newText.setString(text);
	newText.setCharacterSize(size);
	newText.setFillColor(color);
	if(isBold)
		newText.setStyle(sf::Text::Bold);
	newText.setOrigin(newText.getGlobalBounds().width / 2, newText.getGlobalBounds().height / 2);
	newText.setPosition(position.x, position.y);
	return newText;
}