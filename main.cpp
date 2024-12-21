//
//  !!! Dit bestand moet je in principe NIET wijzigen !!!
//  !!!       Enkel de filename mag je aanpassen      !!!
//

#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(700, 700), "Game");
    Game game = Game(&window);
    game.loadMap("resources/map.txt");
    sf::Event event = sf::Event();
    event.key.code = sf::Keyboard::Up;
    game.setCurrentRoom();
    Room* room = game.getCurrentRoom();
    game.renderRoom();
    for (int i = 0; i < 9; i++) {
        room->update(&event);
        game.setCurrentRoom();
        room = game.getCurrentRoom();
    }
    game.renderRoom();
    event.key.code = sf::Keyboard::Down;
    for (int i = 0; i < 8; i++) {
        room->update(&event);
        game.setCurrentRoom();
        room = game.getCurrentRoom();
    }
    event.key.code = sf::Keyboard::Right;
    for (int i = 0; i < 7; i++) {
        room->update(&event);
        game.setCurrentRoom();
        room = game.getCurrentRoom();
    }
    event.key.code = sf::Keyboard::Left;
    room->update(&event);
    game.setCurrentRoom();
    room = game.getCurrentRoom();
    game.renderRoom();
    for (int i = 0; i < 6; i++) {
        room->update(&event);
        game.setCurrentRoom();
        room = game.getCurrentRoom();
    }
    event.key.code = sf::Keyboard::Up;
    for (int i = 0; i < 9; i++) {
        room->update(&event);
        game.setCurrentRoom();
        room = game.getCurrentRoom();
    }
    game.renderRoom();
    return 0;
}
