#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include<stdlib.h>
#define ybar 600
#define screenWidth 800
#define screenHeight 640
#define brickLenght 40
#define brickDepth 20
#define brickNumberLine 10
#define brickNumberColumn 20
typedef struct
{
	double x;
	double y;
	int r;
	int g;
	int b;
	bool exist;
	bool magnet;
	bool plusBall;
	bool minusBall;
	bool plusBar;
	bool minusBar;
	bool speedUp;
	bool speedDown;

} brick;
bool checkBarCollision(double xbar,double x,double y,double radius,double barLenght,double moveSpeedy)
{
	if((ybar+moveSpeedy>=y+radius && y+radius>=ybar && x>=xbar && x<=xbar+barLenght) || sqrt(pow(x-xbar,2) + pow(y-ybar,2))<=radius || sqrt(pow(xbar+barLenght-x,2) + pow(y-ybar,2))<=radius) return true;
	return false;
}
bool checkWallCollision(double x,double y,double radius)
{
	if(x-radius<=0 || x+radius>=screenWidth) return true;
	return false;
}
bool checkCeilingCollision(double x,double y,double radius)
{
	if(y-radius<=0) return true;
	return false;
}
void dir1Increment(double &x,double &y,double moveSpeedx,double moveSpeedy)
{
	x+=moveSpeedx;
	y+=moveSpeedy;
}
void dir2Increment(double &x,double &y,double moveSpeedx,double moveSpeedy)
{
	x+=moveSpeedx;
	y-=moveSpeedy;
}
void dir3Increment(double &x,double &y,double moveSpeedx,double moveSpeedy)
{
	x-=moveSpeedx;
	y-=moveSpeedy;
}
void dir4Increment(double &x,double &y,double moveSpeedx,double moveSpeedy)
{
	x-=moveSpeedx;
	y+=moveSpeedy;
}
bool checkHalfCollision(double x,double xbar,double barLenght)
{
	if(x<=xbar+barLenght/2) return true;
	else return false;
}
void initializeStruct(brick bricks[brickNumberLine][brickNumberColumn])
{
	for(int i=0;i<brickNumberLine;i++)
		for(int j=0;j<brickNumberColumn;j++)
		{
			bricks[i][j].magnet=false;
			bricks[i][j].minusBall=false;
			bricks[i][j].minusBar=false;
			bricks[i][j].plusBall=false;
			bricks[i][j].plusBar=false;
			bricks[i][j].speedUp=false;
			bricks[i][j].speedDown=false;
		}
}
void loadBricks(const char *filename,brick bricks[brickNumberLine][brickNumberColumn])
{
	std::ifstream openfile(filename);
		for(int i=0;i<brickNumberLine;i++)
			for(int j=0;j<brickNumberColumn;j++)
				openfile >> bricks[i][j].exist;
}


