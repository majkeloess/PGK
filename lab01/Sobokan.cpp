// Uwaga! Co najmniej C++17!!!
// Project-> ... Properties->Configuration Properties->General->C++ Language Standard = ISO C++ 17 Standard (/std:c++17)

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

private:
  std::vector<std::vector<Field>> map;
  sf::Vector2f shift, tile_size;
  sf::Vector2i player_position;
  std::vector<sf::Vector2i> park_positions;

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
}

void Sokoban::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

  for (int y = 0; y < map.size(); ++y)
  {
    for (int x = 0; x < map[y].size(); ++x)
    {
      sf::Vector2f position = shift + sf::Vector2f(x * tile_size.x, y * tile_size.y);

      sf::RectangleShape tile(sf::Vector2f(tile_size.x - 2, tile_size.y - 2));
      tile.setPosition(position);

      switch (map[y][x])
      {
      case Field::VOID:
        tile.setFillColor(sf::Color::Black);
        break;
      case Field::FLOOR:
        tile.setFillColor(sf::Color(150, 75, 0));
        break;
      case Field::WALL:
        tile.setFillColor(sf::Color::White);
        break;
      case Field::BOX:
        tile.setFillColor(sf::Color::Red);
        break;
      case Field::PARK:
        tile.setFillColor(sf::Color::Green);
        break;
      case Field::PLAYER:
        tile.setFillColor(sf::Color::Black);
        break;
      }

      target.draw(tile);
    }
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
  bool allow_move = false;                                             // Pesymistyczne za³ó¿my, ¿e gracz nie mo¿e siê poruszyæ.
  sf::Vector2i new_pp(player_position.x + dx, player_position.y + dy); // Potencjalna nowa pozycja gracza.
  Field fts = map[new_pp.y][new_pp.x];                                 // Element na miejscu na które gracz zamierza przejæ.
  if (fts != Field::WALL)
  {
    Field ftsa = map[new_pp.y + dy][new_pp.x + dx]; // Element na miejscu ZA miejscem na które gracz zamierza przejæ. :-D

    // Gracz mo¿e siê poruszyæ jeli pole na którym ma stan¹æ to pod³oga lub miejsce na skrzynki.
    if (fts == Field::FLOOR || fts == Field::PARK)
      allow_move = true;
    // Jeli pole na które chce siê poruszyæ gracz zawiera skrzynkê to mo¿e siê on poruszyæ jedynie jeli kolejne pole jest puste lub zawiera miejsce na skrzynkê  - bo wtedy mo¿e przepchn¹æ skrzynkê.
    if (fts == Field::BOX && (ftsa == Field::FLOOR || ftsa == Field::PARK))
    {
      allow_move = true;
      // Przepychamy skrzynkê.
      map[new_pp.y + dy][new_pp.x + dx] = Field::BOX;
      // Oczywicie pole na którym sta³a skrzynka staje siê teraz pod³og¹.
      map[new_pp.y][new_pp.x] = Field::FLOOR;
    }
  }

  if (allow_move)
  {
    // Przesuwamy gracza.
    map[player_position.y][player_position.x] = Field::FLOOR;
    player_position = new_pp;
    map[player_position.y][player_position.x] = Field::PLAYER;
  }

  // Niestety w czasie ruchu mog³y ucierpieæ miejsca na skrzynkê. ;-(
  for (auto park_position : park_positions)
    if (map[park_position.y][park_position.x] == Field::FLOOR)
      map[park_position.y][park_position.x] = Field::PARK;
}

bool Sokoban::Is_Victory() const
{
  // Tym razem dla odmiany optymistycznie zak³adamy, ¿e gracz wygra³.
  // No ale jeli na którymkolwiek miejscu na skrzynki nie ma skrzynki to chyba za³o¿enie by³o zbyt optymistyczne... : -/
  for (auto park_position : park_positions)
    if (map[park_position.y][park_position.x] != Field::BOX)
      return false;
  return true;
}

int COUNT = 0;

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

    if (!sokoban.Is_Victory())
    {
      while (window.pollEvent(event))
      {
        if (event.type == sf::Event::Resized)
        {
          // Zmiana widoku na nowe wymiary okna
          float width = static_cast<float>(event.size.width);
          float height = static_cast<float>(event.size.height);
          window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
          sokoban.SetDrawParameters(window.getSize());
        }
        else if (event.type == sf::Event::Closed)
        {
          // Obs³uga zamkniêcia okna
          window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
          // Obs³uga naciniêcia klawiszy przez gracza
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
            // Obs³uga innych klawiszy
            break;
          }
        }
      }
      window.draw(sokoban);
    }

    if (sokoban.Is_Victory())
    {
      while (COUNT < 1)
      {
        std::cout << "Gratulacje! Wygrales!" << std::endl;

        COUNT++;
      }
      sf::RectangleShape vicScreen(sf::Vector2f(window.getSize().x, window.getSize().y));
      vicScreen.setFillColor(sf::Color::Green);

      window.draw(vicScreen);
      // break;
      // window.close();
    }

    window.display();
  }

  return 0;
}