#include <SFML/Graphics.hpp>
//#include <SFML/Font.hpp>

/*-lsfml-audio
-lsfml-graphics
-lsfml-window
-lsfml-system
*/
#include "source.h"

#include <cassert> 
#include <iostream>
#include <list>
#include <memory>

class Subject; 
class elf; 

mushroom::mushroom(const sf::Texture &image, Subject &sb, float time = 0)
{
		sb.Attach(this); 
		sys = &sb;
		 
		Spawntime = time;
		sprite.setTexture(image);
		bool colresult = true;
		while (colresult)
		{	
			X = rand()%MAPSIZE_X;		// start position
			Y = rand()%MAPSIZE_Y;
			colresult = Collision();
		}
			// start position
		sprite.setTextureRect(sf::IntRect(0, p_y, p_w , p_h ));
		sprite.scale(0.5f, 0.5f);
}/*	*/	
mushroom::~mushroom()
{

	sys->Detach(this);
}
elf::elf(const sf::Texture &image)
{
	sprite.setTexture(image);
	X = 100;		// start position
	Y = 350;	// start position
	sprite.setTextureRect(sf::IntRect(0, p_y, p_w , p_h ));
	sprite.scale(0.5f, 0.5f);
		
	dx = 0;
	dy = 0;
	CurrentFrame = 0;
}
elf::elf(const sf::Texture &image, Subject &sb)
{
	sb.Attach(this);  
	sprite.setTexture(image);
	X = 100;		// start position
	Y = 350;	// start position
	sprite.setTextureRect(sf::IntRect(0, p_y, p_w , p_h ));
	sprite.scale(0.5f, 0.5f);
		
	dx = 0;
	dy = 0;
	CurrentFrame = 0;
}
void mushroom::update(float time)
{	
		sprite.setPosition(X - offsetX, Y - offsetY);
		
		
		float T = cl.getElapsedTime().asSeconds();
		if(T-Spawntime>=5 && T-Spawntime<=6 )
		{
			sprite.setTextureRect(sf::IntRect(0,580, p_w , p_h ));
		}
		if(T-Spawntime>=9 && T-Spawntime<=10 )
		{
			sprite.setTextureRect(sf::IntRect(0,400, p_w , p_h+30 ));
		}	
		if(T-Spawntime>=10)
		{
	// list release
			sys->Detach(this);
		}
			
}

void elf::CollisionX()
{	//#include <cassert> 

	int TileX = (int) X/TileSize;
	int TileY = (int) Y/TileSize;
	
	int i, j;

	if (X<0 || Y<0)
	{
		return;
	}


    i = X/TileSize+1;	
	{
		// collision with right side
		i = X/TileSize-1;	
		if(dx<0)
		{
			for (int k = 0; k<2;k++)
			{
				j = Y/TileSize+k;
				if(TileMap[j][i] != ' ')
				{
					X = (i+1)*(TileSize);// + scale*p_h;
					dx = 0;
					return;
				}
				
			}
			
		}
		
		// collision with left side
		i = X/TileSize+p_w/TileSize*scale;	
		if(dx>0) 
		{
			for (int k = 0; k<2;k++)
			{
				j = Y/TileSize+k;
				if(TileMap[j][i] != ' ')
				{
					X =  (i)*(TileSize)-p_w*scale;
					dx = 0;
					return;
				}
			}
		}
	}
}
void elf::CollisionY()
{	//#include <cassert>
		int i, j;

	int TileX = (int) X/TileSize;
	int TileY = (int) Y/TileSize;

	if (X<0 || Y<0)
	{
		return;
	}

		

    i = X/TileSize+1;	
	{
		// collision with top side
		if(dy<0)
		{
			j = Y/TileSize;
			if(TileMap[j][i] != ' ')
			{
				Y = j*(TileSize+2);// - scale*p_h/2;
				dy = 0;
				return;
			}
				
		}
		

		j = Y/TileSize+p_h/TileSize*scale;
		if(dy>0) 
		{
			
			if(TileMap[j][i] != ' ')
			{
				Y =  j*TileSize - scale*p_h;
				dy = 0;
				OnGround = true;
				return;
			}
		}
		
		j = Y/TileSize+p_h/TileSize*scale;
		if(dy==0 && TileMap[j][i] == ' ' ) 
		{
			OnGround = false;	
			return;
		}				
	}	

}
void AI_Move(elf &AI, float time)
{
	bool OnGrTemp = AI.OnGround;
	int tempX = AI.X;
	int tempY = AI.Y;
	if(AI.AI_LeftMove == true)
	{
		AI.dx = Velocity/2;
	}
	else AI.dx = -Velocity/2;
	
	
	AI.update(time);
	if( OnGrTemp != AI.OnGround )
	{
		AI.dy = -Velocity*(1-std::rand()%3);
		AI.dx = -2*Velocity*(1-std::rand()%3);
		AI.AI_LeftMove = ( std::rand()%2);
		AI.OnGround = false;
		AI.update(time);
	}
	if(AI.X == tempX)
	{
		AI.dy = -Velocity*(1-std::rand()%3);
		AI.AI_LeftMove = ( std::rand()%2);
		AI.OnGround = false;
		AI.update(time);
	}


}

