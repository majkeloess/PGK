// Uwaga! Co najmniej C++17!!!
// Project-> ... Properties->Configuration Properties->General->C++ Language Standard == ISO C++ 17 Standard (/std:c++17)

#include "SFML/Graphics.hpp"
#include "ShareTechMono-Regular.h"
#include <numeric>
#include <iostream>
#include <memory>
#include <cmath>

std::shared_ptr<sf::Font> font{};

class hexagon : public sf::Drawable
{
protected:
    sf::Vector2f left_top, right_bottom, center;
    sf::Vector2f p[6]; // Kolejność punktów opisana w pliku PDF do laboratorium.
public:
    void Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom);
    void Draw_Border(sf::RenderTarget &target, sf::RenderStates states, sf::String name) const;
    bool rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float &alpha, float &beta) const;

private:
    float d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const;
};

float hexagon::d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const
{
    float A = p0.y - p1.y;
    float B = p1.x - p0.x;
    float C = p0.x * (p1.y - p0.y) - p0.y * (p1.x - p0.x);

    return 2 * ((A * p.x + B * p.y + C) / sqrt(A * A + B * B)) / sqrt(3.0f);
}

bool hexagon::rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float &alpha, float &beta) const
{
    float L = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
    sf::Vector2f p3(0.5f * (p0.x - p1.x) + 0.5f * sqrt(3.0f) * (p1.y - p0.y) + p0.x, 0.5f * sqrt(3.0f) * (p0.x - p1.x) - 0.5f * (p1.y - p0.y) + p0.y);

    beta = -d_coefficient(p0, p1, p) / L;
    alpha = d_coefficient(p0, p3, p) / L;

    if ((alpha > 1.0f) || (beta > 1.0f) || (alpha < 0.0f) || (beta < 0.0f))
        return false;
    else
        return true;
}

void hexagon::Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom)
{
    left_top = _left_top;
    right_bottom = _right_bottom;
    float a = ((right_bottom.y - left_top.y) / 2.0f + 0.5f) - 20.0f;
    center = left_top + sf::Vector2f((right_bottom.x - left_top.x) / 2.0f + 0.5f, (right_bottom.y - left_top.y) / 2.0f + 0.5f + 10);

    p[0] = center - sf::Vector2f(0.0f, a);
    p[1] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[2] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
    p[3] = center + sf::Vector2f(0.0f, a);
    p[4] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[5] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
}

void hexagon::Draw_Border(sf::RenderTarget &target, sf::RenderStates states, sf::String name) const
{

    sf::ConvexShape hexagonShape(6);

    for (int i = 0; i < 6; ++i)
    {
        hexagonShape.setPoint(i, p[i]);
    }

    hexagonShape.setFillColor(sf::Color::Transparent);
    hexagonShape.setOutlineColor(sf::Color::Black);
    hexagonShape.setOutlineThickness(2);

    target.draw(hexagonShape, states);
}

class hexagon_RGB : public hexagon
{
public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    float Black;
    void update(float slide);
};

void hexagon_RGB::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Vector2f mid = (p[3] + p[0]) / 2.f;

    sf::Vertex rhombus1[4] = {p[0], p[1], mid, p[5]};
    sf::Vertex rhombus2[4] = {mid, p[1], p[2], p[3]};
    sf::Vertex rhombus3[4] = {p[5], mid, p[3], p[4]};

    float grey = Black;

    rhombus1[0].color = sf::Color(255 * grey, 0, 0);
    rhombus1[1].color = sf::Color(255 * grey, 255 * grey, 0);
    rhombus1[2].color = sf::Color(255 * grey, 255 * grey, 255 * grey);
    rhombus1[3].color = sf::Color(255 * grey, 0, 255 * grey);

    rhombus2[0].color = sf::Color(255 * grey, 255 * grey, 255 * grey);
    rhombus2[1].color = sf::Color(255 * grey, 255 * grey, 0);
    rhombus2[2].color = sf::Color(0, 255 * grey, 0);
    rhombus2[3].color = sf::Color(0, 255 * grey, 255 * grey);

    rhombus3[0].color = sf::Color(255 * grey, 0, 255 * grey);
    rhombus3[1].color = sf::Color(255 * grey, 255 * grey, 255 * grey);
    rhombus3[2].color = sf::Color(0, 255 * grey, 255 * grey);
    rhombus3[3].color = sf::Color(0, 0, 255 * grey);

    target.draw(rhombus1, 4, sf::Quads);
    target.draw(rhombus2, 4, sf::Quads);
    target.draw(rhombus3, 4, sf::Quads);

    Draw_Border(target, states, "RGB");
}

