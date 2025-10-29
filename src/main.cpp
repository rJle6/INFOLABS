#include "../include/Button.h"
#include "../include/Layout.h"
#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    // SFML 3.0 использует Vector2u
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Buttons");
    
    auto button1 = std::make_shared<Button>("Start Game", sf::Vector2f(200, 60), sf::Vector2f(0, 0));
    auto button2 = std::make_shared<Button>("Settings", sf::Vector2f(200, 60), sf::Vector2f(0, 0));
    auto button3 = std::make_shared<Button>("Exit", sf::Vector2f(200, 60), sf::Vector2f(0, 0));
    
    button1->setOnClick([]() { std::cout << "Start Game clicked!" << std::endl; });
    button2->setOnClick([]() { std::cout << "Settings clicked!" << std::endl; });
    button3->setOnClick([]() { std::cout << "Exit clicked!" << std::endl; });
    
    HLayout horizontalLayout(sf::Vector2f(50, 200), 25.f);
    horizontalLayout.addButton(button1);
    horizontalLayout.addButton(button2);
    horizontalLayout.addButton(button3);
    
    while (window.isOpen()) {
        horizontalLayout.update(window);
        
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        window.clear(sf::Color(40, 40, 60));
        window.draw(horizontalLayout);
        window.display();
    }
    
    return 0;
}