void DrawFont(sf::RenderWindow &sf_win, const sf::Texture &t)
{
//	sf::RectangleShape rectangle(sf::Vector2f(30, 30));

    sf::Sprite Cloud, Cloud2, Tree, Tree2, Sun;
    Cloud.setTexture(t);
	Cloud.setTextureRect(sf::IntRect(0, 0, 300 , 230 ));
	Cloud.scale(0.5f, 0.5f);	
	Cloud.setPosition(0 , 0);
	
	Cloud2.setTexture(t);
	Cloud2.setTextureRect(sf::IntRect(0, 230, 550 , 80 ));
	Cloud2.scale(0.5f, 0.5f);	
	Cloud2.setPosition(250 , 30);
	
	Sun.setTexture(t);
	Sun.setTextureRect(sf::IntRect(1000, 0, 200 ,200 ));
	Sun.scale(0.5f, 0.5f);	
	Sun.setPosition(350 , 50);
	

    Tree.setTexture(t);
	Tree.setTextureRect(sf::IntRect(0, 300, 600 , 600 ));
	Tree.scale(0.5f, 0.5f);
	
	int TreeX = 100, TreeY = 130;
	Tree.setPosition(TreeX - offsetX/5, TreeY-offsetY/5);

	Tree2.setTexture(t);
	Tree2.setTextureRect(sf::IntRect(1230, 300, 600 , 600 ));
	Tree2.scale(0.5f, 0.5f);
	
	int Tree2X = 700, Tree2Y = 130;
	Tree2.setPosition(Tree2X - offsetX/5, Tree2Y-offsetY/5);

	sf::RectangleShape rectangle(sf::Vector2f(1000, 450));
	sf::Color color(200, 230, 255);
	rectangle.setFillColor(color);	
	
	sf_win.draw(rectangle);
	sf_win.draw(Cloud);
	sf_win.draw(Cloud2);
	sf_win.draw(Tree);
	sf_win.draw(Tree2);
	sf_win.draw(Sun);	
	
	

}
void PlayerWin(bool Victory,sf::RenderWindow & sf_win, const sf::Texture &t, sf::Text &text)
{
	sf_win.clear();
	sf::RectangleShape rectangle(sf::Vector2f(1000,1000));
	rectangle.setFillColor(sf::Color::White);	
	sf_win.draw(rectangle);

	if(Victory)
		text.setString("You Win!");
	else 	
		text.setString("You Lose!");
    text.setPosition(sf_win.getSize().x/2-100, sf_win.getSize().y/5);

	sf_win.draw(text);
		
    sf::Sprite s;
    s.setTexture(t);
	s.setTextureRect(sf::IntRect(0, 0, 2000 , 2000 ));
	s.scale(0.3f, 0.3f);
	s.setPosition(sf_win.getSize().x/4,sf_win.getSize().y/5+50);
	sf_win.draw(s);
}

