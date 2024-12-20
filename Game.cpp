//
// Created by Tim Apers on 29/10/2024.
//

#include "Game.h"
#include "Room.h"
#include "fstream"
#include "iostream"
Game::Game(sf::RenderWindow* window) {
    this->window = window;
}

void Game::update() {
    //setCurrentRoom();

    this->window->clear();
    if (currentRoom) { currentRoom->render(this->window); }
    this->window->display();
    while (window->isOpen()) {
        sf::Event event;

        // Wait for event to fire
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            } else if (event.type == sf::Event::KeyPressed) {
                currentRoom->update(&event);
                setCurrentRoom();
                this->window->clear();
                currentRoom->render(this->window);
                this->window->display();
            }
        }
    }
}

void Game::loadMap(const std::string& filename) {
    const int MAX_GRID_SIZE = 10;
    ifstream map(filename);
    string line;

    vector<vector<string>> kamers;
    vector<string> kamerBuffer;

    auto isRoomValid = [](const vector<string>& room) {
        for (const auto& line : room) {
            if (line.find_first_not_of(' ') != string::npos) {
                return true;
            }
        }
        return false;
    };

    int gridX = 0, gridY = 0;

    if (map.is_open()) {
        while (getline(map, line)) {
            if (line.empty()) {
                kamerBuffer.clear();
                continue;
            }
            kamerBuffer.push_back(line);

            if (kamerBuffer.size() == 7) {
                size_t maxWidth = 0;
                for (const auto& kamerBufferLine : kamerBuffer) {
                    maxWidth = max(maxWidth, kamerBufferLine.length());
                }

                for (size_t colStart = 0; colStart + 7 <= maxWidth; colStart += 7) {
                    vector<string> room;
                    bool isEmptyRoom = true;

                    // Check if this 7x7 section is empty
                    for (const auto& kamerBufferLine : kamerBuffer) {
                        string roomSection;
                        if (colStart + 7 <= kamerBufferLine.length()) {
                            roomSection = kamerBufferLine.substr(colStart, 7);
                            room.push_back(roomSection);
                            if (roomSection.find_first_not_of(' ') != string::npos) {
                                isEmptyRoom = false;
                            }
                        }
                    }

                    if (room.size() == 7) {
                        if (!isEmptyRoom && isRoomValid(room)) {
                            kamers.push_back(room);
                            roomGrid[gridY][gridX] = kamers.size() - 1;
                        } else {
                            // Mark empty room in grid with -1
                            roomGrid[gridY][gridX] = -1;
                        }
                    }
                    gridX++;
                }

                kamerBuffer.clear();
                gridX = 0;
                gridY++;
            }
        }
        map.close();

        // Calculate base positions for each grid cell (including empty spaces)
        const int ROOM_WIDTH = 700;  // 7 tiles * 100 pixels
        const int ROOM_HEIGHT = 700; // 7 tiles * 100 pixels

        for (int indexRooms = 0; indexRooms < kamers.size(); indexRooms++) {
            Room* kamer = new Room;

            // Find this room's position in the grid
            int roomGridY = -1, roomGridX = -1;
            for (int y = 0; y < MAX_GRID_SIZE; y++) {
                for (int x = 0; x < MAX_GRID_SIZE; x++) {
                    if (roomGrid[y][x] == indexRooms) {
                        roomGridY = y;
                        roomGridX = x;
                        break;
                    }
                }
                if (roomGridY != -1) break;
            }

            // Calculate base position for this room based on grid position
            int baseX = roomGridX * ROOM_WIDTH;
            int baseY = roomGridY * ROOM_HEIGHT;

            for (int indexroomsize = 0; indexroomsize < kamers[indexRooms].size(); indexroomsize++) {
                for (int indexline = 0; indexline < kamers[indexRooms][indexroomsize].size(); indexline++) {
                    Entity* entity = nullptr;
                    Player* playerentity = nullptr;
                    switch (kamers[indexRooms][indexroomsize][indexline]) {
                        case '#':
                            entity = new Wall();
                            entity->setSprite("resources/wall.png");
                            break;
                        case '@':
                            entity = new Floor();
                            entity->setSprite("resources/floor.png");

                            playerentity = new Player();
                            playerentity->setSprite("resources/player.png");
                            kamer->playerptr = playerentity;
                            break;
                        case '!':
                            entity = new Weapon();
                            entity->setSprite("resources/weapon.png");
                            break;
                        case '%':
                            entity = new Enemy();
                            entity->setSprite("resources/enemy.png");
                            break;
                        case '_':
                            entity = new Floor();
                            entity->setSprite("resources/floor.png");
                            break;
                    }

                    if (entity != nullptr) {
                        // Use absolute positioning based on grid position
                        int posX = baseX + (indexline * 100);
                        int posY = baseY + (indexroomsize * 100);
                        entity->setPosition({posX, posY});
                        kamer->addEntity(entity);

                        if (playerentity != nullptr) {
                            playerentity->setPosition({posX, posY});
                            kamer->addEntity(playerentity);
                            currentRoom = kamer;
                        }
                    }
                }
            }
            this->rooms.push_back(kamer);
        }
    }

    get<0>(currentRoomMatrix) = 1;
    get<1>(currentRoomMatrix) = 1;
}


void Game::setCurrentRoom() {
    // Controleer of de speler boven de huidige kamergrens zit
    if (currentRoom->playerptr->getPosition().y < (std::get<1>(currentRoomMatrix) * 700)) {
        get<1>(currentRoomMatrix) -= 1;
    } else if (currentRoom->playerptr->getPosition().y >= ((std::get<1>(currentRoomMatrix) + 1) * 700)) {
        get<1>(currentRoomMatrix) += 1;
    } else if (currentRoom->playerptr->getPosition().x < (std::get<0>(currentRoomMatrix) * 700)) {
        get<0>(currentRoomMatrix) -= 1;
    }else if (currentRoom->playerptr->getPosition().x >= ((std::get<0>(currentRoomMatrix) + 1) * 700)) {
        get<0>(currentRoomMatrix) += 1;
    }else {
        return; // Geen verandering nodig
    }

    // Verplaats de speler naar de nieuwe kamer
    Room* oldRoom = currentRoom;
    currentRoom = rooms[roomGrid[get<1>(currentRoomMatrix)][std::get<0>(currentRoomMatrix)]];
    currentRoom->addEntity(oldRoom->playerptr);
    currentRoom->playerptr = oldRoom->playerptr;
    oldRoom->removeEntity(oldRoom->playerptr);
    oldRoom->playerptr = nullptr;
}

