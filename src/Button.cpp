#include "../include/Button.h"
#include <iostream>

Button::Button(const std::string& text, const sf::Vector2f& size, const sf::Vector2f& position) 
    : m_isHovered(false), m_isPressed(false), m_text(nullptr) {
    
    m_shape.setSize(size);
    m_shape.setPosition(position);
    
    m_normalColor = sf::Color(70, 70, 70);
    m_hoverColor = sf::Color(100, 100, 100);
    m_pressColor = sf::Color(50, 50, 50);
    m_currentColor = m_normalColor;
    m_shape.setFillColor(m_currentColor);
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2.f);
    
    if (!m_font.openFromFile("fonts/arial.ttf")) {
        std::cerr << "Error: Could not load font from fonts/arial.ttf" << std::endl;
        return;
    }
    
    // Создаем текст после загрузки шрифта
    m_text = std::make_unique<sf::Text>(m_font, text, 20);
    m_text->setFillColor(sf::Color::White);
    
    // SFML 3.0: используем position и size
    sf::FloatRect textBounds = m_text->getLocalBounds();
    m_text->setOrigin(textBounds.position + textBounds.size / 2.f);
    m_text->setPosition(position + size / 2.f);
}

void Button::setOnClick(std::function<void()> onClick) {
    m_onClick = onClick;
}

void Button::update(const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    m_isHovered = m_shape.getGlobalBounds().contains(mousePos);
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (m_isHovered) {
            m_isPressed = true;
        }
    } else {
        if (m_isPressed && m_isHovered) {
            if (m_onClick) m_onClick();
        }
        m_isPressed = false;
    }
    
    if (m_isPressed && m_isHovered) m_currentColor = m_pressColor;
    else if (m_isHovered) m_currentColor = m_hoverColor;
    else m_currentColor = m_normalColor;
    
    m_shape.setFillColor(m_currentColor);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_shape, states);
    if (m_text) {
        target.draw(*m_text, states);
    }
}

void Button::setPosition(const sf::Vector2f& position) {
    m_shape.setPosition(position);
    sf::Vector2f size = m_shape.getSize();
    if (m_text) {
        m_text->setPosition(position + size / 2.f);
    }
}

sf::Vector2f Button::getPosition() const { return m_shape.getPosition(); }
sf::Vector2f Button::getSize() const { return m_shape.getSize(); }