void DrawMap(sf::RenderWindow &sf_win, const sf::Texture &t)
{
//	sf::RectangleShape rectangle(sf::Vector2f(30, 30));

    sf::Sprite s;
    s.setTexture(t);
	s.setTextureRect(sf::IntRect(230, 960, 60 , 60 ));
	s.scale(0.5f, 0.5f);


	int i, j;
	for (i = 0; i<H; i++)
	{
		for(j = 0; j<TileMap[i].length(); j++)
		{
			if(TileMap[i][j] == ' ')
			{
				continue;	
			}	
			else if(TileMap[i][j] == '0')
			{
	//			rectangle.setFillColor(sf::Color::Red);	
				s.setTextureRect(sf::IntRect(310, 980, 60 , 60 ));
			}
			else if(TileMap[i][j] == 'P')
			{
				//rectangle.setFillColor(sf::Color::Blue);	
				s.setTextureRect(sf::IntRect(260, 910, 60 , 60 ));
			}
			else if(TileMap[i][j] == 'r')
			{
				s.setTextureRect(sf::IntRect(290, 940, 60 , 60 ));
				//rectangle.setFillColor(sf::Color::Green);	
			}
			else if(TileMap[i][j] == 'w')
			{
				s.setTextureRect(sf::IntRect(330, 940, 60 , 60 ));	
				//rectangle.setFillColor(sf::Color::Yellow);	
			}
			          
			s.setPosition(j*TileSize - offsetX,i*TileSize - offsetY);
			sf_win.draw(s);
		}
	}
}
void Subject::Detach(Creature* o)
{
	if (!o) return;
    mushroom* mush = dynamic_cast<mushroom*>(o);
	auto temp = observers.begin();
	bool del = false;		
    if (mush) 
    {
		
			auto temp2 = mush_list.begin();
			
			for (auto i = mush_list.begin(); i!=mush_list.end(); i++)
			{
				if ( (*i).get() == mush)
				{
					std::cout << "delete mushroom\n";
					temp2 = i;
					del = true;
				}
			}
			if(del)
				mush_list.erase(temp2);
   
	}


	for (auto i = observers.begin(); i != observers.end(); i++)
	{
		if (*i == o)
		{
			std::cout << "delete observer var\n";
			temp = i;
		}
	}
	if(del)
	observers.erase(temp);


/*	observers.remove(o);	 */ 

}
 	
