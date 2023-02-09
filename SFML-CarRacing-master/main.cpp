#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <sstream>

#include "Player.h"

int main()
{
    sf::VideoMode vm(1086, 679);

    sf::RenderWindow window(vm, "Highway Racer", sf::Style::Titlebar);

    // Load textures here
    sf::Texture textureRoad;
    textureRoad.loadFromFile("assets/imgs/background.png");

    sf::Texture textureRoadStripe;
    textureRoadStripe.loadFromFile("assets/imgs/roadStripe.png");

    sf::Texture textureRock;
    textureRock.loadFromFile("assets/imgs/rock.png");

    sf::Texture textureCars[16];
    textureCars[0].loadFromFile("assets/imgs/carOne.png");
    textureCars[1].loadFromFile("assets/imgs/carTwo.png");
    textureCars[2].loadFromFile("assets/imgs/carThree.png");
    textureCars[3].loadFromFile("assets/imgs/carFour.png");
    textureCars[4].loadFromFile("assets/imgs/carFive.png");
    textureCars[5].loadFromFile("assets/imgs/carSix.png");
    textureCars[6].loadFromFile("assets/imgs/carSeven.png");
    textureCars[7].loadFromFile("assets/imgs/carEight.png");
    textureCars[8].loadFromFile("assets/imgs/carNine.png");
    textureCars[9].loadFromFile("assets/imgs/carTen.png");
    textureCars[10].loadFromFile("assets/imgs/carEleven.png");
    textureCars[11].loadFromFile("assets/imgs/carTwelve.png");
    textureCars[12].loadFromFile("assets/imgs/carThirteen.png");
    textureCars[13].loadFromFile("assets/imgs/carFourteen.png");
    textureCars[14].loadFromFile("assets/imgs/carFifteen.png");
    textureCars[15].loadFromFile("assets/imgs/carSixteen.png");

    sf::Texture textureTree;
    textureTree.loadFromFile("assets/imgs/tree.png");

    sf::Texture textureSpeedometer;
    textureSpeedometer.loadFromFile("assets/imgs/speedometer.png");

    // set the road up
    sf::Sprite spriteRoad;
    spriteRoad.setTexture(textureRoad);

    sf::Sprite spriteRoadStripe[5];
    for(int i=0; i<5; i++)
    {
        spriteRoadStripe[i].setTexture(textureRoadStripe);
        spriteRoadStripe[i].setPosition(525, i*150);
    }

    // environment details
    //rocks
    const int NUMBEROFROCKS = 2;
    sf::Sprite spriteRock[NUMBEROFROCKS];

    srand(int(time(0)));

    for(int i=0; i<NUMBEROFROCKS; i++)
    {
        spriteRock[i].setTexture(textureRock);
        // spawn the environment details in random locations

        // this makes sure that all the rocks spawn on different sides of the road
        if(i == 0)
        {
            spriteRock[i].setPosition((rand() % 280), (rand() % 900) - 1500);
        }else
        {
            spriteRock[i].setPosition((rand() % 770) + 710, (rand() % 900) - 900);
        }
    }

    // trees
    const int NUMBEROFTREES = 20;
    sf::Sprite spriteTree[NUMBEROFTREES];

    for(int i=0; i<NUMBEROFTREES; i++)
    {
        spriteTree[i].setTexture(textureTree);

        if(i % 2 == 0)
        {
            spriteTree[i].setPosition((rand() % 230), (rand() % 1000) - 500);
        }
        else
        {
            spriteTree[i].setPosition((rand() % 810) + 780, (rand() % 1000) - 500);
        }

        spriteTree[i].setOrigin(94, 102);
        spriteTree[i].setRotation((rand() % 360) + 1);
    }

    // set player car up
    Player player;
    player.setTexture(textureCars[0]);
    player.setPlayerPosition(575, 500);
    player.setCarState(Player::CarState::STOPPED);

    // set up the npc car
    // it will just be one sprite that switches textures
    sf::Sprite spriteNpc;
    int textureChoice = rand() % 15;
    spriteNpc.setTexture(textureCars[textureChoice]);
    spriteNpc.setPosition((rand() % 345) + 344, (rand() % 1) - 2000);

    // set up the HUD
    sf::Sprite spriteSpeedometer;
    spriteSpeedometer.setTexture(textureSpeedometer);
    spriteSpeedometer.setPosition(840, 520);

    // a triangle that will represent the speedometer needle
    sf::ConvexShape dometerNeedle(3);
    dometerNeedle.setPoint(0, sf::Vector2f(0, 10));
    dometerNeedle.setPoint(1, sf::Vector2f(5, 80));
    dometerNeedle.setPoint(2, sf::Vector2f(-5, 80));

    dometerNeedle.setPosition(915, 593);
    dometerNeedle.setFillColor(sf::Color::Red);
    sf::FloatRect dometerBounds = dometerNeedle.getLocalBounds();
    dometerNeedle.setOrigin(dometerBounds.left + dometerBounds.width - 5, dometerBounds.top + dometerBounds.height - 5);
    dometerNeedle.setRotation(225);

    // sounds
    sf::Music introMusic;
    introMusic.openFromFile("assets/sounds/Intro.wav");
    introMusic.setLoop(true);
    introMusic.play();

    // car crash sound
    sf::SoundBuffer crashSoundBuffer;
    crashSoundBuffer.loadFromFile("assets/sounds/Crash.wav");
    
    sf::Sound crashSound;
    crashSound.setBuffer(crashSoundBuffer);

    // car rev sound
    sf::SoundBuffer carRevBuffer;
    carRevBuffer.loadFromFile("assets/sounds/CarRev.wav");
    
    sf::Sound carRevSound;
    carRevSound.setBuffer(carRevBuffer);

    // car rev reverse
    sf::SoundBuffer carRevReverseBuffer;
    carRevReverseBuffer.loadFromFile("assets/sounds/CarRevReverse.wav");

    sf::Sound carRevReverseSound;
    carRevReverseSound.setBuffer(carRevReverseBuffer);

    // car idle
    sf::SoundBuffer carIdleBuffer;
    carIdleBuffer.loadFromFile("assets/sounds/CarIdle.wav");

    sf::Sound carIdleSound;
    carIdleSound.setBuffer(carIdleBuffer);
    carIdleSound.setLoop(true);

    enum class GameState {START, RUNNING, PAUSED, OVER};
    GameState gameState = GameState::START;

    /* in the game, everything around the car moves instead of the car moving
    *  gameSpeed is the speed at which everything moves which can be considered
    * also to be how fast the car will appear to be moving to the player */
    const float MAXGAMESPEED = 1100.0f;
    float gameSpeed = 0.0f;

    sf::Font font;
    font.loadFromFile("assets/fonts/Bungee-Regular.ttf");

    sf::Text message;
    message.setFont(font);
    message.setCharacterSize(50);
    message.setFillColor(sf::Color::White);

    sf::FloatRect messageRect = message.getLocalBounds();

    message.setOrigin(messageRect.left + messageRect.width / 2.0f, messageRect.top + messageRect.height / 2.0f);
    message.setPosition(vm.getDesktopMode().width - (vm.getDesktopMode().width / 1.3), vm.getDesktopMode().height - (vm.getDesktopMode().height / 1.4));
    message.setString("Highway Racer");

    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::White);
    score.setPosition(20, 20);

    sf::Clock clock;

    while(window.isOpen())
    {
        /*****************************
        *   Handle the player's input
        ******************************/

	    sf::Event event;

	    while(window.pollEvent(event))
	    {
	        if(event.type == sf::Event::Closed)
	        {
		        window.close();
	        }else if(event.type == sf::Event::KeyPressed)
	        {
		        if(event.key.code == sf::Keyboard::Enter)
		        {
		            if(gameState == GameState::START || gameState == GameState::PAUSED)
		            {
			            gameState = GameState::RUNNING;
                        if(carRevSound.getStatus() == sf::SoundSource::Status::Paused)
                        {
                            carRevSound.play();
                        }else if(carRevReverseSound.getStatus() == sf::SoundSource::Status::Paused)
                        {
                            carRevReverseSound.play();
                        }
                        introMusic.stop();
		            }else if(gameState == GameState::RUNNING)
		            {
			            gameState = GameState::PAUSED;
                        message.setString("Paused");
                        message.setPosition(vm.getDesktopMode().width - (vm.getDesktopMode().width / 1.45), 
                                            vm.getDesktopMode().height - (vm.getDesktopMode().height / 1.45));
                        if(carRevSound.getStatus() == sf::SoundSource::Status::Playing)
                        {
                            carRevSound.pause();
                        }else if(carRevReverseSound.getStatus() == sf::SoundSource::Status::Playing)
                        {
                            carRevReverseSound.pause();
                        }
		            }else if(gameState == GameState::OVER)
                    {
                        // reset the game
                        player.setPlayerPosition(575, 500);
                        spriteNpc.setPosition((rand() % 345) + 344, (rand() % 1) - 2000);
                        gameSpeed = 0.0f;
                        dometerNeedle.setRotation(225);
                        player.setScore(0);
                        introMusic.play();
                        gameState = GameState::START;
                        message.setString("Highway Racer");
                        message.setPosition(vm.getDesktopMode().width - (vm.getDesktopMode().width / 1.3), 
                                            vm.getDesktopMode().height - (vm.getDesktopMode().height / 1.4));
                    }
		        }
	        }
	    }

	    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	    {
	        window.close();
	    }
	    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameState == GameState::RUNNING)
        {
            if(!(player.getCarState() == Player::CarState::ACCELERATE))
            {
                player.setCarState(Player::CarState::ACCELERATE);
                if(carRevReverseSound.getStatus() == sf::SoundSource::Status::Playing)
                {
                    carRevReverseSound.stop();
                }else if(carIdleSound.getStatus() == sf::SoundSource::Status::Playing)
                {
                    carIdleSound.stop();
                }

                if(gameSpeed > 50)
                {
                    carRevSound.setPlayingOffset(sf::seconds(10));
                }else
                {
                    carRevSound.setPlayingOffset(sf::seconds(0));
                }       

                carRevSound.play();
            }

            // check where we are at in the car rev sound file and 
            // loop it when it gets to a certain time

            if(int(carRevSound.getPlayingOffset().asSeconds()) == 30)
            {
                carRevSound.setPlayingOffset(sf::seconds(20));
            }

            if(gameSpeed < MAXGAMESPEED)
            {
                gameSpeed += 0.55;
            }
        }else if(!(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && gameState == GameState::RUNNING)
        {
            if(player.getCarState() == Player::CarState::ACCELERATE)
            {
                player.setCarState(Player::CarState::DECELERATE);
                if(carRevSound.getStatus() == sf::SoundSource::Status::Playing)
                {
                    carRevSound.stop();
                } 
                carRevReverseSound.play();
            }

            if(gameSpeed > 0.0)
            {
                gameSpeed -= 0.38;
            }
        }

        if(gameSpeed <= 0.0)
        {
            player.setCarState(Player::CarState::STOPPED);
            if(carRevReverseSound.getStatus() == sf::SoundSource::Status::Playing)
            {
                carRevReverseSound.stop();
            }
            
            if(carIdleSound.getStatus() == sf::SoundSource::Status::Stopped)
            {
                carIdleSound.play();
            }
        }   

        // control player car
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && gameState == GameState::RUNNING)
        {
            // make sure that the car can't leave the road
            if(player.getPlayerPosition().x > 370)
            {
                player.setPlayerPosition(player.getPlayerPosition().x - 1.2, player.getPlayerPosition().y);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && gameState == GameState::RUNNING)
        {
            if(player.getPlayerPosition().x < 633)
            {
                player.setPlayerPosition(player.getPlayerPosition().x + 1.2, player.getPlayerPosition().y);
            }
        }



        /*****************************
        *   Update the scene
        ******************************/

	    sf::Time dt = clock.restart();

	    std::stringstream ss;

	    ss << "Score: " << player.getScore();;
	    score.setString(ss.str());

        if(gameState == GameState::RUNNING)
        {

            for(int i=0; i<5; i++)
	        {
		        // road stripes
		        spriteRoadStripe[i].setPosition(spriteRoadStripe[i].getPosition().x, (spriteRoadStripe[i].getPosition().y + (gameSpeed * dt.asSeconds())));

		        /* when the stripe gets off screen, put it back at the top to give the illusion of
		        * endless road */
		        if(spriteRoadStripe[i].getPosition().y > 679)
		        {
		            spriteRoadStripe[i].setPosition(spriteRoadStripe[i].getPosition().x, -70);
		        }
	        }

            // do the same for the environment detail but move them at a slightly slower speed
	        for(int i=0; i<NUMBEROFROCKS; i++)
	        {
		        spriteRock[i].setPosition(spriteRock[i].getPosition().x, (spriteRock[i].getPosition().y + (gameSpeed/1.5 * dt.asSeconds())));

		        if(spriteRock[i].getPosition().y > 679)
		        {
		            if(i == 0)
		            {
			            spriteRock[i].setPosition((rand() % 280), (rand() % -1) - 1000);
		            }
		            else
		            {
			            spriteRock[i].setPosition((rand() % 810) + 780, (rand() % -1) - 500);
		            }
		        }
	        }

	        for(int i=0; i<NUMBEROFTREES; i++)
	        {
		        spriteTree[i].setPosition(spriteTree[i].getPosition().x, (spriteTree[i].getPosition().y + (gameSpeed/1.5 * dt.asSeconds())));

		        if(spriteTree[i].getPosition().y > 800)
		        {
		            if(i == 0)
		            {
			            spriteTree[i].setPosition((rand() % 280), (rand() % -1) - 200);
		            }
		            else
		            {
			            spriteTree[i].setPosition((rand() % 770) + 750, (rand() % -1) - 200);
		            }
		        }
	        }

            // move the npc cars
	        if(player.getCarState() == Player::CarState::ACCELERATE)
	        {
		        spriteNpc.setPosition(spriteNpc.getPosition().x, (spriteNpc.getPosition().y + (gameSpeed * dt.asSeconds())));

                // move the odometer
                if((dometerNeedle.getRotation() >= 0 && dometerNeedle.getRotation() <= 120) || 
                    (dometerNeedle.getRotation() <= 360 && dometerNeedle.getRotation() >= 190))
                {
		            dometerNeedle.setRotation(dometerNeedle.getRotation() + (10 * dt.asSeconds()));
                }

	        }else if(player.getCarState() == Player::CarState::DECELERATE)
	        {   
                if(spriteNpc.getPosition().y > -1000)
                {
	                spriteNpc.setPosition(spriteNpc.getPosition().x, (spriteNpc.getPosition().y - (800 * dt.asSeconds())));
                }

               
                // move the odometer the opposite way
                if((dometerNeedle.getRotation() >= 0 && dometerNeedle.getRotation() <= 121) || 
                    (dometerNeedle.getRotation() <= 360 && dometerNeedle.getRotation() >= 225))
                {
                    dometerNeedle.setRotation(dometerNeedle.getRotation() - (47 * dt.asSeconds()));
                }
	        }

	        // if the we pass the npc, move the npc back to the front of the road
	        if(spriteNpc.getPosition().y > 800)
	        {
		        spriteNpc.setPosition((rand() % 345) + 344, (rand() % 1) - 2000);

	            // set a new texture for the sprite
		        int textureChoice = rand() % 15;
		        spriteNpc.setTexture(textureCars[textureChoice]);

		        // increment player score
		        player.setScore(player.getScore() + 1);
	        }

            // here we are going to get the player boundaries and modify them
            // a little to get a little more realisic collision
            sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();
            sf::FloatRect playerModBounds = sf::FloatRect(playerBounds.left+10,
                                            playerBounds.top+10,
                                            playerBounds.width-10,
                                            playerBounds.height-10);

            // check for collision
            if(spriteNpc.getGlobalBounds().intersects(playerModBounds))
            {
                crashSound.play();
                if(carRevSound.getStatus() == sf::SoundSource::Status::Playing)
                {
                    carRevSound.stop();
                } 
                gameState = GameState::OVER;
                message.setString("Game Over!");
                message.setPosition(vm.getDesktopMode().width - (vm.getDesktopMode().width / 1.4), 
                                            vm.getDesktopMode().height - (vm.getDesktopMode().height / 1.45));
            }
	    }


        /*****************************
        *   Draw the scene
        ******************************/
        // clear what is currently on the screen
        window.clear();

        window.draw(spriteRoad);

        for(int i=0; i<5; i++)
        {
            window.draw(spriteRoadStripe[i]);
        }

        window.draw(player.getSprite());

        for(int i=0; i<NUMBEROFROCKS; i++)
        {
            window.draw(spriteRock[i]);
        }

        for(int i=0; i<NUMBEROFTREES; i++)
        {
            window.draw(spriteTree[i]);
        }

        window.draw(spriteSpeedometer);
        window.draw(dometerNeedle);

        window.draw(spriteNpc);

	    window.draw(score);

        if(gameState == GameState::START || gameState == GameState::PAUSED || gameState == GameState::OVER)
        {
            window.draw(message);
        }

        window.display();
    }
    return 0;
}
