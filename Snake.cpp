#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeparticle.h>


// Pointers to the HGE objects we will use
hgeSprite*			PRAVILA;
hgeSprite*			spr;
hgeSprite*			spr1_my_name;
hgeSprite*			sprite_apple;
hgeSprite*			rad_1;
hgeSprite*			rad_2;
hgeSprite*			rad_3;
hgeSprite*			rad_4;
hgeSprite*			rad_5;
hgeSprite*			rad_6;
hgeSprite*			sPODAROK;
hgeFont*			fnt;
hgeSprite*			sTEXTURE;
hgeSprite*			sThe_end;
hgeSprite*			sGame_over;

// Handles for HGE resourcces
HTEXTURE			hPRAVILA;
HTEXTURE			hGame_over;
HTEXTURE			TEXTURE;
HTEXTURE			theend;
HTEXTURE			tex[100];
HTEXTURE			tex1;
HTEXTURE			tex_apple;
HTEXTURE			RAD1;
HTEXTURE			hPODAROK;

HEFFECT				snd;
HEFFECT				fon_snd;

HGE *hge=0;
int dir;
int chek=0;
int num=3;

class Help_class
{
public:
	float x_prav,y_prav;
	Help_class()
	{
		x_prav=0,y_prav=0;
	}
	void Move_prav()
	{
		hge->System_SetState(HGE_FPS,100);
		PRAVILA->Render(x_prav,y_prav);
		if (hge->Input_GetKeyState(HGEK_SPACE))
		{
			y_prav+=1000;
		}
	}

	void boom() 
	{
		hge->Effect_PlayEx(snd,20,0,1,false);
	}
	void fon_music() 
	{
		hge->Effect_PlayEx(fon_snd,10,0,1,false);
	}
	void THE_END()
	{
		sThe_end->Render(0,0);
	}
	void Game_over()
	{
		sGame_over->Render(0,0);
	}
};

class snake
{
public:
	float x;
	float y;

	void Print_Field()
	{
		//pole 20*20
		for (float i=0; i<600; i=i+30)
		{
			hge->Gfx_RenderLine(0,i,600,i,0xFFFFFFFF);
		}

		for (float j=0; j<600; j=j+30)
		{
			hge->Gfx_RenderLine(j,0,j,600,0xFFFFFFFF);
		}
	}

	void DrawSnake()
	{
		for(int i=0;i<num;i++)
		{
			spr->Render(s[i].x,s[i].y);
		}
	}

} s[100];


class Fructs
{
public:
	
	virtual void New()=0;
	
	virtual void Draw()=0;
	
	~ Fructs(){}
};
class Apple:public Fructs
{
public:

	float x_apple;
	float y_apple;
	
	virtual void New()
	{
	float mass[18]={30, 60 ,90 ,120 ,150 ,180 ,210, 240, 270, 330, 360, 390 ,420 ,450 ,480, 510, 540 ,570};
	
		x_apple=mass[rand()%17];
		y_apple=mass[rand()%17];
		
	}

	virtual void Draw()
	{	
		sprite_apple->Render(x_apple,y_apple);
	}

	virtual ~Apple(){}

}m[100];

class Podarok:public Fructs
{
public:
	float x_podarok;
	float y_podarok;
	Podarok()
	{
		x_podarok=300;
		y_podarok=270;
	}
	virtual void New()
	{
		x_podarok+=600;
		y_podarok+=600;
	}
	virtual void Draw()
	{
		sPODAROK->Render(x_podarok,y_podarok);
	}
	virtual ~Podarok(){}
};



Fructs*a=new Apple;
Fructs*p=new Podarok;
snake S;
Podarok P;
Help_class H;


void MoveSnake()
{
	//для хвоста змейки
	for (int i=num;i>0;--i)
	{
		s[i].x=s[i-1].x;
		s[i].y=s[i-1].y;
	}
	float dt=hge->Timer_GetDelta();

	if (dir==0) s[0].y+=30;//для головы змеи
	if (dir==1) s[0].x-=30;
	if (dir==2) s[0].x+=30;
	if (dir==3) s[0].y-=30;

	if (hge->Input_GetKeyState(HGEK_LEFT)) {dir=1;}
	if (hge->Input_GetKeyState(HGEK_RIGHT)){dir=2;}
	if (hge->Input_GetKeyState(HGEK_UP))   {dir=3;} 
	if (hge->Input_GetKeyState(HGEK_DOWN)) {dir=0;}

	for (int i=0;i<5;i++)//подбор яблок
	{
		if ((s[0].x==m[i].x_apple)&&(s[0].y==m[i].y_apple))//когда голова наезжает на яблоко 
		{
			num++;
			m[i].New();
			//змейка++
			//m[0].New();//			New();//рандомное появление яблока
			//H.eda();//звук 
		}
	}

	if (s[0].x>570) {dir=1;H.boom();} if (s[0].x<0) {dir=2;H.boom();}//выход за поля+звук
	if (s[0].y>570) {dir=3;H.boom();} if (s[0].y<0) {dir=0;H.boom();}

	for (int i=1;i<num;i++)//наезд на свое тело
	{
		if (s[0].x==s[i].x && s[0].y==s[i].y) 
			num=3;
	}

	for (int i=1;i<num;i++)//наезд на радиацию
		if ((s[0].x==330 && s[0].y==240)||(s[0].x==330 && s[0].y==270)||(s[0].x==330 && s[0].y==300)||(s[0].x==270 && s[0].y==240)||(s[0].x==270 && s[0].y==270)||(s[0].x==270 && s[0].y==300))
		{ 
			num=1;
			
		}

		if ((s[0].x==P.x_podarok) && (s[0].y==P.y_podarok))//подобрать подарок
		{
			num+=10; //змейка+10
			p->New();
		}
}

