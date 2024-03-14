#include "SFML/Graphics.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
enum class Field
{
  VOID,
  FLOOR,
  WALL,
  BOX,
  PARK,
  PLAYER
};

class Sokoban : public sf::Drawable
{
public:
  void LoadMapFromFile(std::string fileName);
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  void SetDrawParameters(sf::Vector2u draw_area_size);
  void Move_Player_Left();
  void Move_Player_Right();
  void Move_Player_Up();
  void Move_Player_Down();
  bool Is_Victory() const;
  sf::Text getVictoryText()
  {
    return victory_text;
  }
  void UpdateVictoryTextPosition(sf::Vector2u windowSize)
  {
    if (Is_Victory())
    {
      sf::FloatRect textRect = victory_text.getLocalBounds();
      victory_text.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
      victory_text.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));
    }
  }

private:
  std::vector<std::vector<Field>> map;
  sf::Vector2f shift, tile_size;
  sf::Vector2i player_position;
  std::vector<sf::Vector2i> park_positions;
  sf::Font font;
  sf::Text victory_text;
  sf::Texture wall_texture;
  sf::Texture box_texture;
  sf::Texture player_texture;
  sf::Texture floor_texture;

  void move_player(int dx, int dy);
};

void Sokoban::LoadMapFromFile(std::string fileName)
{
  std::string str;
  std::vector<std::string> vos;

  std::ifstream in(fileName.c_str());

  if (!in)
  {
    std::cerr << "Error opening file: " << fileName << std::endl;
    return;
  }

  while (std::getline(in, str))
  {
    vos.push_back(str);
  }
  in.close();

  map.clear();
  map.resize(vos.size(), std::vector<Field>(vos[0].size()));
  for (auto [row, row_end, y] = std::tuple{vos.cbegin(), vos.cend(), 0}; row != row_end; ++row, ++y)
    for (auto [element, end, x] = std::tuple{row->begin(), row->end(), 0}; element != end; ++element, ++x)
      switch (*element)
      {
      case 'X':
        map[y][x] = Field::WALL;
        break;
      case '*':
        map[y][x] = Field::VOID;
        break;
      case ' ':
        map[y][x] = Field::FLOOR;
        break;
      case 'B':
        map[y][x] = Field::BOX;
        break;
      case 'P':
        map[y][x] = Field::PARK;
        park_positions.push_back(sf::Vector2i(x, y));
        break;
      case 'S':
        map[y][x] = Field::PLAYER;
        player_position = sf::Vector2i(x, y);
        break;
      }
  wall_texture.loadFromFile("wall.png");
  player_texture.loadFromFile("player.png");
  box_texture.loadFromFile("chest.png");
  floor_texture.loadFromFile("floor.png");

  font.loadFromFile("garota_bonita.ttf");
  victory_text.setFont(font);
  victory_text.setString("");
  victory_text.setCharacterSize(100);
  victory_text.setFillColor(sf::Color::Red);
  victory_text.setStyle(sf::Text::Bold);
  victory_text.setPosition(shift.x + tile_size.x * map[0].size() / 2, shift.y + tile_size.y * map.size() / 2);
}

void Sokoban::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  sf::Sprite sprite;
  sf::RectangleShape tile(sf::Vector2f(tile_size.x - 2, tile_size.y - 2));

  for (int y = 0; y < map.size(); ++y)
  {
    for (int x = 0; x < map[y].size(); ++x)
    {
      sf::Vector2f position = shift + sf::Vector2f(x * tile_size.x, y * tile_size.y);

      tile.setPosition(position);
      sprite.setPosition(position);
      sprite.setScale(
          tile_size.x / sprite.getLocalBounds().width,
          tile_size.y / sprite.getLocalBounds().height);

      switch (map[y][x])
      {
      case Field::VOID:
        tile.setFillColor(sf::Color::Black);
        target.draw(tile);
        break;
      case Field::FLOOR:
        sprite.setTexture(floor_texture);
        target.draw(sprite);
        break;
      case Field::WALL:
        sprite.setTexture(wall_texture);
        target.draw(sprite);
        break;
      case Field::BOX:
        sprite.setTexture(box_texture);
        target.draw(sprite);
        break;
      case Field::PARK:
        tile.setFillColor(sf::Color::Red);
        target.draw(tile);
        break;
      case Field::PLAYER:
        tile.setFillColor(sf::Color::Black);
        target.draw(tile);
        break;
      }
    }
  }

  if (Is_Victory())
  {
    target.draw(victory_text);
  }
}

