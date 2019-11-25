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


class Personaggio {

public:
	std::vector<Vertex> personaggio;  // floating personaggio

	// per i movimenti
	float last_mov_pers_h;
	float last_mov_pers_v;
	float vel_h;
	float vel_v;
	float MAX_VEL_H = 0.00025 * 3;
	float MAX_VEL_V = 0.00025 * 3;
	bool on_ground;

public:
	//  methods
	Personaggio() : last_mov_pers_h(0), last_mov_pers_v(0), vel_h(0), vel_v(0), on_ground(true) {

		personaggio.clear();
		personaggio.push_back(Vertex(-.05, 0, -1, 0, 0));
		personaggio.push_back(Vertex(.05, 0, -1, 1, 0));
		personaggio.push_back(Vertex(.05, .1, -1, 1, 1));
		personaggio.push_back(Vertex(-.05, .1, -1, 0, 1));


	}
	~Personaggio() {
	}

	// inizio nostre funzioni
	void MoveOrCollide(double nvel_h);
	void Move_up_down_personaggio(int dir);
	void Jump_personaggio();
	void Gravity_personaggio();
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
	Personaggio Player;

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