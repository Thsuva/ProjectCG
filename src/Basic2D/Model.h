///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

//  world geometry
// eye    (0,0,0)
// center (0,0,-1)
// up     (0,1,0)
// left -1, right 1
//  top & bottom centered on Y to match the window aspect ratio
// clipping planes: 1, 5.1

#pragma warning(disable: 4996)

#include <vector>
#include <time.h>
#include <math.h>
#include <list>

#define PI 3.141592654

// A class for storing vetices
//  Vertex
class Vertex {
public:

	float x, y, z;     // 3d coordinate of the vertex
	float Nx, Ny, Nz;     // Normal to the surface (if present)
	float u, v;						// texture coordinates
	float r, g, b;				// Color (0.0 -  1.0)

	Vertex() {}
	Vertex(float x, float y, float z) : x(x), y(y), z(z) { }
	Vertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z),
		u(u), v(v) { }
	inline void SetP(float x, float y, float z) {
		this->x = x; this->y = y;
		this->z = z;
	}
	inline void SetN(float x, float y, float z) { Nx = x; Ny = y; Nz = z; }
};

class Bullet {

public:
	std::vector<Vertex> bullet;  // floating personaggio

	bool alive;
	// per i movimenti
	float bullet_x;
	float bullet_y;
	float bullet_horizontal_transl;
	float bullet_vertical_transl;
	float bullet_vel_h;
	float b_height = 0.01;
	float b_width = 0.01;

public:
	//  methods
	Bullet(float b_x, float b_y, float trasl_x, float trasl_y, float my_vel) 
	{
		alive = true;
		bullet_x = b_x;
		bullet_y = b_y;
		bullet_horizontal_transl = trasl_x;
		bullet_vertical_transl = trasl_y;
		bullet_vel_h = my_vel;
		bullet.clear();
		bullet.push_back(Vertex(-0.005, 0.045, -1, 0, 0));
		bullet.push_back(Vertex(0.005, 0.045, -1, 1, 0));
		bullet.push_back(Vertex(0.005, 0.055, -1, 1, 1));
		bullet.push_back(Vertex(-0.005, 0.055, -1, 0, 1));

	}
	void Update_position();
	void Die();
	int round(double value);
	~Bullet() {
	}

};



class Character {

public:
	std::vector<Vertex> personaggio;  // floating personaggio

	bool alive;

	// per i movimenti
	int character_direction;
	float player_x;
	float player_y;
	float player_horizontal_transl;
	float player_vertical_transl;
	float p_height = 0.1;
	float p_width = 0.1;
	float vel_h;
	float vel_v;
	float MAX_VEL_H = 0.00025 * 2;
	float MAX_VEL_V = 0.00025 * 50;
	
	//collision tile of feet
	int left_tile = 19;
	int middle_feet_tile=19;
	int right_tile=20;
	

	//collision tile of body
	int bottom_tile = 11;
	int middle_body_tile = 11;
	int top_tile = 10;


	int weapon = 0;

public:
	//  methods
	Character() {
		alive = true;
		// inizialmente il personaggio è orientato verso destra
		character_direction = -1;
	}
	~Character() {
	}

	// inizio nostre funzioni
	void MoveOrCollide(double nvel_h);
	void Move_up_down_personaggio(int dir);
	void Jump_personaggio();
	void Setup_position();
	void Gravity();
	bool Is_on_tile();
	void Die();
	void Convert_coordinate_for_translation();
	Bullet shoot();
	int round(double value);
};

class Personaggio : public Character
{
	public:
		Personaggio() : Character(){

			player_x = 1;
			player_y = 0.55;
			vel_h = 0;
			vel_v = 0;
			personaggio.clear();
			personaggio.push_back(Vertex(-0.05, 0, -1, 0, 0));
			personaggio.push_back(Vertex(0.05, 0, -1, 1, 0));
			personaggio.push_back(Vertex(0.05, 0.1, -1, 1, 1));
			personaggio.push_back(Vertex(-0.05, 0.1, -1, 0, 1));

		}
		~Personaggio(){
		}

};

class Enemy : public Character
{
	public:
		int id;
		
