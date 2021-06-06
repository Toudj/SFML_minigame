// Game1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Hero.h"
#include "Enemy.h"
#include "Rocket.h"


sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "Ninjame",
    sf::Style::Default);

sf::Texture skyTexture;
sf::Texture bgTexture;
sf::Sprite  skySprite;
sf::Sprite  bgSprite;

//Add Hero
Hero hero;
//Add Enemy
std::vector<Enemy*> enemies;
//Add Rocket
std::vector<Rocket*> rockets;

float currentTime;
float prevTime = 0.0f;

//Event 
sf::Event event;

//Payer position


bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2)
{
    sf::FloatRect shape1 = sprite1.getGlobalBounds();
    sf::FloatRect shape2 = sprite1.getGlobalBounds();
    if (shape1.intersects(shape2))
    {
        return true;
    }
    else
    {
        return false;
    }
}
void shoot()
{
    Rocket* rocket = new Rocket();
    rocket->init("Assets/graphics/rocket.png",
        hero.getSprite().getPosition(), 40.0f);
    rockets.push_back(rocket);
   
}

void spawnEnemy()
{
    int randLoc = rand() % 3;

    sf::Vector2f enemyPos;
    float speed;

    switch (randLoc)
    {
    case 0:
        enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.75f);
        speed = -50;
        break;
    case 1:
        enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f);
        speed = -50;
        break;
    case 2:
        enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f);
        speed = -50;
        break;
    default:
        std::cout << "Incorrect y value" << std::endl;
        return;
    }
    Enemy* enemy = new Enemy();
    enemy->init("Assets/graphics/enemy.png", enemyPos, speed);
    enemies.push_back(enemy);
}
void Init()
{
    //Load sky Texture
    if (!skyTexture.loadFromFile("Assets/graphics/sky.png"))
    {    //error    
    }
    //Set and attach Sky Texture to sky sprite

    skySprite.setTexture(skyTexture);

    //Load background Texture

    if (!bgTexture.loadFromFile("Assets/graphics/bg.png"))
    {//error
    }
    //Set and attach background Texture to background sprite

    bgSprite.setTexture(bgTexture);

    //Init player with Texture, sprite and position
    hero.init("Assets/graphics/hero.png",
        sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f), 200.0f);
    
    srand((int)time(0));
    
    
    //heroSprite.setTexture(heroTexture);
    //heroSprite.setPosition(sf::Vector2f(viewSize.x / 2, viewSize.y / 2));
    //heroSprite.setOrigin(heroTexture.getSize().x / 2, heroTexture.getSize().y / 2);

}

void updateInput()
{

    while (window.pollEvent(event))
    {

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up)
            {
                hero.jump(750.0f);
                std::cout << "Up" << std::endl;
            }
            
            if (event.key.code == sf::Keyboard::Key::Escape)
            {
                window.close();
            }
                
            if (event.key.code == sf::Keyboard::Right)
            {
                std::cout << "Right" << std::endl;
            }   

            if (event.key.code == sf::Keyboard::Down)
            {
                shoot();
            }
                
        }

        //if (event.type == sf::Event::KeyReleased)
        //{
        //    if (event.key.code == sf::Keyboard::Right)
        //        //playerMoving = false;
        //}

    }


}


void update(float dt)
{
    hero.update(dt);
    currentTime += dt;
    //Spawn Enemies
    if (currentTime >= prevTime + 1.125f)
    {
        spawnEnemy();
        prevTime = currentTime;
    }

    //Update Enemies
    for (int i = 0; i < enemies.size(); i++)
    {
        Enemy* enemy = enemies[i];
        enemy->update(dt);
        if (enemy->getSprite().getPosition().x < 0)
        {
            enemies.erase(enemies.begin() + i);
            delete(enemy);
        }
    }

    //update Rockets
    for (int i = 0; i < rockets.size(); i++)
    {
        Rocket* rocket = rockets[i];
        rocket->update(dt);
        if (rocket->getSprite().getPosition().x > viewSize.x)
        {
            
            rockets.erase(rockets.begin() + i);
            delete(rocket);
        }
        
    }
    //Check Collision between Rocket and Enemies
    for (int i = 0; i < rockets.size(); i++)
    {
        for (int j = 0; j < enemies.size(); j++)
        {
            Rocket* rocket = rockets[i];
            Enemy* enemy = enemies[j];
           
            if (checkCollision(rocket->getSprite(), enemy->getSprite()))
            {

                //rockets.erase(rockets.begin() + i);
                //enemies.erase(enemies.begin() + j);
                //delete(rocket);
                //delete(enemy);
                std::cout << "Enemy" << std::endl;
            }
        }
    }


 
}
void draw()
{
    window.draw(skySprite);
    window.draw(bgSprite);
    //window.draw(heroSprite);

    window.draw(hero.getSprite());
    for (Enemy* enemy : enemies)
    {
        window.draw(enemy->getSprite());
    }
    for (Rocket* rocket : rockets)
    {
        window.draw(rocket->getSprite());
    }
}

int main()
{
    sf::Clock clock;
    window.setFramerateLimit(60);
    Init();

    while (window.isOpen())
    {

        updateInput();
        sf::Time dt = clock.restart();
        update(dt.asSeconds());
        window.clear();
        draw();
        window.display();
    }

    return 0;
}
