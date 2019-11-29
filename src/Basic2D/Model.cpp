///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "Model.h"
#include "SOIL.h"

// All Setup For OpenGL Goes Here
bool MyModel::InitGL(void)
{
	if (!this->LoadGLTextures())				// Jump To Texture Loading Routine
	{
		return false;
	}							// If Texture Didn't Load Return FALSE


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	this->BuildFont();

	// eye    (0,0,0)
	// center (0,0,-1)
	// up     (0,1,0)
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

	return true;										// Initialization Went OK
}


void MyModel::ReSizeGLScene(int width, int height)
{
	if (height == 0) height = 1;					// Prevent A Divide By Zero By
	if (width == 0) width = 1;					// Prevent A Divide By Zero By

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// orthographic projection
  // background in [-1, 1] on both x and y
  // fill th background according to the window aspect ratio
  // void WINAPI glOrtho( GLdouble left,   GLdouble right,
  //                      GLdouble bottom, GLdouble top,
  //                      GLdouble zNear,  GLdouble zFar );
	if (width >= height) {
		this->plx = 1.0;
		this->ply = (double)height / (double)width;
	}
	else {
		this->plx = (double)width / (double)height;
		this->ply = 1.0;
	}
	glOrtho(-this->plx, this->plx, -this->ply, this->ply, 1, 5.1);

	//  saving the window width and height in pixels
	this->Wheight = height;
	this->Wwidth = width;

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}



