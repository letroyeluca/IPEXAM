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
    vector<Entity*> Weapons;
    bool enemykilled = false;
    Position oldplayerpos {0,0};
    bool kamercheck = false;
    bool update = false;

    //loop door alle elementen in de kamer
    for (Entity* entity : entities) {

        // als er een player tegen komt
        if (auto* player = dynamic_cast<Player*>(entity)){

            // controlleer of deze loop als is gepasseerd
            if (!kamercheck) {

                //slaag de oude positie van de player op
                oldplayerpos = player->getPosition();

                //update de player met de nieuwe positie
                playerptr = player;
                player->update(event);
                kamercheck = true;
            }else{
                entity->update(event);
            }
        }

        //als er een muur word gevonden slaag die op in de lijst van muren
        if (auto* wall = dynamic_cast<Wall*>(entity)){
            walls.push_back(wall);
        }

        // als er vijand word gevonden slaag die op in de lijst van vijanden
        if (auto* enemy = dynamic_cast<Enemy*>(entity)){
            Enemys.push_back(enemy);
        }

        // als het een wapen vind zet die op de lijst met wapens
        if (auto* weapon = dynamic_cast<Weapon*>(entity)) {
            Weapons.push_back(weapon);
        }
    }


    for(Entity* weapon: Weapons){
        if (this->playerptr->standsOn(weapon)) {
            this->playerptr->setAttackPower(1);
            this->removeEntity(weapon);
            weapon->update(event);
        }
    }

    for(Entity* entity : walls){
        if(entity->getPosition().x == playerptr->nieuwePos.x && entity->getPosition().y == playerptr->nieuwePos.y){
            playerptr->setPosition(oldplayerpos);

        }
    }

    for(Entity* entity : Enemys){
        // als de speler op de vijand gaat staan en het zwaard heeft opgepakt, verslaag de vijand
        if(this->playerptr->standsOn(entity) && this->playerptr->getAttackPower() == 1){
            this->removeEntity(entity);
            enemykilled = true;
            update = true;
            entity->update(event);
        }

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