void Sokoban::SetDrawParameters(sf::Vector2u draw_area_size)
{
  this->tile_size = sf::Vector2f(
      std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size())),
      std::min(std::floor((float)draw_area_size.x / (float)map[0].size()), std::floor((float)draw_area_size.y / (float)map.size())));
  this->shift = sf::Vector2f(
      ((float)draw_area_size.x - this->tile_size.x * map[0].size()) / 2.0f,
      ((float)draw_area_size.y - this->tile_size.y * map.size()) / 2.0f);
}

void Sokoban::Move_Player_Left()
{
  move_player(-1, 0);
}

void Sokoban::Move_Player_Right()
{
  move_player(1, 0);
}

void Sokoban::Move_Player_Up()
{
  move_player(0, -1);
}

void Sokoban::Move_Player_Down()
{
  move_player(0, 1);
}

void Sokoban::move_player(int dx, int dy)
{
  bool allow_move = false;
  sf::Vector2i new_pp(player_position.x + dx, player_position.y + dy);
  Field fts = map[new_pp.y][new_pp.x];
  if (fts != Field::WALL)
  {
    Field ftsa = map[new_pp.y + dy][new_pp.x + dx];

    if (fts == Field::FLOOR || fts == Field::PARK)
      allow_move = true;

    if (fts == Field::BOX && (ftsa == Field::FLOOR || ftsa == Field::PARK))
    {
      allow_move = true;

      map[new_pp.y + dy][new_pp.x + dx] = Field::BOX;
      map[new_pp.y][new_pp.x] = Field::FLOOR;
    }
  }

  if (allow_move)
  {
    map[player_position.y][player_position.x] = Field::FLOOR;
    player_position = new_pp;
    map[player_position.y][player_position.x] = Field::PLAYER;
  }

  for (auto park_position : park_positions)
    if (map[park_position.y][park_position.x] == Field::FLOOR)
      map[park_position.y][park_position.x] = Field::PARK;

  if (Is_Victory())
  {
    victory_text.setString("Victory");
  }
}

bool Sokoban::Is_Victory() const
{

  for (auto park_position : park_positions)
    if (map[park_position.y][park_position.x] != Field::BOX)
      return false;
  return true;
}

int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "GFK Lab 01", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
  Sokoban sokoban;

  sokoban.LoadMapFromFile("plansza.txt");
  sokoban.SetDrawParameters(window.getSize());

  while (window.isOpen())
  {
    sf::Event event;
    window.clear();

    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Resized)
      {
        float width = static_cast<float>(event.size.width);
        float height = static_cast<float>(event.size.height);
        window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
        sokoban.SetDrawParameters(window.getSize());
      }
      else if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
      {
        window.close();
      }

      if (!sokoban.Is_Victory())
      {
        if (event.type == sf::Event::KeyPressed)
        {
          switch (event.key.code)
          {
          case sf::Keyboard::Left:
            sokoban.Move_Player_Left();
            break;
          case sf::Keyboard::Right:
            sokoban.Move_Player_Right();
            break;
          case sf::Keyboard::Up:
            sokoban.Move_Player_Up();
            break;
          case sf::Keyboard::Down:
            sokoban.Move_Player_Down();
            break;
          default:
            break;
          }
        }
      }
    }

    if (!sokoban.Is_Victory())
    {
      window.draw(sokoban);
    }

    if (sokoban.Is_Victory())
    {
      sf::RectangleShape vicScreen(sf::Vector2f(window.getSize().x, window.getSize().y));
      vicScreen.setFillColor(sf::Color::White);

      window.draw(vicScreen);
      sokoban.UpdateVictoryTextPosition(window.getSize());
      window.draw(sokoban.getVictoryText());
    }

    window.display();
  }

  return 0;
}