void hexagon_RGB::update(float slide)
{
    Black = slide;
}

class hexagon_CMY : public hexagon
{
public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    float BlackCMY;
    void update(float slide);
};

void hexagon_CMY::update(float slide)
{
    BlackCMY = slide;
}

void hexagon_CMY::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Vector2f mid = (p[3] + p[0]) / 2.f;

    sf::Vertex rhombus1[4] = {p[0], p[1], mid, p[5]};
    sf::Vertex rhombus2[4] = {mid, p[1], p[2], p[3]};
    sf::Vertex rhombus3[4] = {p[5], mid, p[3], p[4]};

    float grey = BlackCMY;

    rhombus1[0].color = sf::Color(255 * (1 - grey), 255, 255); // Cyan
    rhombus1[1].color = sf::Color(255 * (1 - grey), 255 * (1 - grey), 255);
    rhombus1[2].color = sf::Color(255 * (1 - grey), 255 * (1 - grey), 255 * (1 - grey));
    rhombus1[3].color = sf::Color(255 * (1 - grey), 255, 255 * (1 - grey));

    rhombus2[0].color = sf::Color(255 * (1 - grey), 255 * (1 - grey), 255 * (1 - grey));
    rhombus2[1].color = sf::Color(255 * (1 - grey), 255 * (1 - grey), 255);
    rhombus2[2].color = sf::Color(255, 255 * (1 - grey), 255 * (1 - grey));
    rhombus2[3].color = sf::Color(255, 255 * (1 - grey), 255);

    rhombus3[0].color = sf::Color(255 * (1 - grey), 255, 255 * (1 - grey));
    rhombus3[1].color = sf::Color(255 * (1 - grey), 255 * (1 - grey), 255 * (1 - grey));
    rhombus3[2].color = sf::Color(255, 255 * (1 - grey), 255);
    rhombus3[3].color = sf::Color(255, 255, 255 * (1 - grey));

    target.draw(rhombus1, 4, sf::Quads);
    target.draw(rhombus2, 4, sf::Quads);
    target.draw(rhombus3, 4, sf::Quads);

    Draw_Border(target, states, "CMY");
}

float hue_to_rgb(float p, float q, float t)
{
    if (t < 0.0f)
        t += 1.0f;
    if (t > 1.0f)
        t -= 1.0f;
    if (t < 1.0f / 6.0f)
        return p + (q - p) * 6.0f * t;
    if (t < 1.0f / 2.0f)
        return q;
    if (t < 2.0f / 3.0f)
        return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
}

sf::Color hsl2rgb(float h, float s, float l)
{
    float r, g, b;

    if (s == 0.0f)
    {
        r = g = b = l; // achromatic
    }
    else
    {

        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r = hue_to_rgb(p, q, h + 1.0f / 3.0f);
        g = hue_to_rgb(p, q, h);
        b = hue_to_rgb(p, q, h - 1.0f / 3.0f);
    }

    return sf::Color(static_cast<sf::Uint8>(r * 255), static_cast<sf::Uint8>(g * 255), static_cast<sf::Uint8>(b * 255));
}

class hexagon_HSL : public hexagon
{
public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void update(float slide);
    float Light = 1;
};

