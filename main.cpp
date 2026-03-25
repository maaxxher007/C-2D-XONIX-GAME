// Author: Maaz Sher
// Email: shermaaz55@gmail.com
// Role: AI/ML Developer

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <fstream>
#include <cstring>
using namespace sf;

	const int m = 25;
	const int n = 40;

	int grd[m][n] = {0};
	int tilesiz = 18;
	int enemycont = 4;

//  enemy structure and movement
struct enemy
{
    int x, y, dx, dy;
    	float sped;
   		 enemy()
    {
     		   x = y = 300;
       			 dx = 4 - rand() % 8;
      				  dy = 4 - rand() % 8;
      					  sped = 1.0f;
    }
    void move()
    {
        	x += int(dx * sped);
      			  if (grd[y / tilesiz][x / tilesiz] == 1) { dx = -dx; x += int(dx * sped); }
      				  y += int(dy * sped);
       					 if (grd[y / tilesiz][x / tilesiz] == 1) { dy = -dy; y += int(dy * sped); }
    }
};

enemy enemis[20];

//  file I/O (high score)
int filel(const char* filename) {
    std::ifstream fin(filename);
   	 int scor = 0;
   		 fin >> scor;
   			 fin.close();
   				 return scor;
}



void files(const char* filename, int scor) {
  				  std::ofstream fout(filename);
   					 fout << scor << std::endl;
    						fout.close();
}




//  area calculation and claiming
void drop(int y, int x)
{
  	  if (y < 0 || y >= m || x < 0 || x >= n) return;
   		 if (grd[y][x] != 0) return;
   			 grd[y][x] = -1;
   				 drop(y - 1, x);
   					 drop(y + 1, x);
   						 drop(y, x - 1);
  								  drop(y, x + 1);
}







int area()
{
    											for (int i = 0; i < enemycont; i++)
       										 drop(enemis[i].y / tilesiz, enemis[i].x / tilesiz);
   									 int own = 0;
  								  for (int i = 0; i < m; i++)
      							  for (int j = 0; j < n; j++)
       								 {
            					if (grd[i][j] == 0) {
            				    grd[i][j] = 1;
            			    own++;
           							 }
          			  else if (grd[i][j] == -1)
              		  grd[i][j] = 0;
            else if (grd[i][j] == 2 || grd[i][j] == 3)
          grd[i][j] = 1;
        }
    return own;
}

// menu and difficulty enums
		enum menu { mainm, single, multi, exitmenu };
				enum hardship { none, easy, medium, hard, continuous };

//   game state variables
int diffselet = 0;
int multidiffselet = 0;
bool selectdificulty = false;
bool selectmultidificulty = false;
float continuostime = 0;
int continuousnextad = 20;

//  player 1 and player 2
int x1 = 0;
int y1playr = 0; 
int dx1 = 0;
int dy1 = 0;
int x2 = n - 1, y2playr = m - 1, dx2 = 0, dy2 = 0;
int scor1 = 0, scor2 = 0;
int powerupcont1 = 0, powerupcont2 = 0;
int lastpowerupscor1 = 0, lastpowerupscor2 = 0;

