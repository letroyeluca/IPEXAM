//
// Created by Tim Apers on 29/10/2024.
//

#ifndef GAME_H
#define GAME_H

#include "Room.h"
#include <iostream>
#include <tuple>
using namespace std;


class Game {
public:
    // Deze functies niet aanpassen!
    Game(sf::RenderWindow* window);
    void update();
    std::vector<Room *> getRooms() { return rooms; }
    Room* getCurrentRoom() { return currentRoom; }

    // Deze functies zal je zelf verder moeten aanvullen
    void loadMap(const std::string& filename);
    void setCurrentRoom();
    tuple<int, int> currentRoomMatrix;
    vector<vector<int>> roomGrid = vector<vector<int>>(10, vector<int>(10, -1));
    void PlayerMoveRoom();

private:
    sf::RenderWindow* window;
    std::vector<Room*> rooms;
    Room* currentRoom = nullptr;
};



#endif //GAME_H
