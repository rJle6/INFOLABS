#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <memory>

class Button : public sf::Drawable {
private:
    sf::RectangleShape m_shape;
    std::unique_ptr<sf::Text> m_text;  // Используем указатель
    sf::Font m_font;
    sf::Color m_normalColor, m_hoverColor, m_pressColor, m_currentColor;
    std::function<void()> m_onClick;
    bool m_isHovered, m_isPressed;

public:
    Button(const std::string& text, const sf::Vector2f& size, const sf::Vector2f& position);
    void setOnClick(std::function<void()> onClick);
    void update(const sf::RenderWindow& window);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
};