int main()
{
    srand(time(0));
  	  RenderWindow windo(VideoMode(n * tilesiz, m * tilesiz + 100), "Xonix Game!");
  		  windo.setFramerateLimit(60);

    			Texture backgroundtextur;
   				 backgroundtextur.loadFromFile("image.jpg");
   					 Sprite backgroundsprit(backgroundtextur);
   						 backgroundsprit.setScale(
     						   float(n * tilesiz) / backgroundtextur.getSize().x,
       							 float(m * tilesiz + 100) / backgroundtextur.getSize().y
   										 );

    												sf::Music music;
   								 if (music.openFromFile("music.wav")) {
     							   music.setLoop(true);
     						   music.play();
   													 }

    Texture t1, t2, t3;
   	 t1.loadFromFile("images/tiles.png");
    		t2.loadFromFile("images/gameover.png");
   			 t3.loadFromFile("images/enemy.png");

   					 Sprite stil(t1), sgameover(t2), senemy(t3);
   						 sgameover.setPosition(100, 100);
   							 senemy.setOrigin(20, 20);

    Font font;
    if (!font.loadFromFile("Arimo-Bold.ttf")) {
      				  return -1;
    						}

    Text menutext;
   	 menutext.setFont(font);
   		 menutext.setCharacterSize(36);
   			 menutext.setFillColor(Color::White);




    Text scoretext;
   		 scoretext.setFont(font);
   			 scoretext.setCharacterSize(24);
    				scoretext.setFillColor(Color::Yellow);
    					scoretext.setPosition(10, 10);




    menu menus = mainm;
   	 int menuselection = 0;
    		const int menuoptions = 4;
   			 std::string menuitems[menuoptions] = { "Single Player", "Multiplayer", "High Score", "Exit" };



    hardship difficulty = none;
   		 hardship multidifficulty = none;
		   			 const int diffoptions = 4;
						    std::string diffitems[diffoptions] = {
							"Easy ",
							"Medium ",
							"Hard ",
							"Continuous Mode"
						    };

    bool game = true;
    		float timer = 0, delay = 0.07;
   			 Clock clok;



    int highscor = filel("highscore.txt");
   	 bool highscorebroken = false;
   		 bool multihighscorebroken = false;



    for (int i = 0; i < m; i++)
       		 for (int j = 0; j < n; j++)
        		    if (i == 0 || j == 0 || i == m - 1 || j == n - 1)
          			      grd[i][j] = 1;
   					 float speedtim = 0;




    // This part is for menu and background textures
    Texture multimenuTextur, multieasyTextur, multimediumTextur, multihardTextur, multicontTextur;
    		Sprite multimenuSprit, multieasySprit, multimediumSprit, multihardSprit, multicontSprit;
   			 bool multimenuLoaded = false, multieasyLoaded = false, multimediumLoaded = false, multihardLoaded = false, multicontLoaded = false;



    if (multimenuTextur.loadFromFile("multi.jpg")) {
       	 multimenuSprit.setTexture(multimenuTextur);
       		 multimenuSprit.setScale(
         		   float(n * tilesiz) / multimenuTextur.getSize().x,
           				 float(m * tilesiz + 100) / multimenuTextur.getSize().y
     						   );
        multimenuLoaded = true;
    }
    
    
    
    if (multieasyTextur.loadFromFile("multie.jpg")) {
      		  multieasySprit.setTexture(multieasyTextur);
      			  multieasySprit.setScale(
       			     float(n * tilesiz) / multieasyTextur.getSize().x,
       				     float(m * tilesiz + 100) / multieasyTextur.getSize().y
        );
        multieasyLoaded = true;
    }
    
    
    
    if (multimediumTextur.loadFromFile("multim.jpg")) {
     		   multimediumSprit.setTexture(multimediumTextur);
       			 multimediumSprit.setScale(
        			    float(n * tilesiz) / multimediumTextur.getSize().x,
         				   float(m * tilesiz + 100) / multimediumTextur.getSize().y
       							 );
        multimediumLoaded = true;
    }
    
    
    
    
    if (multihardTextur.loadFromFile("multih.jpg")) {
      		  multihardSprit.setTexture(multihardTextur);
      			  multihardSprit.setScale(
        			    float(n * tilesiz) / multihardTextur.getSize().x,
         				   float(m * tilesiz + 100) / multihardTextur.getSize().y
      							  );
        multihardLoaded = true;
    }
    
    
    
    
    if (multicontTextur.loadFromFile("multicon.jpg")) {
     			   multicontSprit.setTexture(multicontTextur);
      				  multicontSprit.setScale(
         				   float(n * tilesiz) / multicontTextur.getSize().x,
          					  float(m * tilesiz + 100) / multicontTextur.getSize().y
     							   );
        multicontLoaded = true;
    }
    
    
    

    Texture singleeasyTextur, singlemediumTextur, singlehardTextur, singlecontTextur;
   			 Sprite singleeasySprit, singlemediumSprit, singlehardSprit, singlecontSprit;
   				 bool singleeasyLoaded = false, singlemediumLoaded = false, singlehardLoaded = false, singlecontLoaded = false;

    if (singleeasyTextur.loadFromFile("singlee.jpg")) {
       		 singleeasySprit.setTexture(singleeasyTextur);
      			  singleeasySprit.setScale(
       				     float(n * tilesiz) / singleeasyTextur.getSize().x,
        				    float(m * tilesiz + 100) / singleeasyTextur.getSize().y
       							 );
        singleeasyLoaded = true;
    }
    
    
    
    
    if (singlemediumTextur.loadFromFile("singlem.jpg")) {
      		  singlemediumSprit.setTexture(singlemediumTextur);
       			 singlemediumSprit.setScale(
          			  float(n * tilesiz) / singlemediumTextur.getSize().x,
           				 float(m * tilesiz + 100) / singlemediumTextur.getSize().y
        						);
        singlemediumLoaded = true;
    }
    
    
    
    
    if (singlehardTextur.loadFromFile("singleh.jpg")) {
     		   singlehardSprit.setTexture(singlehardTextur);
      			  singlehardSprit.setScale(
        			    float(n * tilesiz) / singlehardTextur.getSize().x,
          				  float(m * tilesiz + 100) / singlehardTextur.getSize().y
      							  );
        singlehardLoaded = true;
    }
    
    
    
    
    if (singlecontTextur.loadFromFile("singlecon.jpg")) {
      		  singlecontSprit.setTexture(singlecontTextur);
       			 singlecontSprit.setScale(
          			  float(n * tilesiz) / singlecontTextur.getSize().x,
          				  float(m * tilesiz + 100) / singlecontTextur.getSize().y
        );
        singlecontLoaded = true;
    }
    
    

    bool powerupactiv = false;
    	float poweruptimr = 0;
   		 int powerupactiveplayr = 0;





    // This part is for the main game loop
    while (windo.isOpen())
    {
        float tim = clok.getElapsedTime().asSeconds();
       		 clok.restart();
      			  timer += tim;
      				  speedtim += tim;

        // This part is for power up logic
        if (menus == single) {
         		   if (scor1 > 0 && scor1 % 30 == 0 && scor1 != lastpowerupscor1) {
           			     powerupcont1++;
            				    lastpowerupscor1 = scor1;
          				  }
       				 }
       				 
       				 
       				 
       				 
        if (menus == multi) {
           			 if (scor1 > 0 && scor1 % 30 == 0 && scor1 != lastpowerupscor1) {
               				 powerupcont1++;
             					   lastpowerupscor1 = scor1;
            								}
            								
            								
            								
            if (scor2 > 0 && scor2 % 30 == 0 && scor2 != lastpowerupscor2) {
         		       powerupcont2++;
             			   lastpowerupscor2 = scor2;
            }
        }

        if (powerupactiv) {
         			   poweruptimr += tim;
          				  if (poweruptimr >= 3.0f) {
           					     powerupactiv = false;
             						   poweruptimr = 0;
             							   powerupactiveplayr = 0;
            }
        						}
        						
        						
        						
        						

     	   if (speedtim >= 20.0f) {
       			     for (int i = 0; i < enemycont; i++)
         			       enemis[i].sped += 0.2f;
         					   speedtim = 0;
       								 }
       								 
       								 
       								 
       								 

        if (difficulty == continuous && menus == single && game) {
         	   continuostime += tim;
            			if (continuostime >= continuousnextad && enemycont + 2 <= 10) {
              				  for (int i = enemycont; i < enemycont + 2; i++)
                				    enemis[i] = enemy();
              						  enemycont += 2;
            						    continuousnextad += 20;
       							     }
       								 }



        if (multidifficulty == continuous && menus == multi && game) {
       			     continuostime += tim;
           			 if (continuostime >= continuousnextad && enemycont + 2 <= 10) {
               				 for (int i = enemycont; i < enemycont + 2; i++)
               				     enemis[i] = enemy();
              					  enemycont += 2;
              						  continuousnextad += 20;
           							 }
       									 }




        Event e;
        while (windo.pollEvent(e))
        {
            // This part is for menu and event handling
          			  if (e.type == Event::Closed)
           			     windo.close();

            if (menus == single) {
             	   if (e.type == Event::KeyPressed && e.key.code == Keyboard::O && powerupcont1 > 0 && !powerupactiv) {
               		     powerupactiv = true;
                			    poweruptimr = 0;
                 				   powerupactiveplayr = 1;
                  					  powerupcont1--;
             					   }
           					 }
           					 
           					 
            if (menus == multi) {
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::O && powerupcont1 > 0 && !powerupactiv) {
               		     powerupactiv = true;
                 			   poweruptimr = 0;
                 					   powerupactiveplayr = 1;
                  						  powerupcont1--;
                								}
                								
                								
                								
                								
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::I && powerupcont2 > 0 && !powerupactiv) {
                		    powerupactiv = true;
                  			  poweruptimr = 0;
                   				 powerupactiveplayr = 2;
                  				  powerupcont2--;
                						}
           								 }




            if (selectdificulty)
            {
                if (e.type == Event::KeyPressed)
                {
               	     if (e.key.code == Keyboard::Up)
                		        diffselet = (diffselet + diffoptions - 1) % diffoptions;
                    if (e.key.code == Keyboard::Down)
                  				      diffselet = (diffselet + 1) % diffoptions;
                  				      
                  				      
                  				      
                  				      
                    if (e.key.code == Keyboard::Enter)
                    {
                        if (diffselet == 0) difficulty = easy;
                    		    else if (diffselet == 1) difficulty = medium;
                     				   else if (diffselet == 2) difficulty = hard;
                       					 else if (diffselet == 3) difficulty = continuous;

                        enemycont = 4;
                    		    continuostime = 0;
                    			    continuousnextad = 20;
                    				    for (int i = 1; i < m - 1; i++)
                      						      for (int j = 1; j < n - 1; j++)
                       							         grd[i][j] = 0;
                       									 x1 = 0; y1playr = 0; dx1 = dy1 = 0;
                       										 scor1 = 0;
                      											  game = true;
                      												  highscorebroken = false;
                     							   for (int i = 0; i < enemycont; i++)
                        					    enemis[i] = enemy();
                        					selectdificulty = false;
                       					 menus = single;
                 						   }
                 				   if (e.key.code == Keyboard::Escape)
                  			      selectdificulty = false;
             						   }
             				   continue;
       	    						 }
       				     if (selectmultidificulty)
           					 {
             			   if (e.type == Event::KeyPressed)
               				 {
                  		  if (e.key.code == Keyboard::Up)
                       		 multidiffselet = (multidiffselet + diffoptions - 1) % diffoptions;
                   		 if (e.key.code == Keyboard::Down)
                     	   multidiffselet = (multidiffselet + 1) % diffoptions;
                   	 if (e.key.code == Keyboard::Enter)
                  	  {
                       if (multidiffselet == 0) multidifficulty = easy;
                      else if (multidiffselet == 1) multidifficulty = medium;
                    else if (multidiffselet == 2) multidifficulty = hard;
                  else if (multidiffselet == 3) multidifficulty = continuous;
                enemycont = 4;
              continuostime = 0;
           continuousnextad = 20;
  for (int i = 1; i < m - 1; i++)
     for (int j = 1; j < n - 1; j++)
         grd[i][j] = 0;
            x1 = 0; y1playr = 0; dx1 = dy1 = 0;
                x2 = n - 1; y2playr = m - 1; dx2 = dy2 = 0;
                    scor1 = scor2 = 0;
                       game = true;
                         multihighscorebroken = false;
                             for (int i = 0; i < enemycont; i++)
                                enemis[i] = enemy();
                                     selectmultidificulty = false;
                                         menus = multi;
                                              }
                    if (e.key.code == Keyboard::Escape)
                                           selectmultidificulty = false;
                }
                                                  continue;
            }

            if (menus == mainm)
            {
                     if (e.type == Event::KeyPressed)
                {
                             if (e.key.code == Keyboard::Up)
                                     menuselection = (menuselection + menuoptions - 1) % menuoptions;
                                     if (e.key.code == Keyboard::Down)
                                            menuselection = (menuselection + 1) % menuoptions;
                                                if (e.key.code == Keyboard::Enter)
                  						  {
                        if (menuselection == 0)
                       						 {
                                                       selectdificulty = true;
                                                          diffselet = 0;
                                                                    continue;
                        						}
                        else if (menuselection == 1)
                        						{
                                                 selectmultidificulty = true;
                                                    multidiffselet = 0;
                                                      continue;
                       							 }
                        else if (menuselection == 2)
                       					 {
                                         menus = exitmenu;
                       						 }
                        else if (menuselection == 3)
                      						  {
                                       windo.close();
                      						  }
                   					 }
                }
            }
            else if (menus == multi || menus == single)
          						  {
                                      if (e.type == Event::KeyPressed)
            						    {
                                       if (e.key.code == Keyboard::Escape)
             				       {
                        menus = mainm;
                				    }
                }
            }
            else if (menus == exitmenu)
          						  {
                                        if (e.type == Event::KeyPressed)
            					    {
                    menus = mainm;
                }
            }
        }

        windo.clear();

        //  drawing menus and backgrounds
        if (selectmultidificulty && multimenuLoaded) {
            windo.draw(multimenuSprit);
        }
        else if (menus == multi) {
            if (multidifficulty == easy && multieasyLoaded)
          	      windo.draw(multieasySprit);
         	   else if (multidifficulty == medium && multimediumLoaded)
             		   windo.draw(multimediumSprit);
          			  else if (multidifficulty == hard && multihardLoaded)
              			  windo.draw(multihardSprit);
         				   else if (multidifficulty == continuous && multicontLoaded)
             					   windo.draw(multicontSprit);
          				  else if (multimenuLoaded)
             					   windo.draw(multimenuSprit);
        }
        else if (menus == single) {
          		  if (difficulty == easy && singleeasyLoaded)
           		 		    windo.draw(singleeasySprit);
          					  else if (difficulty == medium && singlemediumLoaded)
              							  windo.draw(singlemediumSprit);
           						 else if (difficulty == hard && singlehardLoaded)
               					 windo.draw(singlehardSprit);
          				  else if (difficulty == continuous && singlecontLoaded)
              			  windo.draw(singlecontSprit);
            else
                windo.draw(backgroundsprit);
        }
        else {
            windo.draw(backgroundsprit);
        }

        // drawing difficulty selection menu
        if (selectdificulty)
        {
            for (int i = 0; i < diffoptions; i++)
            {
              	  menutext.setString(diffitems[i]);
              		  menutext.setPosition(120, 120 + i * 60);
                if (i == diffselet)
              				      menutext.setFillColor(Color::Yellow);
                else
              					      menutext.setFillColor(Color::White);
              						  windo.draw(menutext);
            }
            windo.display();
            continue;
        }

        // drawing multiplayer difficulty selection menu
        if (selectmultidificulty)
        {
            for (int i = 0; i < diffoptions; i++)
            {
           		     menutext.setString(diffitems[i]);
            			    menutext.setPosition(120, 120 + i * 60);
                if (i == multidiffselet)
             				       menutext.setFillColor(Color::Yellow);
                else
             					       menutext.setFillColor(Color::White);
            						    windo.draw(menutext);
            }
            windo.display();
            continue;
        }

        //  drawing main menu
        if (menus == mainm)
        {
            for (int i = 0; i < menuoptions; i++)
            {
                menutext.setString(menuitems[i]);
            		    menutext.setPosition(120, 120 + i * 60);
                if (i == menuselection)
            			        menutext.setFillColor(Color::Yellow);
                else
            				        menutext.setFillColor(Color::White);
                windo.draw(menutext);
            }
            windo.display();
            continue;
        }
        
        
        // rawing high score/exit menu
        else if (menus == exitmenu)
        {
            menutext.setString("High Score: " + std::to_string(highscor) +
                               "\n\nPress any key to return");
           					 menutext.setPosition(80, 40);
          							  menutext.setFillColor(Color::Cyan);
          								  windo.draw(menutext);
         									   windo.display();
            continue;
        }
        
        
        
        
        //player 1 (single player) logic and drawing
        else if (menus == single)
        {
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }

            if (!game)
            {
                if (scor1 > highscor) {
              	      highscor = scor1;
                 			   files("highscore.txt", highscor);
                 				   highscorebroken = true;
                }

                windo.draw(sgameover);

               		 Text finalscoretext;
              			  finalscoretext.setFont(font);
             				   finalscoretext.setCharacterSize(36);
              					  finalscoretext.setFillColor(Color::Red);
              						  std::string msg = "Final Score:\nPlayer 1: " + std::to_string(scor1);
             						   if (highscorebroken) msg += "\nHigh Score Broken!";
             							   finalscoretext.setString(msg);
             								   finalscoretext.setPosition(120, 250);
             									   windo.draw(finalscoretext);

                windo.display();
                continue;
            }

            if (timer > delay)
            {
                x1 += dx1;
            	    y1playr += dy1;
            		    if (x1 < 0) x1 = 0; if (x1 > n - 1) x1 = n - 1;
             			   if (y1playr < 0) y1playr = 0; if (y1playr > m - 1) y1playr = m - 1;
              				  if (grd[y1playr][x1] == 2) game = false;
						     int tilesThisMove = 0;
              						  if (grd[y1playr][x1] == 0) { grd[y1playr][x1] = 2; tilesThisMove = 1; }
							          if (grd[y1playr][x1] == 1 && (dx1 != 0 || dy1 != 0))
                {
           							         dx1 = dy1 = 0;
            								        int claimed = area();
             									       if (claimed > 10)
             									           scor1 += claimed * 2;
                    else
                									        scor1 += claimed;
                }
                else if (tilesThisMove)
                {
                    scor1 += 1;
                }

                timer = 0;
            }

            if (!powerupactiv) {
          		      for (int i = 0; i < enemycont; i++) enemis[i].move();
            }

            for (int i = 0; i < enemycont; i++)
          		      if (grd[enemis[i].y / tilesiz][enemis[i].x / tilesiz] == 2)
           			         game = false;

            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                {
                    if (grd[i][j] == 0) continue;
               		     if (grd[i][j] == 1) stil.setTextureRect(IntRect(0, 0, tilesiz, tilesiz));
                			    if (grd[i][j] == 2) stil.setTextureRect(IntRect(54, 0, tilesiz, tilesiz));
                				    stil.setPosition(j * tilesiz, i * tilesiz + 50);
                 					   windo.draw(stil);
                }

            stil.setTextureRect(IntRect(36, 0, tilesiz, tilesiz));
          		  stil.setPosition(x1 * tilesiz, y1playr * tilesiz + 50);
          			  windo.draw(stil);

            senemy.rotate(10);
            for (int i = 0; i < enemycont; i++)
            {
                senemy.setPosition(enemis[i].x, enemis[i].y + 50);
                windo.draw(senemy);
            }

            scoretext.setString("Player 1: " + std::to_string(scor1));
            windo.draw(scoretext);
        }
        // player 1 and player 2 (multiplayer) logic and drawing
        else if (menus == multi)
        {
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0; }
           	 if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0; }
         		   if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }
          			  if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }



           			 if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }
          				  if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }
          					  if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1; }
          						  if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1; }

            if (!game)
            {
                std::string winner;
          		      if (scor1 > scor2) winner = "Player 1 Wins!";
           			     else if (scor2 > scor1) winner = "Player 2 Wins!";
            				    else winner = "It's a Tie!";

                int bestscor = (scor1 > scor2) ? scor1 : scor2;
             			   if (bestscor > highscor) {
               					     highscor = bestscor;
                						    files("highscore.txt", highscor);
                 							   multihighscorebroken = true;
                }

                windo.draw(sgameover);

                Text finalscoretext;
             		   finalscoretext.setFont(font);
             				   finalscoretext.setCharacterSize(36);
              						  finalscoretext.setFillColor(Color::Red);
               								 std::string msg = "Final Score:\nPlayer 1: " + std::to_string(scor1) +
                    						              "\nPlayer 2: " + std::to_string(scor2) +
                       							           "\n" + winner;
               							 if (multihighscorebroken) msg += "\nHigh Score Broken!";
              					  finalscoretext.setString(msg);
              				  finalscoretext.setPosition(120, 220);
              			  windo.draw(finalscoretext);
			  windo.display();
                continue;
            }

            if (timer > delay)
            {
                x1 += dx1;
              		  y1playr += dy1;
              			  if (x1 < 0) x1 = 0; if (x1 > n - 1) x1 = n - 1;
              				  if (y1playr < 0) y1playr = 0; if (y1playr > m - 1) y1playr = m - 1;
              					  int tilesThisMove1 = 0;
              							  if (grd[y1playr][x1] == 2 || grd[y1playr][x1] == 3) game = false;
              					  if (grd[y1playr][x1] == 0) { grd[y1playr][x1] = 2; tilesThisMove1 = 1; }
				         x2 += dx2;
             			   y2playr += dy2;
              				  if (x2 < 0) x2 = 0; if (x2 > n - 1) x2 = n - 1;
               			 if (y2playr < 0) y2playr = 0; if (y2playr > m - 1) y2playr = m - 1;
              		  int tilesThisMove2 = 0;
              	  if (grd[y2playr][x2] == 2 || grd[y2playr][x2] == 3) game = false;
              if (grd[y2playr][x2] == 0) { grd[y2playr][x2] = 3; tilesThisMove2 = 1; }
            if (grd[y1playr][x1] == 1 && (dx1 != 0 || dy1 != 0))
                {
               dx1 = dy1 = 0;
                    int claimed = area();
                   	 if (claimed > 10)
                  	      scor1 += claimed * 2;
                 	   else
                    		    scor1 += claimed;
                }
             				   else if (tilesThisMove1)
                {
              						      scor1 += 1;
                }

               		 if (grd[y2playr][x2] == 1 && (dx2 != 0 || dy2 != 0))
                {
                			    dx2 = dy2 = 0;
               					     int claimed = area();
                					    if (claimed > 10)
                 						       scor2 += claimed * 2;
                    else
                 							       scor2 += claimed;
                }
              									  else if (tilesThisMove2)
                {
            											        scor2 += 1;
                }

          												      timer = 0;
            }

            if (!powerupactiv) {
         		       for (int i = 0; i < enemycont; i++) enemis[i].move();
            }

          	  for (int i = 0; i < enemycont; i++)
                if (grd[enemis[i].y / tilesiz][enemis[i].x / tilesiz] == 2 || grd[enemis[i].y / tilesiz][enemis[i].x / tilesiz] == 3)
                    game = false;

            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                {
                    if (grd[i][j] == 0) continue;
             	       if (grd[i][j] == 1) stil.setTextureRect(IntRect(0, 0, tilesiz, tilesiz));
              		      if (grd[i][j] == 2) stil.setTextureRect(IntRect(54, 0, tilesiz, tilesiz));
              				      if (grd[i][j] == 3) stil.setTextureRect(IntRect(72, 0, tilesiz, tilesiz));
                				    stil.setPosition(j * tilesiz, i * tilesiz + 50);
                    windo.draw(stil);
                }

            stil.setTextureRect(IntRect(36, 0, tilesiz, tilesiz));
          			  stil.setPosition(x1 * tilesiz, y1playr * tilesiz + 50);
         				   windo.draw(stil);
        					    stil.setTextureRect(IntRect(90, 0, tilesiz, tilesiz));
         						   stil.setPosition(x2 * tilesiz, y2playr * tilesiz + 50);
         								   windo.draw(stil);

            senemy.rotate(10);
            for (int i = 0; i < enemycont; i++)
            {
                senemy.setPosition(enemis[i].x, enemis[i].y + 50);
                windo.draw(senemy);
       
            }

            scoretext.setString("Player 1: " + std::to_string(scor1) + "  Player 2: " + std::to_string(scor2));
            windo.draw(scoretext);
        }

        windo.display();
    }

    return 0;
}
