#pragma once

#include <SFML/Graphics.hpp>

class Player
{
    public:
        enum CarState{ACCELERATE, DECELERATE, STOPPED};
        int getScore();
        void setScore(int score);
        sf::Sprite getSprite();
        void setTexture(sf::Texture texture);
        void setCarState(CarState state);
        CarState getCarState();
        sf::Vector2f getPlayerPosition();
        void setPlayerPosition(float x, float y);

    private:
        int m_PlayerScore = 0;
        sf::Sprite m_PlayerSprite;
        CarState m_CarState;
        sf::Texture m_PlayerTexture;
};