int main(int argc,char* argv[]){
	FreeConsole();
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_EVENT eve;
	ALLEGRO_TIMER *timer;
	


	bool done = false,played=false,done2=false,win=false,ex=false;
	short DIR=1,k=1,auxDIR;
	int magnetNumber=100,plusBallNumber=100,minusBallNumber=100,plusBarNumber=100,minusBarNumber=100,speedUpNumber=100,speedDownNumber=100;
	double x = 400, y = 320, xbar=360, barLenght=100, barDepth=20, moveSpeedx=5, moveSpeedy=5, radius=5, moveSpeed=10, bricksNumber=0;
	int plusBally,plusBallx,minusBally,minusBallx,plusBary,plusBarx,minusBary,minusBarx,speedUpy,speedUpx,speedDowny,speedDownx,barMagnety,barMagnetx;
	bool redraw = true,magnet=false,magnetActive=false,plusBar=false,minusBar=false,plusBall=false,minusBall=false,speedUp=false,speedDown=false;
	brick bricks[brickNumberLine][brickNumberColumn];
	initializeStruct(bricks);
	srand(time(NULL));
	loadBricks("bricks.txt",bricks);

	for(int i=0;i<brickNumberLine;i++)
		for(int j=0;j<brickNumberColumn;j++)
		{
			bricks[i][j].x=j*brickLenght;
			bricks[i][j].y=i*brickDepth;
			int k=rand()%1+2;
			bricks[i][j].r=i*3+j*7;
			bricks[i][j].g=i*6+j*11;
			bricks[i][j].b=i*2+j*9;
			if(bricks[i][j].exist)
			{
			if(magnetNumber==0) bricks[i][j].magnet=false;
			else 
			{
					
					bricks[i][j].magnet=rand()%k-1;
					if(bricks[i][j].magnet)
					{
						magnetNumber--;
					}
				
			}
			if(!bricks[i][j].magnet)
			{
				if(plusBarNumber==0) bricks[i][j].plusBar=false;
						else
						{
							bricks[i][j].plusBar=rand()%k-1;
							if(bricks[i][j].plusBar)
							{
								plusBarNumber--;
							}
						}
			}
			if(!bricks[i][j].magnet && !bricks[i][j].plusBar)
			{
				if(minusBarNumber==0) bricks[i][j].minusBar=false;
				else
				{
					bricks[i][j].minusBar=rand()%k-1;
					if(bricks[i][j].minusBar)
					{
						minusBarNumber--;
					}
				}
			}
			if(!bricks[i][j].magnet && !bricks[i][j].plusBar && !bricks[i][j].minusBar)
			{
				if(plusBallNumber==0) bricks[i][j].plusBall=false;
				else
				{
					bricks[i][j].plusBall=rand()%k-1;
					if(bricks[i][j].plusBall)
					{
						plusBallNumber--;
					}
				}
			}
			if(!bricks[i][j].magnet && !bricks[i][j].plusBar && !bricks[i][j].minusBar && !bricks[i][j].plusBall)
			{
				if(minusBallNumber==0) bricks[i][j].minusBall=false;
				else
				{
					bricks[i][j].minusBall=rand()%k-1;
					if(bricks[i][j].minusBall)
					{
						minusBallNumber--;
					}
				}
			}
			if(!bricks[i][j].magnet && !bricks[i][j].plusBar && !bricks[i][j].minusBar && !bricks[i][j].plusBall && !bricks[i][j].minusBall)
			{
				if(speedUpNumber==0) bricks[i][j].speedUp=false;
				else
				{
					bricks[i][j].speedUp=rand()%k-1;
					if(bricks[i][j].speedUp)
					{
						speedUpNumber--;
					}
				}
			}
			if(!bricks[i][j].magnet && !bricks[i][j].plusBar && !bricks[i][j].minusBar && !bricks[i][j].plusBall && !bricks[i][j].minusBall && !bricks[i][j].speedUp)
			{
				if(speedDownNumber==0) bricks[i][j].speedDown=false;
				else
				{
					bricks[i][j].speedDown=rand()%k-1;
					if(bricks[i][j].speedDown)
					{
						speedDownNumber--;
					}
				}
			}
			
			bricksNumber++;
			}
		}

	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *font = al_load_font("font.ttf",20,NULL);
	al_init_primitives_addon();
	al_init_image_addon();
	ALLEGRO_BITMAP *sign=al_load_bitmap("sign.bmp");
	al_install_keyboard();
	al_install_mouse();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(5);
	ALLEGRO_SAMPLE *magnetSound = al_load_sample("magnet.wav");
	ALLEGRO_SAMPLE *poingSound = al_load_sample("poing.wav");
	ALLEGRO_SAMPLE *brickSound = al_load_sample("brick.wav");
	ALLEGRO_SAMPLE *wallSound = al_load_sample("wall.wav");
	ALLEGRO_SAMPLE *song = al_load_sample("intro.ogg");
	ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance,ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance,al_get_default_mixer());
	event_queue = al_create_event_queue();
	timer = al_create_timer(1 / 60.0);
	display=al_create_display(screenWidth,screenHeight);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_register_event_source(event_queue , al_get_keyboard_event_source());
	al_register_event_source(event_queue , al_get_mouse_event_source());
	al_register_event_source(event_queue , al_get_timer_event_source(timer));
	al_play_sample_instance(songInstance);

	al_draw_text(font,al_map_rgb(255,10,10),screenWidth/2,screenHeight-100,ALLEGRO_ALIGN_CENTRE,"CLICK HERE TO START");

	al_draw_filled_rectangle(10,10,30,30,al_map_rgb(130,130,130));
	al_draw_text(font,al_map_rgb(0,255,0),40,10,NULL," - Magnet");

	al_draw_filled_rectangle(10,50,30,70,al_map_rgb(130,130,130));
	al_draw_line(20,52,20,68,al_map_rgb(0,0,0),2);
	al_draw_line(12,60,28,60,al_map_rgb(0,0,0),2);
	al_draw_text(font,al_map_rgb(0,254,0),40,50,NULL," - Longer Bar");

	al_draw_filled_rectangle(10,90,30,110,al_map_rgb(130,130,130));
	al_draw_line(12,100,28,100,al_map_rgb(0,0,0),1);
	//al_
	al_flip_display();
	while(!done2)
	{
		al_wait_for_event(event_queue , &eve);
		if(eve.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if((eve.mouse.button & 1) && eve.mouse.x>=screenWidth/2-100 && eve.mouse.x<=screenWidth/2 + 100 && eve.mouse.y>=screenHeight-100 && eve.mouse.y<=screenHeight-80)
			done2=true;
		}
		if(eve.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if(eve.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				done=true;
				done2=true;
			}
		}
	}

	al_hide_mouse_cursor(display);	
	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
	al_start_timer(timer);
	while(!done)
	{
		al_wait_for_event(event_queue , &eve);
		if(eve.type == ALLEGRO_EVENT_KEY_DOWN)
			if(eve.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
			
				done=true;
				ex=true;
			}
			else
				done=false;
		else if(eve.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)

		{
			if((eve.mouse.button & 1) && DIR==0)
			{
				DIR=auxDIR;
				played=false;
				magnetActive=false;
			}
		}

		else if(eve.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if(DIR==1) dir1Increment(x,y,moveSpeedx,moveSpeedy);
			else if(DIR==2) dir2Increment(x,y,moveSpeedx,moveSpeedy);
			else if(DIR==3) dir3Increment(x,y,moveSpeedx,moveSpeedy);
			else if(DIR==4) dir4Increment(x,y,moveSpeedx,moveSpeedy);
			if(y+radius>=screenHeight) done=true;
			if(magnet)
			{
				barMagnety+=2;
				if(barMagnety+10>=ybar && barMagnetx+10>=xbar && barMagnetx<=xbar+barLenght)
				{
					magnetActive=true;
					magnet=false;
				}
				if(barMagnety>ybar)
				{
					magnet=false;
				}
			}
			if(plusBar)
			{
				plusBary+=2;
				if(plusBary+10>=ybar && plusBarx+10>=xbar && plusBarx<=xbar+barLenght)
				{
					if(barLenght==100)
						barLenght=200;
					else if(barLenght==50)
						barLenght=100;
					plusBar=false;
				}
				if(plusBary>ybar)
				{
					plusBar=false;
				}
			}
			if(speedUp)
			{
				speedUpy+=2;
				if(speedUpy+10>=ybar && speedUpx+10>=xbar && speedUpx<=xbar+barLenght)
				{
					if(moveSpeed==10)
						moveSpeed=15;
					else if(moveSpeed==3)
						moveSpeed=10;
					speedUp=false;
				}
				if(speedUpy>ybar)
				{
					speedUp=false;
				}
			}
			if(speedDown)
			{
				speedDowny+=2;
				if(speedDowny+10>=ybar && speedDownx+10>=xbar && speedDownx<=xbar+barLenght)
				{
					if(moveSpeed==10)
					
						moveSpeed=3;
						
					else if(moveSpeed==15)
						moveSpeed=10;
					speedDown=false;
				}
				if(speedDowny>ybar)
				{
					speedDown=false;
				}
			}
			if(minusBar)
			{
				minusBary+=2;
				if(minusBary+10>=ybar && minusBarx+10>=xbar && minusBarx<=xbar+barLenght)
				{
					if(barLenght==200)
						barLenght=100;
					else if(barLenght==100)
						barLenght=50;
					minusBar=false;
				}
				if(minusBary>ybar)
				{
					minusBar=false;
				}
			}
			if(plusBall)
			{
				plusBally+=2;
				if(plusBally+10>=ybar && plusBallx+10>=xbar && plusBallx<=xbar+barLenght)
				{
					if(radius==5)
						radius=10;
					else if(radius==3)
						radius=5;
					plusBall=false;
				}
				if(plusBally>ybar)
				{
					plusBall=false;
				}
			}
			if(minusBall)
			{
				minusBally+=2;
				if(minusBally+10>=ybar && minusBallx+10>=xbar && minusBallx<=xbar+barLenght)
				{
					if(radius==5)
						radius=3;
					else if(radius==10)
						radius=5;
					minusBall=false;
				}
				if(minusBally>ybar)
				{
					minusBall=false;
				}
			}
			
			for(int i=0;i<brickNumberLine;i++)
				for(int j=0;j<brickNumberColumn;j++)
				{
					if(bricks[i][j].exist)
					{
					bool hit=false;
					if(y-radius<=bricks[i][j].y+brickDepth && y-radius>=bricks[i][j].y  && x>=bricks[i][j].x && x<=bricks[i][j].x+brickLenght && bricks[i][j].exist==1)
					{
						bricks[i][j].exist=0;
						DIR==2?DIR=1:DIR=4;
						hit=true;
					}
					if(x-radius<=bricks[i][j].x+brickLenght && x-radius>=bricks[i][j].x && y>=bricks[i][j].y && y<=bricks[i][j].y+brickDepth && bricks[i][j].exist==1)
					{
						bricks[i][j].exist=0;
						DIR==4?DIR=1:DIR=2;
						hit=true;
					}
					if(x+radius>=bricks[i][j].x && x+radius<=bricks[i][j].x+brickLenght && y>=bricks[i][j].y && y<=bricks[i][j].y+brickDepth && bricks[i][j].exist==1)
					{
						bricks[i][j].exist=0;
						DIR==2?DIR=3:DIR=4;
						hit=true;
					}
					if(y+radius>=bricks[i][j].y && y+radius<=bricks[i][j].y && x>=bricks[i][j].x && x<=bricks[i][j].x+brickLenght && bricks[i][j].exist==1)
					{
						bricks[i][j].exist=0;
						DIR==1?DIR=2:DIR=3;
						hit=true;
					}
					if(sqrt(pow(x-bricks[i][j].x,2) + pow(y-bricks[i][j].y,2))<=radius && bricks[i][j].exist==1) {DIR=3;bricks[i][j].exist=0;hit=true;}
					if(sqrt(pow(x-bricks[i][j].x-brickLenght,2) + pow(y-bricks[i][j].y,2))<=radius && bricks[i][j].exist==1) {DIR=2;bricks[i][j].exist=0;hit=true;}
					if(sqrt(pow(x-bricks[i][j].x-brickLenght,2) + pow(y-bricks[i][j].y-brickDepth,2))<=radius && bricks[i][j].exist==1) {DIR=1;bricks[i][j].exist=0;hit=true;}
					if(sqrt(pow(x-bricks[i][j].x,2) + pow(y-bricks[i][j].y-brickDepth,2))<=radius && bricks[i][j].exist==1) {DIR=4;bricks[i][j].exist=0;hit=true;}
					if(hit)
					{
						bricksNumber--;
						
						if(bricksNumber==0)
						{
								done=true;
								win=true;
								

						}
						al_play_sample(brickSound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
						if(bricks[i][j].magnet && !magnet)
						{
							magnet=true;
							barMagnetx=bricks[i][j].x;
							barMagnety=bricks[i][j].y;
						}
						else if(bricks[i][j].plusBar && !plusBar)
						{
							plusBar=true;
							plusBarx=bricks[i][j].x;
							plusBary=bricks[i][j].y;

						}
						else if(bricks[i][j].minusBar && !minusBar)
						{
							minusBar=true;
							minusBarx=bricks[i][j].x;
							minusBary=bricks[i][j].y;
						}
						else if(bricks[i][j].plusBall && !plusBall)
						{
							plusBall=true;
							plusBallx=bricks[i][j].x;
							plusBally=bricks[i][j].y;
						}
						else if(bricks[i][j].minusBall && !minusBall)
						{
							minusBall=true;
							minusBallx=bricks[i][j].x;
							minusBally=bricks[i][j].y;
						}
						else if(bricks[i][j].speedUp && !speedUp)
						{
							speedUp=true;
							speedUpx=bricks[i][j].x;
							speedUpy=bricks[i][j].y;
						}
						else if(bricks[i][j].speedDown && !speedDown)
						{
							speedDown=true;
							speedDownx=bricks[i][j].x;
							speedDowny=bricks[i][j].y;
						}
					}
					}
				}


			if(checkBarCollision(xbar,x,y,radius,barLenght,moveSpeedy))
			{
				
				if(checkHalfCollision(x,xbar,barLenght))
				{
					moveSpeedx=(barLenght/2 - (x-xbar))*moveSpeed/barLenght;
					moveSpeedy=abs(moveSpeed-moveSpeedx);
					DIR=3;
				}
				if(!checkHalfCollision(x,xbar,barLenght))
				{
					moveSpeedx=abs((x-(xbar+barLenght/2))*moveSpeed/barLenght);
					moveSpeedy=abs(moveSpeed-moveSpeedx);
					DIR=2;
				}
				if(magnetActive)
				{
					if(!played)
					{
						al_play_sample(magnetSound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
						played=true;
					}
					auxDIR=DIR;
					DIR=0;
					
				}
				else
					al_play_sample(poingSound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
			}
			if(checkWallCollision(x,y,radius))
			{
				al_play_sample(wallSound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
				if(DIR==1) DIR=4;
				else if(DIR==2) DIR=3;
				else if(DIR==3) DIR=2;
				else if(DIR==4) DIR=1;
			}
			if(checkCeilingCollision(x,y,radius)) 
			{
				DIR==2?DIR=1:DIR=4;
				al_play_sample(wallSound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);

			}
		}
		else if(eve.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			double dif=x-xbar;
			if(eve.mouse.x+barLenght>=screenWidth) xbar=screenWidth-barLenght;
			else xbar=eve.mouse.x;
			if(DIR==0)
			{
				x=xbar+dif;
			}
		}
		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));
			for(int i=0;i<brickNumberLine;i++)
				for(int j=0;j<brickNumberColumn;j++)
				{
					if(bricks[i][j].exist)
					{
						al_draw_filled_rounded_rectangle(bricks[i][j].x+1,bricks[i][j].y+1,bricks[i][j].x+brickLenght-1,bricks[i][j].y+brickDepth-1,3,3,al_map_rgb(bricks[i][j].r,bricks[i][j].g,bricks[i][j].b));
						
					}
				}
			if(magnet)
			{
				al_draw_filled_rectangle(barMagnetx,barMagnety,barMagnetx+10,barMagnety+10,al_map_rgb(130,130,130));
			}
			if(plusBar)
			{
				al_draw_filled_rectangle(plusBarx,plusBary,plusBarx+10,plusBary+10,al_map_rgb(130,130,130));
				al_draw_line(plusBarx+5,plusBary+1,plusBarx+5,plusBary+9,al_map_rgb(0,0,0),1);
				al_draw_line(plusBarx+1,plusBary+5,plusBarx+9,plusBary+5,al_map_rgb(0,0,0),1);
				
			}
			if(minusBar)
			{
				al_draw_filled_rectangle(minusBarx,minusBary,minusBarx+10,minusBary+10,al_map_rgb(130,130,130));
				al_draw_line(minusBarx+1,minusBary+5,minusBarx+9,minusBary+5,al_map_rgb(0,0,0),1);
			}
			if(plusBall)
			{
				al_draw_filled_rectangle(plusBallx,plusBally,plusBallx+10,plusBally+10,al_map_rgb(130,130,130));
				al_draw_filled_circle(plusBallx+2,plusBally+5,1,al_map_rgb(0,0,0));
				al_draw_filled_circle(plusBallx+7,plusBally+5,2,al_map_rgb(0,0,0));
			}
			if(minusBall)
			{
				al_draw_filled_rectangle(minusBallx,minusBally,minusBallx+10,minusBally+10,al_map_rgb(130,130,130));
				al_draw_filled_circle(minusBallx+7,minusBally+5,1,al_map_rgb(0,0,0));
				al_draw_filled_circle(minusBallx+2,minusBally+5,2,al_map_rgb(0,0,0));
			}
			if(speedUp)
			{
				al_draw_filled_rectangle(speedUpx,speedUpy,speedUpx+10,speedUpy+10,al_map_rgb(130,130,130));
				al_draw_line(speedUpx,speedUpy,speedUpx+5,speedUpy+5,al_map_rgb(0,0,0),1);
				al_draw_line(speedUpx+5,speedUpy+5,speedUpx,speedUpy+10,al_map_rgb(0,0,0),1);
				al_draw_line(speedUpx+5,speedUpy,speedUpx+10,speedUpy+5,al_map_rgb(0,0,0),1);
				al_draw_line(speedUpx+10,speedUpy+5,speedUpx+5,speedUpy+10,al_map_rgb(0,0,0),1);
			}
			if(speedDown)
			{
				al_draw_filled_rectangle(speedUpx,speedUpy,speedUpx+10,speedUpy+10,al_map_rgb(130,130,130));
				al_draw_line(speedDownx+5,speedDowny,speedDownx,speedDowny+5,al_map_rgb(0,0,0),1);
				al_draw_line(speedDownx,speedDowny+5,speedDownx+5,speedDowny+10,al_map_rgb(0,0,0),1);
				al_draw_line(speedDownx+10,speedDowny,speedDownx+5,speedDowny+5,al_map_rgb(0,0,0),1);
				al_draw_line(speedDownx+5,speedDowny+5,speedDownx+10,speedDowny+10,al_map_rgb(0,0,0),1);
			}

			al_draw_filled_rounded_rectangle(xbar,ybar,xbar+barLenght,ybar+barDepth,2,2,al_map_rgb(0,0,255));
			al_draw_filled_circle(x,y,radius,al_map_rgb(rand()%255,rand()%255,rand()%255));
			al_flip_display();
		}

			
		
}
al_clear_to_color(al_map_rgb(0,0,0));
if(!ex)
{
if(win)
{
	al_draw_text(font,al_map_rgb(0,0,255),screenWidth/2,screenHeight/2,ALLEGRO_ALIGN_CENTRE,"- GAME OVER -    - YOU WIN -");
}
else
{
	al_draw_text(font,al_map_rgb(0,0,255),screenWidth/2,screenHeight/2,ALLEGRO_ALIGN_CENTRE,"- GAME OVER -    - YOU LOSE -");
}
al_flip_display();
al_rest(4);
}

		return 0;
		}