#include <Adafruit_NeoPixel.h>
#include <Adafruit_LiquidCrystal.h>

#define LED_ARRAY_PIN 2
#define LED_ARRAY_WIDTH 6
#define LED_ARRAY_HEIGHT 8

#define BUTTON_UP 13
#define BUTTON_DOWN 12
#define BUTTON_LEFT 11
#define BUTTON_RIGHT 10

enum class Direction
{
  Up,
  Down,
  Right,
  Left
};

struct Position
{
  int x,y;
  Position():x(0), y(0){}
  Position(int pX, int pY): x(pX), y(pY){}
};

template<class T>
class Queue
{
  private:
    struct QNode
    {
      T content;
      QNode* next;

      QNode(T _content): content(_content), next(nullptr){}
      QNode(T _content, QNode* _next): content(_content), next(_next){}
    };

    QNode* first;
    QNode* last;
    int _size=0;
  public:
  
    Queue(): first(nullptr), last(nullptr){}

	void push(const T& content);
  	void pop();
  	T get(const int& index);
  	T get_last();
  	T get_first();
  	T* get_array();
    int size(){return _size;}
  	void clear()
    {
		while(_size>0)
          pop();
    }
};

template<class T> T Queue<T>::get_last(){return last->content;}
template<class T> T Queue<T>::get_first(){return first->content;}

template<class T> void Queue<T>::push(const T& content)
{
  	if(!first)
    {
       first = new QNode(content);
       last = first;
    }
    else
    {
       last->next = new QNode(content);
       last = last->next;
    }
      
    _size++;
}

template<class T> void Queue<T>::pop()
{
    if(!first)
      return;
     else if(first==last)
    {
      delete first;
      first=nullptr;
      last = nullptr;
    }
    else
    {
      QNode* next = first->next;
      delete first;
      first=next;
    }
  
  	_size--;
}

/*Performance of get function is poor
  For better performance consider implementing iterator
  But for our purposes this simple implementation is ok
*/

template<class T> T Queue<T>::get(const int& index)
{
  QNode* actual = first;
   if(index > _size)
     return T(); //normally we would throw exception
   for(int x=0;x<index;x++)
     actual = actual->next;
  
  return actual->content;
}

template<class T> T* Queue<T>::get_array()
{
  T* output = new T[_size];
  QNode* actual = first;
  for(int x=0;x<_size;x++)
  {
    output[x]= actual->content;
    actual=actual->next;
  }
  
  return output;
}

/*VARIABLES*/

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
   LED_ARRAY_WIDTH * LED_ARRAY_HEIGHT, 
   LED_ARRAY_PIN, 
   NEO_GRB + NEO_KHZ800);

Adafruit_LiquidCrystal screen(0);
int** Pixel_Array;
uint32_t Snake_Head_Color = Adafruit_NeoPixel::Color(250,150,0);
uint32_t Snake_Color = Adafruit_NeoPixel::Color(0,90,30);
uint32_t Fruit_Color = Adafruit_NeoPixel::Color(200,0,255);
Queue<Position> Snake;
Position Fruit_Position;

Direction Actual_Direction = Direction::Right;
int DirectionX = 1; //moving right at the start
int DirectionY = 0;

//game state flags
bool CollisionOccured = false;
bool GameCompleted = false;
bool FruitEaten = false;
bool Collided = false;

/*END VARIABLES*/

/*FUNCTION DECLARATIONS*/

void Init_Pixel_Array();
void Draw();
void Set_Direction(); //sets direction depending on input
void Change_Direction();
void Move();
void Collision(const Position& next_pos);
Position Generate_Unique_Position();
Position Next_Position(const Position& Actual_Position);
void Check_Boundaries();
bool Collides_With_Snake(const Position& pos);
void Check_Game_States();
void Wait_For_Input();
void Reload_Game();

/*END FUNTION DECLARATIONS*/



void setup()
{
  pinMode(LED_ARRAY_PIN, OUTPUT);
  
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
    
  Init_Pixel_Array();
  pixels.begin();
  
  Snake.push(Position(0,0));
  Fruit_Position = Generate_Unique_Position();
  
  screen.begin(16, 2);
  screen.print("Score");
  screen.setCursor(0,1);
  screen.print(Snake.size()-1);
}

void loop()
{
  delay(1000);
  Set_Direction();
  Move();
  Draw();
  Check_Game_States();
}

/*FUNCTION INITS*/

void Check_Game_States()
{ 
  if(GameCompleted)
  {
    screen.clear();
    screen.print("Congratulations !");
    screen.setCursor(0,1);
    screen.print("You won game");
    Wait_For_Input();
    Reload_Game();
  }
  if(Collided)
  {
    screen.clear();
    screen.print("You lost !");
    Wait_For_Input();
    Reload_Game();
  }
  if(FruitEaten)
  {
    //update score
    screen.setCursor(0,1);
    screen.print(Snake.size()-1);
  }
}

void Reload_Game()
{
  GameCompleted=false;
  Collided=false;
  FruitEaten=false;
  CollisionOccured=false;
  Snake.clear();
  Snake.push(Position(0,0));
  screen.clear();
  screen.print("Score");
  screen.setCursor(0,1);
  screen.print(Snake.size()-1);  
}

void Wait_For_Input()
{
  while(digitalRead(BUTTON_UP)!=HIGH &&
       digitalRead(BUTTON_DOWN)!=HIGH &&
       digitalRead(BUTTON_LEFT)!=HIGH &&
       digitalRead(BUTTON_RIGHT)!=HIGH)
  {}
}