bool FrameFunc()
{
	float dt=hge->Timer_GetDelta();
	if (hge->Input_GetKeyState(HGEK_ESCAPE)){ return true;}

	MoveSnake();

	return false;
}

float nuM(){return num;}
bool RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	
	sTEXTURE->Render(0,0);
	spr1_my_name->Render(540,570);
	/*a->Draw();*/
	S.DrawSnake();
	S.Print_Field();
	
	p->Draw();
	/*F.DrawPodarok();*/
	for (int i=0;i<5;i++)//рисуем 10 яблок
	{
		a->Draw();
		m[i].Draw();
	}
	rad_1->Render(330,240);
	rad_2->Render(330,270);
	rad_6->Render(330,300);
	rad_3->Render(270,240);
	rad_4->Render(270,270);
	rad_5->Render(270,300);
	fnt->printf(360,6, HGETEXT_LEFT, "num_of_Snake: %.f", nuM());//ВЫВОД РАЗМЕР ЗМЕИ
	if(num==1)
	{
		H.Game_over();
		s[0].x=500;
		s[0].y=300;
	}
	if(num>=35)//конец игры если змейка состоит из 35 квадратиков
	{
		H.THE_END();
		s[0].x=500;
		s[0].y=300;
	}	
	H.Move_prav();
	hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, "hge_tut03.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Snake ");
	hge->System_SetState(HGE_FPS,10);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 600);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);


	if(hge->System_Initiate()) {
		hGame_over=hge->Texture_Load("resource/gv.png");
		hPRAVILA=hge->Texture_Load("resource/do_igri1.png");
		TEXTURE=hge->Texture_Load("resource/3215.jpg");//fon
		theend=hge->Texture_Load("resource/happy_NE.jpg");//okon4anie_igri
		snd=hge->Effect_Load("resource/udar.mp3");//удар в стенку
		fon_snd=hge->Effect_Load("resource/fon_muz.mp3");//съела яблоко
		tex[0]=hge->Texture_Load("resource/zm1.jpg");//звено змейки
		tex1=hge->Texture_Load("resource/alx22.png");//
		RAD1=hge->Texture_Load("resource/rad.png");//радиация
		tex_apple=hge->Texture_Load("resource/apple.png");//яблоко;
		hPODAROK=hge->Texture_Load("resource/podarokk.png");//podarok

		if(!snd || !tex)
		{
			hge->System_Shutdown();
			hge->Release();
			return 0;
		}
		sGame_over=new hgeSprite(hGame_over,0,0,600,600);
		PRAVILA=new hgeSprite(hPRAVILA,0,0,600,600);
		sTEXTURE=new hgeSprite(TEXTURE,0,0,600,600);//background
		sThe_end=new hgeSprite(theend,0,0,600,600);
		spr=new hgeSprite(tex[0], 0, 0, 29, 29);
		sprite_apple=new hgeSprite(tex_apple,0,0,29,29);
		spr1_my_name=new hgeSprite(tex1, 0, 0, 60, 29);
		rad_1=new hgeSprite(RAD1,0,0,29,29);
		rad_2=new hgeSprite(RAD1,0,0,29,29);
		rad_3=new hgeSprite(RAD1,0,0,29,29);
		rad_4=new hgeSprite(RAD1,0,0,29,29);
		rad_5=new hgeSprite(RAD1,0,0,29,29);
		rad_6=new hgeSprite(RAD1,0,0,29,29);
		sPODAROK=new hgeSprite(hPODAROK,0,0,29,29);
		// загрузка шрифта
		fnt=new hgeFont("font1.fnt");
		H.fon_music();//fom_muz
		hge->System_Start();
		

		delete fnt;
		delete spr;
		hge->Texture_Free(tex[0]);
		hge->Effect_Free(snd);
	}

	hge->System_Shutdown();
	hge->Release();
	return 0;
}

