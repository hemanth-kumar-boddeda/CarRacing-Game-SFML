#include "Player.h"

int Player::getScore()
{
    return m_PlayerScore;
}

void Player::setScore(int score)
{
    m_PlayerScore = score;
}

sf::Sprite Player::getSprite()
{
    return m_PlayerSprite;
}

void Player::setTexture(sf::Texture texture)
{
    m_PlayerTexture = texture;
    m_PlayerSprite.setTexture(m_PlayerTexture);
}

void Player::setCarState(CarState state)
{
    m_CarState = state;
}

Player::CarState Player::getCarState()
{
    return m_CarState;
}

void Player::setPlayerPosition(float x, float y)
{
    m_PlayerSprite.setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Player::getPlayerPosition()
{
    return m_PlayerSprite.getPosition();
}
