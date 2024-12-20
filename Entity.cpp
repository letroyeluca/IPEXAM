//
// Created by Tim Apers on 29/10/2024.
//

#include "Entity.h"
#include <iostream>
#include "Game.h"
void Entity::setSprite(const std::string& img_path) {
    if (!this->texture.loadFromFile(img_path)) {
        std::cerr << "Failed to load texture" << std::endl;
    }
    this->sprite.setTexture(this->texture);
    sf::Vector2u size = this->texture.getSize();
    float scaleX = 100.0f / size.x;
    float scaleY = 100.0f / size.y;
    this->sprite.setScale(scaleX, scaleY);
}

void Entity::render(sf::RenderWindow *window) {
    sprite.setPosition(position.x%700, position.y%700);
    window->draw(sprite);
}

void Entity::update(sf::Event* event) {

}

const Position &Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const Position &position) {
    Position oudePos =  Entity::position;
    Entity::position = position;

}

void Player::update(sf::Event* event) {
    switch (event->key.code) {

        case sf::Keyboard::Left:
            // Move to the left
            nieuwePos.x = (Player::getPosition().x - 100);
            nieuwePos.y = (Player::getPosition().y);
            Player::setPosition(nieuwePos);

                break;
        case sf::Keyboard::Right:
            // Move to the right
            nieuwePos.x = (Player::getPosition().x + 100);
            nieuwePos.y = (Player::getPosition().y);
            Player::setPosition(nieuwePos);
                break;
        case sf::Keyboard::Up:
            // Move up
            nieuwePos.x = (Player::getPosition().x);
            nieuwePos.y = (Player::getPosition().y- 100);
            Player::setPosition(nieuwePos);
                break;
        case sf::Keyboard::Down:
            // Move down
            nieuwePos.x = (Player::getPosition().x);
            nieuwePos.y = (Player::getPosition().y+ 100);
            Player::setPosition(nieuwePos);
                break;
        default: break;
    }
}

int Player::getAttackPower() const {
    return AttackPower;
}

void Player::setAttackPower(int AttackPower) {
    Player::AttackPower = AttackPower;
}

bool Entity::standsOn(Entity* entityToCheck) {
    if(Entity::getPosition().x == entityToCheck->getPosition().x && Entity::getPosition().y == entityToCheck->getPosition().y){
        return true;
    }else{
        return false;
    }
}