void Subject::CollideObjects(elf *Player)
{

	for (auto o = observers.begin(); o!=observers.end(); o++) 
	{
	
		mushroom *mush = dynamic_cast<mushroom*>(*o);
		if (mush)
		{
			if ((Player->X > mush->X)&&(Player->X <= mush->X + 2*TileSize)
				&&(Player->Y > mush->Y)&&(Player->Y <= mush->Y + 2*TileSize)
				)
			{
				std::cout<<"Collide";
						
				Player->PickedMushrooms += 1;
				Detach(mush);
			}
		}
	}
		
	
	
	
	for (auto o = observers.begin(); o!=observers.end(); o++) 
	{
		elf *elf_inst = dynamic_cast<elf*>(*o);
		if(elf_inst )
		{
			for (auto other_o = o; other_o!=observers.end(); other_o++) 
			{
				mushroom *mush = dynamic_cast<mushroom*>(*other_o);
				if (mush)
				{
					if (
					(elf_inst->X > mush->X)&&(elf_inst->X <= mush->X + 2*TileSize)
					&&(elf_inst->Y > mush->Y)&&(elf_inst->Y <= mush->Y + 2*TileSize)
					)
					{
						std::cout<<"Collide";
						elf_inst->PickedMushrooms += 1;
						Detach(mush);
						PickedMushrooms+=1;
					}
				}
   			}
		}
	}	
}
void Subject::PrintText(const elf &Player, sf::RenderWindow &sf_win)
{
	int i = 1;
	Print (Player.PickedMushrooms, "Player:  ", i, sf_win) ;
	for (auto &o : observers)
	{
		elf *elf_i = dynamic_cast<elf*>(o);
		if(elf_i)
		{
			i++;
			Print (elf_i->PickedMushrooms,"Elf:  ", i, sf_win) ;
		}	
	}
	i++;
	Print (PickedMushrooms,"Elf Team:  ", i, sf_win) ;
}
void Subject::Print(int Mush_N, std::string Str, int line, sf::RenderWindow &sf_win)
{
	text.setString(Str + std::to_string(Mush_N));
    text.setPosition(sf_win.getSize().x-150, 50.f + 30*line);
	sf_win.draw(text);	
}
Subject::Subject()
{
   	
    std::string myfontFileName="../textures/Arial.ttf";
    if (!myFont.loadFromFile(myfontFileName))
        {
        std::cout << "Could not find the font " << myfontFileName << std::endl;
        }
        
   	text.setFont(myFont);
    text.setCharacterSize(24);
    text.setColor(sf::Color::Blue);
}
int main()
{
    using namespace sf;

	int MaxScore = 10;
    int p_x = 120 ,p_y = 660, p_w = 120, p_h = 120;
    sf::RenderWindow sf_win(sf::VideoMode(600, 600), "Hello");
    sf::CircleShape MyCircle(100.f );
    sf::RectangleShape rs(sf::Vector2f(100, 100));
    MyCircle.setFillColor(sf::Color::Green);

	sf::Text text;
	sf::Font myFont;
    std::string myfontFileName="../textures/Arial.ttf";
    if (!myFont.loadFromFile(myfontFileName))
        {
        std::cout << "Could not find the font " << myfontFileName << std::endl;
        }
        
   	text.setFont(myFont);
    text.setCharacterSize(48);
    text.setColor(sf::Color::Red);
    

    sf::Texture t;
    t.loadFromFile
  //  ("C:\Users\Santik\source\repos\ConsoleApplication12\pngegg (2).png");
  (elf_str);
    sf::Texture map_t;
    map_t.loadFromFile(map_str);
    
    
    sf::Texture mush_t;
    mush_t.loadFromFile(mush_str);
    
    sf::Texture victory;
    victory.loadFromFile(vic_str);
    
    sf::Texture defeat;
    defeat.loadFromFile(defeat_str);

	float CurrentFrame = 0;

	Subject system;
    elf Player(t);
	elf AI(t, system);
	elf AI2(t, system);
	elf AI3(t, system);
	AI2.X = 300;
	AI2.Y = 300;
	AI.X = 2000;
	AI.Y = 300;
	
    Clock clock, clock2;
    
/*	sf::Font MyFont;
	if (!MyFont.loadFromFile("arial.ttf", 50))
	{
    // Error...
    	cout << "font not loaded";
	}*/
	
    

    
   
	float timeSec = clock2.getElapsedTime().asSeconds();
	float SpawnTime = clock2.getElapsedTime().asSeconds();


    system.mush_list.push_back(std::make_shared<mushroom>(mush_t, system, timeSec));
    
    while(sf_win.isOpen())
    {
    	float time = clock.getElapsedTime().asMicroseconds();
    	clock.restart();
    	sf_win.clear();
    	
        sf::Event event;
        while (sf_win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                sf_win.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            Player.dx = -Velocity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            Player.dx = Velocity;
        }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if(Player.OnGround)
            {
            	Player.OnGround = false;
            	Player.dy = -Velocity*2;
			}
        }
        
        timeSec = clock2.getElapsedTime().asSeconds();
        
       
        if(-SpawnTime+timeSec>=3)
        {
        	std::cout<<"Spawned mushroom!!\n";
        	SpawnTime=clock2.getElapsedTime().asSeconds();
        	system.mush_list.push_back(std::make_shared<mushroom>(mush_t, system, 0.f));
		}
        
        offsetX = P_POS_X + Player.X;
 		offsetY = P_POS_Y ;//+ Player.Y;
 		
 		DrawFont(sf_win, map_t);
 		DrawMap(sf_win, map_t);
 		
 		system.MoveObjects(time);
 		system.DrawObjects(time, sf_win);
 		system.CollideObjects(&Player);
 		system.PrintText(Player, sf_win);
 		
 		
        Player.update(time);
        sf_win.draw(Player.sprite);
        if(Player.PickedMushrooms >= MaxScore)
        {
        	PlayerWin(true, sf_win, victory, text);
		}
        else if(system.PickedMushrooms >= MaxScore)
        {
         	PlayerWin(false, sf_win, defeat, text);
		}
        
        //sf_win.draw(text);

        sf_win.display();
    }
}
