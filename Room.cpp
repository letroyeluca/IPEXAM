//
// Created by Tim Apers on 29/10/2024.
//

#include "Room.h"
#include "Entity.h"

void Room::render(sf::RenderWindow* window) const {
    Player* player = nullptr;
    std::vector<Entity*> others = {};
    for (Entity* entity : entities) {
        if (auto* floor = dynamic_cast<Floor*>(entity)) {floor->render(window);}
        else if (auto* p = dynamic_cast<Player*>(entity)) { player = p;}
        else { others.push_back(entity); }
    }
    for (Entity* e : others) { e->render(window); }
    if (player) { player->render(window);}
}

void Room::update(sf::Event * event) {
    vector<Entity*> walls;
    vector<Entity*> weapons;
    vector<Entity*> enemys;
    Position oldplayerpos {0,0};
    Position oldWallPos {0,0};
    int index = 0;
    bool kamercheck = false;
    for (Entity* entity : entities) {
        if (auto* wall = dynamic_cast<Wall*>(entity)){
            walls.push_back(wall);
        }
        if (auto* weapon = dynamic_cast<Weapon*>(entity)) {
            if (this->playerptr->standsOn(weapon)) {
                this->playerptr->setAttackPower(1);
                this->removeEntity(weapon);  // Remove the weapon instead of the player
                
            }
        }
        if (auto* enemy = dynamic_cast<Enemy*>(entity)){
            if(this->playerptr->standsOn(enemy) && this->playerptr->getAttackPower() == 1){
                this->removeEntity(enemy);
            }
        }
        if (auto* player = dynamic_cast<Player*>(entity)){
            if (!kamercheck) {
                oldplayerpos = player->getPosition();
                playerptr = player;
                player->update(event);
                kamercheck = true;
            }else{
                entity->update(event);
            }

        }
        index++;
    }

    for(Entity* entity : walls){
        std::cout << "Entity position (x: " << entity->getPosition().x
                  << ", y: " << entity->getPosition().y << ") "
                  << (entity->getPosition().x == playerptr->nieuwePos.x && entity->getPosition().y == playerptr->nieuwePos.y
                      ? "matches" : "does not match")
                  << " player position (x: " << playerptr->nieuwePos.x
                  << ", y: " << playerptr->nieuwePos.y << ")." << std::endl;

        if(entity->getPosition().x == playerptr->nieuwePos.x && entity->getPosition().y == playerptr->nieuwePos.y){
            playerptr->setPosition(oldplayerpos);
            cout << "(MUUR) ";
        }
    }
    cout << "X:("<<playerptr->getPosition().x << ") Y:(" << playerptr->getPosition().y << ")"<< endl;
}

const vector<Entity *> &Room::getEntities() const {
    return entities;
}

void Room::addEntity(Entity* entitieToAdd) {
    entities.push_back(entitieToAdd);
}

void Room::removeEntity(Entity* entityToRemove) {
    auto it = std::find(entities.begin(), entities.end(), entityToRemove);
    if (it != entities.end()) {
        entities.erase(it);
    }
}