// Load Bitmaps And Convert To Textures
bool MyModel::LoadGLTextures(void)
{
	/* load an image file directly as a new OpenGL texture */
	texture[0] = SOIL_load_OGL_texture
	("../Data/skybox_hd.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	if (texture[0] == 0) return false;

	//  Load 27 personaggio textures
	char ll[200];
	for (int i = 0; i < 2; i++) {
		if (i==0)
			sprintf(ll, "../Data/fabrizio_00.png", i);
		else if (i==1)
			sprintf(ll, "../Data/blank_tile.png", i);

		this->texture[i + 1] = SOIL_load_OGL_texture(
			ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture[i + 1] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// TODO: da togliere
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return true;										// Return Success
}



bool MyModel::DrawGLScene(void)
{
	//  TIMING - start
	clock_t t = clock();
	// elapsed time in seconds from the last draw
	double elapsed = double(t - Tstamp) / (double)CLOCKS_PER_SEC;
	// elapsed time in milliseconds from the last draw
	int ms_elapsed = (int)(t - Tstamp);

	if (ms_elapsed < 10) return true;
	// elapsed time in seconds from the beginning of the program
	this->Full_elapsed = double(t - Tstart) / (double)CLOCKS_PER_SEC;
	this->frameTime += double(t - Tstamp) / (double)CLOCKS_PER_SEC;

	this->Tstamp = t;
	//  TIMING - end

	// spostate queste 3 righe
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View

	// ------------le 3 righe qui sotto servono per spostare il background in orizz di px
	// vedo se gestire o no il salto

	// gravit� mi sembra funzionante
	Player.Gravity();
	glTranslatef((float)Player.last_mov_pers_h, (float)Player.last_mov_pers_v, 0);
	// --------------fino a qui
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//  Background
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glTexCoord2f(Background[i].u, Background[i].v);
		glVertex3f(Background[i].x, Background[i].y, Background[i].z);
	}
	glEnd();

	//  Texture for the personaggio, change every 1/19 sec.
	// int texF = 1 + ((int((Full_elapsed * 19))) % 27);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	//  personaggio geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View

	// Personaggio
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glTexCoord2f(Player.personaggio[i].u, Player.personaggio[i].v);
		glVertex3f(-Player.personaggio[i].x, Player.personaggio[i].y, Player.personaggio[i].z);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	// Tiles
	for (int col = 0; col < Data.screen_width * Data.num_of_screens; col++) {
		for (int row = 0; row < Data.level_height; row++) {
			char id = Data.Get_tile(col, row);
			switch (id)
			{
			case '#':
				glBindTexture(GL_TEXTURE_2D, texture[2]);
				glBegin(GL_QUADS);
				for (int i = 0; i < 4; i++) {
					glTexCoord2f(tile[i].u, tile[i].v);
					glVertex3f(-.975+(.05*col) + tile[i].x + Player.last_mov_pers_h, .55-(.05*row) + tile[i].y + Player.last_mov_pers_v, tile[i].z);
				}
				glEnd();

				break;
			default:
				break;
			}
		}
	}

	//  Some text
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glDisable(GL_TEXTURE_2D);

	// Color
	glColor3f(1.0f, 1.0f, 1.0f);

	// Position The Text On The Screen
	glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)ply - PixToCoord_Y(21),
		-4);

	// compute fps and write text
	this->frames++;
	if (this->frames > 18) {
		this->fps = frames / frameTime;
		this->frames = 0; this->frameTime = 0;
	}
	this->glPrint("Elapsed time: %6.2f sec.  -  Fps %6.2f",
		Full_elapsed, fps);

	if (true) {
		glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(21),
			-4);
		this->glPrint("vel_v: %d, vel_h: %f",Data.Player.Is_on_tile(),Data.Player.last_mov_pers_v);
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	return true;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//  bitmap fonts
void MyModel::BuildFont(void)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(-20,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void MyModel::KillFont(void)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

void MyModel::glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}

// ------------------------------------------------------nostre funzioni

// muove il personaggio
void Personaggio::MoveOrCollide(double nvel_h)
{
	nvel_h = (abs(nvel_h) > MAX_VEL_H) ? (MAX_VEL_H * (nvel_h / abs(nvel_h))) : nvel_h;

	double npx = last_mov_pers_h + nvel_h;
	int next_pos_col = -1;

	if (nvel_h < 0)
		next_pos_col = 21 - (int)((npx) / .05);
	else if (nvel_h > 0)
		next_pos_col = 19 - (int)((npx) / .05);


	int next_pos_row_bottom = (int)(last_mov_pers_v / .05);

	if (last_mov_pers_v > 0)
		next_pos_row_bottom += 12;
	else
		next_pos_row_bottom += 11;

	int next_pos_row_middle = next_pos_row_bottom - 1;
	int next_pos_row_top = next_pos_row_middle;
	int temp_last_pers_v = (int)(last_mov_pers_v * 100000);
	if (temp_last_pers_v % 5000 != 0)
		next_pos_row_top -= 1;

	if (Data.Get_tile(next_pos_col, next_pos_row_bottom) != '#' && Data.Get_tile(next_pos_col, next_pos_row_middle) != '#' && Data.Get_tile(next_pos_col, next_pos_row_top) != '#') {
		last_mov_pers_h = npx;
		vel_h = nvel_h;
	}
	else {
		vel_h = 0;
	}
}

void Personaggio::Move_up_down_personaggio(int dir)
{
	last_mov_pers_v += .00025 * dir;
}

void Personaggio::Gravity()
{
	if (Is_on_tile()) {
		vel_v = 0;
	}
	else {
		vel_v += .00025;
		vel_v = (vel_v > MAX_VEL_V) ? MAX_VEL_V : vel_v;
		double npy = last_mov_pers_v + vel_h;

		int next_pos_row_bottom = (int)(npy / .05);

		if (npy > 0)
			next_pos_row_bottom += 12;
		else
			next_pos_row_bottom += 11;
		
		int current_pos_col_right = 21 - (int)((last_mov_pers_h) / .05);
		int current_pos_col_middle = current_pos_col_right - 1;
		int current_pos_col_left = current_pos_col_middle;
		int temp_last_pers_h = (int)(last_mov_pers_h * 100000);
		if (temp_last_pers_h% 5000 != 0) {
			current_pos_col_left -= 1;
		}

		
		if (next_pos_row_bottom == 23) {
			vel_v = 0;
			//Player.Die();
		}

		if (Data.Get_tile(current_pos_col_right, next_pos_row_bottom) == '#'
			|| Data.Get_tile(current_pos_col_middle, next_pos_row_bottom) == '#'
			|| Data.Get_tile(current_pos_col_left, next_pos_row_bottom) == '#') {
			last_mov_pers_v = ((next_pos_row_bottom - 12) * .05);
			vel_v = 0;
		}
		
		int newt_pos_row_top = next_pos_row_bottom - 2;
		if (Data.Get_tile(current_pos_col_right, newt_pos_row_top) == '#'
			|| Data.Get_tile(current_pos_col_middle, newt_pos_row_top) == '#'
			|| Data.Get_tile(current_pos_col_left, newt_pos_row_top) == '#') {
			last_mov_pers_v = ((next_pos_row_bottom - 10.99) * .05);;
			vel_v = 0;
		}
		
	}
	last_mov_pers_v += vel_v;
}

void Personaggio::Jump_personaggio()
{
	if (Is_on_tile()) {
		vel_v -= MAX_VEL_V;
		last_mov_pers_v += vel_v;
	}
	
}

bool Personaggio::Is_on_tile(){
	int current_pos_col_right = 21 - (int)((last_mov_pers_h) / .05);
	int current_pos_col_middle = current_pos_col_right - 1;
	int current_pos_col_left = current_pos_col_middle;
	int temp_last_pers_h = (int)(last_mov_pers_h * 100000);
	if (temp_last_pers_h % 5000 != 0) {
		current_pos_col_left -= 1;
	}

	int next_pos_row_bottom = (int)(last_mov_pers_v / .05);

	if (last_mov_pers_v > 0)
		next_pos_row_bottom += 13;
	else
		next_pos_row_bottom += 12;
	int temp_last_pers_v = (int)(last_mov_pers_v * 100000);
	if (temp_last_pers_v % 5000 == 0 && (Data.Get_tile(current_pos_col_right, next_pos_row_bottom) == '#'
		|| Data.Get_tile(current_pos_col_middle, next_pos_row_bottom) == '#' || Data.Get_tile(current_pos_col_left, next_pos_row_bottom) == '#')) {
		return true;
	}
	return false;
}


void MyModel::Set_level()
{
	std::ifstream file("../Data/level_input.txt");
	std::string str;
	while (std::getline(file, str)) {
		level.append(str);
	}

}

int MyModel::Get_level_width()
{
	return screen_width * (num_of_screens - 1);
}

int MyModel::Get_level_height()
{
	return level_height;
}

char MyModel::Get_tile(int x, int y) {
	if (x >= 0 && x < screen_width*num_of_screens && y >= 0 && y < level_height)
		return level[y*screen_width*num_of_screens + x];

};

void MyModel::Set_tile(int x, int y, char c) {
	if (x >= 0 && x < screen_width*num_of_screens && y >= 0 && y < level_height)
		level[y*screen_width*num_of_screens + x] = c;
};