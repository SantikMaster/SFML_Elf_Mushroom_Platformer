#include <list>
#include <memory>

class Subject; 
class elf; 


const float G_force = 0.00005;
const int ground = 400;
const float FramesX = 6.;
const int TileSize = 30;
const float Velocity = 0.0005;


const int H = 17;  
const int W = 89;  

const int P_POS_Y = 0;
const int P_POS_X = -300;

float offsetX = 0, offsetY = 0;
const int MAPSIZE_X = 2200;		// start position
const int MAPSIZE_Y = 350;

std::string map_str = "map.png";
std::string elf_str = "pngegg (2).png";
std::string mush_str = "mush.png";


std::string TileMap[H] = {

       "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
 
       "0                                                                          0                                                                           0",

       "0                                                                          0           w                                                               0",

       "0                PPPP                                  P                   0 w                                                                         0",

       "0                                   PP                                     0     kk                                                                    0",

       "0                                                                          0    k  k    k    k                                                         0",

       "0                      P                      PPP                          0    k      kkk  kkk  w                                                     0",

       "0                                            P                            r0    k       k    k                                                         0",

       "0       PPP                                 P                            rr0    k  k                                                                   0",

       "0                                                            OOO        rrr0     kk                                                                    0",

       "0             PPP    PPPPP                                             rrrr0                                                                           0",

       "0                                      0t0                            rrrrr0                                                                           0",

       "0P                                     000         PPP               rrrrrr0           G                                                               0",

       "0           r               w          000              000         rrrrrrr0                                                                           0",

       "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",

       "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",

       "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",

}; 

class Creature
{
//	Subject &sb;
public:
	float X, Y;
	sf::Sprite sprite;
	virtual void update(float time)
	{	
		sprite.setPosition(X - offsetX, Y - offsetY);

	}

	virtual ~Creature() = default;

};
 

class mushroom : public Creature
{
	Subject * sys;
    sf::Clock cl;

	const int p_x = 00 ,p_y = 00, p_w = 120, p_h = 120;
	const float scale = 0.5;	

	float CurrentFrame;
	float Spawntime;
	
public:	
	bool AI_LeftMove;

	mushroom(const sf::Texture &image, Subject &sb,  float time);
	~mushroom();

	void update(float time) override;
	bool Collision()
	{
			//#include <cassert>
		int i, j;


	int TileX = (int) X/TileSize;
	int TileY = (int) Y/TileSize;

	if (X<0 || Y<0)
	{
		return true;
	}

		
    for(i = X/TileSize;i< X/TileSize+2;i++)  	
	{
		for(j = Y/TileSize;j< Y/TileSize+2;j++)  	
		{
			if(TileMap[j][i] != ' ')
			{
				return true;
			}
		}
	}
		
	return false;		
    }	
};
    
class elf : public Creature
{


	const int p_x = 120 ,p_y = 660, p_w = 120, p_h = 120;
	const float scale = 0.5;
	
	const float deltaFrame = 0.005;

	float CurrentFrame;
	
public:	
	bool AI_LeftMove;

	float dx, dy;
	bool OnGround;
	int PickedMushrooms = 0;		

	elf(const sf::Texture &image, Subject &sb);
	elf(const sf::Texture &image); // Ok for player character
	
	void update(float time) override
	{
	
		CollisionX();
		X += dx*time;
		if(!OnGround)
		{
			dy += 0.0001*G_force*time;
		}
		
		CollisionY();
		Y += dy*time;
		if (Y > ground) 
		{
			Y = ground;
			OnGround = true;
			dy = 0;
		}
		
		//s.move(-0.001*time, 0);
            
        CurrentFrame += deltaFrame*time;
        if(CurrentFrame > FramesX)
        {
           	CurrentFrame -=  FramesX;
		}
		if(dx>0)
		{
			sprite.setTextureRect(sf::IntRect(p_x*int(CurrentFrame)+p_w ,p_y , -p_w , p_h));	
		}
		if(dx<0)
		{
			sprite.setTextureRect(sf::IntRect(p_x*int(CurrentFrame)  ,p_y , p_w , p_h));
		}
		
	//	sprite.setPosition(rect.left, rect.top);
	
		sprite.setPosition(X - offsetX, Y - offsetY);
		dx = 0;
	}
	void CollisionX();
	void CollisionY();
};
class Subject 
{ 
    sf::Text text;
	sf::Font myFont;
public: 
   std::list<std::shared_ptr<mushroom>> mush_list;
   
   Subject();
     virtual ~Subject() = default;
     void Attach(Creature& o) 
	 {
	  observers.push_back(&o); 
	 }
	 void Attach(Creature* o) 
	 {
	  observers.push_back(o); 
	 }
     void Detach(Creature* o);

     MoveObjects(float time)
     {
     	for (auto* o : observers)
		{
			elf *AI = dynamic_cast<elf*>(o);
			
        	if (AI) 
        	{
        		bool OnGrTemp = AI->OnGround;
				int tempX = AI->X;
				int tempY = AI->Y;
				if(AI->AI_LeftMove == true)
				{
					AI->dx = Velocity;
				}
				else AI->dx = -Velocity;

	
				AI->update(time);
				if( OnGrTemp != AI->OnGround )
				{
					AI->dy = -2*Velocity*(1-std::rand()%3);
					AI->dx = -Velocity*(1-std::rand()%3);
					AI->AI_LeftMove = ( std::rand()%2);
					AI->OnGround = false;
					AI->update(time);
				}
				if(AI->X == tempX)
				{

					AI->update(time);
				}
				if(AI->X == tempX && AI->Y == tempY)
				{
				
					AI->dy = -2*Velocity;
					AI->OnGround = false;
					if (std::rand()%4 == 1)
					{
						AI->AI_LeftMove = ( std::rand()%2);
					}

					AI->update(time);
				}
     	
			}
			mushroom *mush = dynamic_cast<mushroom*>(o);
			
        	if (mush) 
        	{
        		mush->update(time);
			}
			
			
        }
     		
	 }
 	 DrawObjects(float time, sf::RenderWindow &sf_win)
 	 {
 	 	for (auto* o : observers) 
		{
        	sf_win.draw(o->sprite);
        	o->update(time); 	
   		}
	 }
	void CollideObjects(elf* Player);
	void PrintText(const elf &Player, sf::RenderWindow &sf_win);
	void Print(int Mush_N, std::string Str, int line, sf::RenderWindow &sf_win);
private:
     std::list<Creature*> observers;

};

