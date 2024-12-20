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
    Position oldplayerpos {0,0};
    Position oldWallPos {0,0};
    bool kamercheck = false;
    for (Entity* entity : entities) {
        if (auto* wall = dynamic_cast<Wall*>(entity)){
            walls.push_back(wall);
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

void Room::removeEntity(Entity* entitieToRemove){
    int i = 0;
    for (Entity* entity : entities) {
        if (auto* p = dynamic_cast<Player*>(entity)) {
            entities.erase(entities.begin() + i);
        }
        i++;
    }
}