void hexagon_HSL::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::VertexArray triangle(sf::Points);
    sf::Vector2f mid = sf::Vector2f((p[2] + p[5]) / 2.f);
    float Lslide = Light;
    float alpha, beta;
    for (float y = 294.0f; y < 588.0f; y++)
    {
        for (float x = 0.0f; x < 300.0f; x++)
        {
            sf::Vector2f point(x, y);
            if (rhombus(p[0], p[1], point, alpha, beta))
            {
                float H = 0;
                float S = 1 * alpha;
                float L = Lslide * beta;
                sf::Color col = hsl2rgb(H, S, L);
                triangle.append(sf::Vertex(point, col));
            }

            if (rhombus(p[2], p[3], point, alpha, beta))
            {
                float H = 0 + beta;
                float S = 1;
                float L = Lslide * alpha;
                sf::Color col = hsl2rgb(H, S, L);
                triangle.append(sf::Vertex(point, col));
            }

            if (rhombus(p[4], p[5], point, alpha, beta))
            {
                float H = 0 + alpha;
                float S = 1 * beta;
                float L = Lslide * 1;
                sf::Color col = hsl2rgb(H, S, L);
                triangle.append(sf::Vertex(point, col));
            }
        }
    }
    target.draw(triangle);

    Draw_Border(target, states, "HSL");
}

void hexagon_HSL::update(float slide)
{
    Light = slide;
}

class hexagon_HSB : public hexagon
{
public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    float Brigthness;
    void update(float slide);
};

sf::Color hsb2rgb(float h, float s, float b)
{
    int i;
    float f, p, q, t;
    float r, g, bl;

    if (s == 0)
    {
        r = g = bl = b;
    }
    else
    {
        h /= 60;
        i = floor(h);
        f = h - i;
        p = b * (1 - s);
        q = b * (1 - s * f);
        t = b * (1 - s * (1 - f));
        switch (i)
        {
        case 0:
            r = b;
            g = t;
            bl = p;
            break;
        case 1:
            r = q;
            g = b;
            bl = p;
            break;
        case 2:
            r = p;
            g = b;
            bl = t;
            break;
        case 3:
            r = p;
            g = q;
            bl = b;
            break;
        case 4:
            r = t;
            g = p;
            bl = b;
            break;
        default:
            r = b;
            g = p;
            bl = q;
            break;
        }
    }

    return sf::Color(static_cast<sf::Uint8>(r * 255), static_cast<sf::Uint8>(g * 255), static_cast<sf::Uint8>(bl * 255));
}

void hexagon_HSB::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::VertexArray triangle(sf::Points);
    sf::Vector2f mid = sf::Vector2f((p[2] + p[5]) / 2.f);
    float Bslide = Brigthness;
    float alpha, beta;
    for (float y = 294.0f; y < 588.0f; y++)
    {
        for (float x = 300.0f; x < 600.0f; x++)
        {
            sf::Vector2f point(x, y);
            if (rhombus(p[0], p[1], point, alpha, beta))
            {
                float H = 0;
                float S = 1 * alpha;
                float B = Bslide * beta;
                sf::Color col = hsb2rgb(H, S, B);
                triangle.append(sf::Vertex(point, col));
            }

            if (rhombus(p[2], p[3], point, alpha, beta))
            {
                float H = 360.0f * beta;
                float S = 1;
                float B = Bslide * alpha;
                sf::Color col = hsb2rgb(H, S, B);
                triangle.append(sf::Vertex(point, col));
            }

            if (rhombus(p[4], p[5], point, alpha, beta))
            {
                float H = 350.0f * alpha;
                float S = 1 * beta;
                float B = Bslide;
                sf::Color col = hsb2rgb(H, S, B);
                triangle.append(sf::Vertex(point, col));
            }
        }
    }

    target.draw(triangle);
    Draw_Border(target, states, "HSB");
}

void hexagon_HSB::update(float slide)
{
    Brigthness = slide;
}

class Scroll : public sf::Drawable
{
public:
    Scroll(float height, float maxValue)
        : bar(sf::Vector2f(40, height / 2)), slider(sf::Vector2f(40, height / 20)), maxValue(maxValue), currentValue(0), speed(160), goingDown(true)
    {
        bar.setFillColor(sf::Color(200, 200, 200));
        slider.setFillColor(sf::Color::Black);
        bar.setPosition(700, height / 4);
        slider.setPosition(bar.getPosition());
    }