		Enemy(int x, int y, int my_id) : Character()
		{
			id = my_id;
			player_x = .05 * (x+1);
			player_y = .05 * (y+1);
			vel_h = 0;
			vel_v = 0;
			personaggio.clear();
			personaggio.push_back(Vertex(-0.05, 0, -1, 0, 0));
			personaggio.push_back(Vertex(0.05, 0, -1, 1, 0));
			personaggio.push_back(Vertex(0.05, 0.1, -1, 1, 1));
			personaggio.push_back(Vertex(-0.05, 0.1, -1, 0, 1));
		}
		void random_move(float hero_player_x, int level_width);
		~Enemy() {
		}
};

class MyModel {
public:
	//  general data for window and input controls
	HDC			hDC;		        // Private GDI Device Context
	HGLRC		hRC;		        // Permanent Rendering Context
	HWND		hWnd;		        // Holds Our Window Handle
	HINSTANCE	hInstance;		// Holds The Instance Of The Application

	bool	keys[256];			// Array Used For The Keyboard Routine
	bool	active;		      // Window Active Flag Set To TRUE By Default
	bool	fullscreen;	    // Fullscreen Flag 
	bool game_started;
	int texture_delay;
	Personaggio Player;
	std::list<Enemy> enemy_list;
	std::list<Bullet> bullet_list;

private:
	//  projection limits in X and Y: x in [-plx, plx], y in [-ply, ply]
	double plx, ply;
	int Wheight, Wwidth;  // window dimensions in pixels

	int frames;           // n. of frames
	double frameTime;     // for fps computation
	double fps;

	//  model data
	std::vector<Vertex> Background;   // background
	
	std::vector<Vertex> tile;		  // blocco base
	clock_t Tstamp, Tstart;
	double Full_elapsed;  // elapsed time in seconds from the beginning of the program
	double Shot_elapsed;

	GLuint	texture[28];			// Storage For 28 Textures!
	GLuint	base;				// Base Display List For The Font Set

	std::string level = "";
	int num_of_screens = 3;
	int screen_width = 40;
	int level_height = 24;

public:
	//  methods
	MyModel() : hDC(NULL), hRC(NULL), hWnd(NULL), active(true),
		fullscreen(true), frames(0), fps(0){

		game_started = false;
		texture_delay = 0;
		Shot_elapsed = 1;
		Background.clear();

		// i primi 2 valori dei vertex sono stati cambiati per fittare il 16/9
		// (prima erano tutti -1 e 1 per generare un quadrato di lato 2 centrato in 0,0)
		Background.push_back(Vertex(-1, -1.8, -5, 0, 0));
		Background.push_back(Vertex(5, -1.8, -5, 1, 0));
		Background.push_back(Vertex(5, 1.8, -5, 1, 1));
		Background.push_back(Vertex(-1, 1.8, -5, 0, 1));

		tile.clear();
		tile.push_back(Vertex(-.025, 0, -4, 0, 0));
		tile.push_back(Vertex(.025, 0, -4, 1, 0));
		tile.push_back(Vertex(.025, .05, -4, 1, 1));
		tile.push_back(Vertex(-.025, .05, -4, 0, 1));

		this->Tstart = this->Tstamp = clock();
		this->Full_elapsed = 0;
		this->frameTime = 0;

	}
	~MyModel() {
		this->KillFont();
	}
	bool DrawGLScene(void);
	bool InitGL(void);
	void ReSizeGLScene(int width, int height);
	void glPrint(const char *fmt, ...);			// Custom GL "Print" Routine

	// inizio nostre funzioni
	void Set_level();
	char Get_tile(int x, int y);
	void Set_tile(int x, int y, char c);
	int Get_level_width();
	int Get_level_height();
	void Check_collisions();
	double Get_last_shot_elapsed();
	void Set_shot_elapsed();

private:
	bool LoadGLTextures(void);
	void BuildFont(void);
	void KillFont(void);
	//  Conversion from pixel distance to float X and Y distance
	inline float PixToCoord_X(int pix)
	{
		return (2.0f * (float)pix * (float)plx) / (float)Wwidth;
	}
	inline float PixToCoord_Y(int pix)
	{
		return (2.0f * (float)pix * (float)ply) / (float)Wheight;
	}

};

extern class MyModel Data;