Position Generate_Unique_Position()
{
  int size = Snake.size();
  int array_size = LED_ARRAY_WIDTH * LED_ARRAY_HEIGHT;
  int unique_size = array_size - size;
  
  if(size==array_size)
  {    
    GameCompleted=true;
	return Position(0,0);
  }
  
  Position* Snake_Coords= Snake.get_array();
  int* excluded_positions = new int[size];
  int* unique_positions = new int[unique_size];
  
  for(int x =0;x<size;x++)
    excluded_positions[x] = LED_ARRAY_WIDTH* Snake_Coords[x].y
    						+ Snake_Coords[x].x;
  
  int iterator = 0;
  bool excluded=false;
  for(int x=0;x<array_size;x++)
  {
     for(int y =0;y<size;y++)
       if(x==excluded_positions[y])
       {          
         excluded = true;
       	 break;
       }
    if(!excluded)
      unique_positions[iterator++]=x;
    excluded=false;
  }
  
  int random_pos = unique_positions[random(0, unique_size)];
  int posX = random_pos % LED_ARRAY_WIDTH;
  int posY = random_pos / LED_ARRAY_WIDTH;
  
  //clear memory
  delete[] Snake_Coords;
  delete[] excluded_positions;
  delete[] unique_positions;
  
  return Position(posX, posY);
}

void Init_Pixel_Array()
{
  Pixel_Array = new int*[LED_ARRAY_HEIGHT];
  
  int iterator=0;
  for(int x =0;x<LED_ARRAY_HEIGHT;x++)
  {
  	Pixel_Array[x] = new int[LED_ARRAY_WIDTH];
    for(int y=0;y<LED_ARRAY_WIDTH;y++)
     Pixel_Array[x][y] = iterator++;
  }
}

void Set_Direction()
{
  /*Only 1 button at the time is read, no more
  Order of buttons UP->Down->Left->Right
  If Down and up are pressed up has priority*/
  //prevent movement in opposite directions to the actual movement
  if(digitalRead(BUTTON_UP)==HIGH && Actual_Direction != Direction::Down)  
    Actual_Direction = Direction::Up;
  else if(digitalRead(BUTTON_DOWN)==HIGH && Actual_Direction != Direction::Up)   
    Actual_Direction = Direction::Down;
  else if(digitalRead(BUTTON_LEFT)==HIGH && Actual_Direction != Direction::Right)
    Actual_Direction = Direction::Left;
  else if(digitalRead(BUTTON_RIGHT)==HIGH && Actual_Direction != Direction::Left)
    Actual_Direction = Direction::Right;
    
  Change_Direction();
}

void Change_Direction()
{
  switch(Actual_Direction)
  {
    case Direction::Up:
    	DirectionY=-1; //in Array moving up is opposite
    	DirectionX=0;
    	break;
    case Direction::Down:
    	DirectionY=1;
    	DirectionX=0;
    	break;
    case Direction::Left:
    	DirectionY=0;
    	DirectionX=-1;
    	break;
    case Direction::Right:
    	DirectionY=0;
    	DirectionX=1;
    	break;
    default:
    	DirectionY=0;
    	DirectionX=0;
    	break;
  }
}

Position Next_Position(const Position& Actual_Position)
{
  Position next_pos(Actual_Position.x + DirectionX,
                    Actual_Position.y + DirectionY);
  
  if(Actual_Direction==Direction::Up &&
     next_pos.y < 0)
    next_pos.y=LED_ARRAY_HEIGHT - 1;
  else if(Actual_Direction==Direction::Down &&
     next_pos.y >=LED_ARRAY_HEIGHT)
    next_pos.y=0;
  else if(Actual_Direction==Direction::Right &&
     next_pos.x >=LED_ARRAY_WIDTH)
    next_pos.x=0;
  else if(Actual_Direction==Direction::Left &&
     next_pos.x < 0)
    next_pos.x=LED_ARRAY_WIDTH -1;
    
  return next_pos;
}

void Move()
{  
  Position head = Snake.get_last();
  Position next_pos = Next_Position(head);
  Collision(next_pos);
  //default action after no collision
  if(!CollisionOccured)
  {
    Snake.push(next_pos);
    Snake.pop();
  }
  CollisionOccured = false;
}

void Collision(const Position& next_pos)
{
  Position head = Snake.get_last();
  if(Pixel_Array[next_pos.y][next_pos.x] ==
     Pixel_Array[Fruit_Position.y][Fruit_Position.x])
  {
    Snake.push(Fruit_Position);
    Fruit_Position = Generate_Unique_Position();
    CollisionOccured=true;
    FruitEaten=true;
  }
  else if(Collides_With_Snake(next_pos))
  {    
    CollisionOccured=true;
	Collided=true;
  }  
}

bool Collides_With_Snake(const Position& pos)
{
  Position* positions = Snake.get_array();
  int size = Snake.size();
  
  for(int x=0;x<size;x++)
  {
    if(positions[x].x==pos.x && positions[x].y==pos.y)
      return true;
  }
  
  return false;
}

void Draw()
{
  pixels.clear();
  
  int size = Snake.size();
  Position* positions = Snake.get_array();
  //custom draw for head
  pixels.setPixelColor(Pixel_Array[positions[size-1].y][positions[size-1].x], Snake_Head_Color);
  for(int x=0;x<size-1;x++)
  {
    auto position = positions[x];
    pixels.setPixelColor(Pixel_Array[position.y][position.x], Snake_Color);
  }
  
  //draw fruit
  pixels.setPixelColor(Pixel_Array[Fruit_Position.y][Fruit_Position.x], Fruit_Color);
  
  pixels.show();
  delete[] positions;//prevent memory leaks
}

/*END FUNCTION INITS*/