    void update(float deltaTime)
    {
        if (goingDown)
        {
            currentValue += speed * deltaTime;
            if (currentValue > maxValue)
            {
                currentValue = maxValue;
                goingDown = false;
            }
        }
        else
        {
            currentValue -= speed * deltaTime;
            if (currentValue < 0)
            {
                currentValue = 0;
                goingDown = true;
            }
        }

        updateSliderPosition();
    }

    void setCurrentValue(float value)
    {
        currentValue = std::clamp(value, 0.f, maxValue);
        updateSliderPosition();
    }

    float getCurrentValue() const
    {
        return currentValue;
    }

    // private:
    sf::RectangleShape bar;
    sf::RectangleShape slider;
    float maxValue;
    float currentValue;
    float speed;
    bool goingDown;

    void updateSliderPosition()
    {
        float barTop = bar.getPosition().y;
        float barHeight = bar.getSize().y;
        float sliderHeight = slider.getSize().y;
        float availableSpace = barHeight - sliderHeight;
        float sliderTop = barTop + (currentValue / maxValue) * availableSpace;
        slider.setPosition(bar.getPosition().x, sliderTop);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(bar, states);
        target.draw(slider, states);
    }
};

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

    hexagon_RGB h_RGB;
    hexagon_CMY h_CMY;
    hexagon_HSL h_HSL;
    hexagon_HSB h_HSB;
    Scroll scrollbar(600, 600);
    bool isDraggingSlider = false;

    sf::Clock frame_clock, around_half_secound_clock;
    sf::Int64 mean_frames_time = 0;
    std::vector<sf::Int64> frame_times;
    sf::Text text;

    font = std::make_shared<sf::Font>();
    font->loadFromMemory(&(ShareTechMono_Regular_ttf[0]), ShareTechMono_Regular_ttf.size());
    text.setFont(*font);
    text.setCharacterSize(21);
    text.setFillColor(sf::Color::Black);

    text.setPosition(10, 10);
    h_RGB.Set_Borders(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(300.0f, 292.0f));
    h_CMY.Set_Borders(sf::Vector2f(300.0f, 0.0f), sf::Vector2f(600.0f, 292.0f));
    h_HSL.Set_Borders(sf::Vector2f(0.0f, 292.0f), sf::Vector2f(300.0f, 584.0f));
    h_HSB.Set_Borders(sf::Vector2f(300.0f, 292.0f), sf::Vector2f(600.0f, 584.0f));

    around_half_secound_clock.restart();
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        scrollbar.update(deltaTime.asSeconds());

        sf::Event event;
        window.clear(sf::Color(200, 200, 200));

        frame_clock.restart();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (scrollbar.slider.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        isDraggingSlider = true;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isDraggingSlider = false;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (isDraggingSlider)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    float yRatio = static_cast<float>(mousePos.y - scrollbar.bar.getPosition().y) / scrollbar.bar.getSize().y;
                    float newValue = yRatio * scrollbar.maxValue;
                    scrollbar.setCurrentValue(newValue);
                }
            }
        }

        float currentValue = scrollbar.getCurrentValue();
        float slide = currentValue / scrollbar.maxValue;
        h_HSL.update(slide);
        h_HSB.update(slide);
        h_RGB.update(slide);
        h_CMY.update(slide);

        window.draw(h_RGB);
        window.draw(h_CMY);
        window.draw(h_HSL);
        window.draw(h_HSB);

        text.setString(std::to_string(mean_frames_time) + "us");
        window.draw(text);

        window.draw(scrollbar);

        frame_times.push_back(frame_clock.getElapsedTime().asMicroseconds());
        if (around_half_secound_clock.getElapsedTime().asSeconds() >= 0.5f && frame_times.size() >= 1)
        {
            mean_frames_time = (sf::Int64)((float)std::reduce(frame_times.begin(), frame_times.end()) / (float)frame_times.size());
            frame_times.clear();
            around_half_secound_clock.restart();
        }

        frame_clock.restart();
        window.display();
    }

    font.reset();

    return 0;
}
