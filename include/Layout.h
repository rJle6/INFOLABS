#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Button.h"

class Layout : public sf::Drawable {
protected:
    std::vector<std::shared_ptr<Button>> m_buttons;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    float m_spacing;

public:
    Layout(const sf::Vector2f& position, float spacing = 10.f);
    virtual void addButton(std::shared_ptr<Button> button) = 0;
    virtual void updateLayout() = 0;
    void update(const sf::RenderWindow& window);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(const sf::Vector2f& position);
    void setSpacing(float spacing);
};

class HLayout : public Layout {
public:
    HLayout(const sf::Vector2f& position, float spacing = 10.f);
    virtual void addButton(std::shared_ptr<Button> button) override;
    virtual void updateLayout() override;
};

class VLayout : public Layout {
public:
    VLayout(const sf::Vector2f& position, float spacing = 10.f);
    virtual void addButton(std::shared_ptr<Button> button) override;
    virtual void updateLayout() override;
};