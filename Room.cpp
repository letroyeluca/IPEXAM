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
    vector<Entity*> Enemys;
    bool enemykilled = false;
    Position oldplayerpos {0,0};
    bool kamercheck = false;
    for (Entity* entity : entities) {
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
        if (auto* wall = dynamic_cast<Wall*>(entity)){
            walls.push_back(wall);
        }

        if (auto* enemy = dynamic_cast<Enemy*>(entity)){
            Enemys.push_back(enemy);
        }


    }
    for (Entity* entity : entities) {
        if (auto* weapon = dynamic_cast<Weapon*>(entity)) {
            if (weapon->standsOn(this->playerptr)) {
                this->playerptr->setAttackPower(1);
                this->removeEntity(weapon);
                weapon->update(event);
                entity->update(event);
            }
        }
        if (auto* enemy = dynamic_cast<Enemy*>(entity)){
            if(this->playerptr->standsOn(enemy) && this->playerptr->getAttackPower() == 1){
                this->removeEntity(enemy);

                enemykilled = true;
                entity->update(event);

            }
        }
    }


    for(Entity* entity : walls){
        if(entity->getPosition().x == playerptr->nieuwePos.x && entity->getPosition().y == playerptr->nieuwePos.y){
            playerptr->setPosition(oldplayerpos);

        }
    }

    for(Entity* entity : Enemys){
        if(entity->getPosition().x == playerptr->nieuwePos.x && entity->getPosition().y == playerptr->nieuwePos.y && this->playerptr->getAttackPower() == 0){
            playerptr->setPosition(oldplayerpos);

        }
    }
    //cout << this->playerptr->getPosition().x << "  " << this->playerptr->getPosition().y << endl;

}

const vector<Entity *> &Room::getEntities() const {
    return entities;
}

void Room::addEntity(Entity* entitieToAdd) {
    entities.push_back(entitieToAdd);
}

void Room::removeEntity(Entity* entityToRemove){
    for (int i = 0; i < entities.size(); ++i) {
        if (entities[i] == entityToRemove) {
            entities.erase(entities.begin() + i);
        }
    }
}