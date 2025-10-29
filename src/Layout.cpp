#include "../include/Layout.h"
#include <algorithm>

Layout::Layout(const sf::Vector2f& position, float spacing) 
    : m_position(position), m_spacing(spacing) {}

void Layout::update(const sf::RenderWindow& window) {
    for (auto& button : m_buttons) {
        button->update(window);
    }
}

void Layout::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& button : m_buttons) {
        target.draw(*button, states);
    }
}

void Layout::setPosition(const sf::Vector2f& position) {
    m_position = position;
    updateLayout();
}

void Layout::setSpacing(float spacing) {
    m_spacing = spacing;
    updateLayout();
}

HLayout::HLayout(const sf::Vector2f& position, float spacing) 
    : Layout(position, spacing) {}

void HLayout::addButton(std::shared_ptr<Button> button) {
    m_buttons.push_back(button);
    updateLayout();
}

void HLayout::updateLayout() {
    float currentX = m_position.x;
    float maxHeight = 0.f;
    
    for (auto& button : m_buttons) {
        button->setPosition(sf::Vector2f(currentX, m_position.y));
        currentX += button->getSize().x + m_spacing;
        maxHeight = std::max(maxHeight, button->getSize().y);
    }
    
    m_size = sf::Vector2f(currentX - m_position.x - m_spacing, maxHeight);
}

VLayout::VLayout(const sf::Vector2f& position, float spacing) 
    : Layout(position, spacing) {}

void VLayout::addButton(std::shared_ptr<Button> button) {
    m_buttons.push_back(button);
    updateLayout();
}

void VLayout::updateLayout() {
    float currentY = m_position.y;
    float maxWidth = 0.f;
    
    for (auto& button : m_buttons) {
        button->setPosition(sf::Vector2f(m_position.x, currentY));
        currentY += button->getSize().y + m_spacing;
        maxWidth = std::max(maxWidth, button->getSize().x);
    }
    
    m_size = sf::Vector2f(maxWidth, currentY - m_position.y - m_spacing);
}