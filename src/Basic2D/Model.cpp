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
#include <list>
#include <stdlib.h>


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
	for (int i = 0; i < 4; i++) {
		if (i==0)
			sprintf(ll, "../Data/fabrizio_00.png", i);
		else if (i==1)
			sprintf(ll, "../Data/blank_tile.png", i);
		else if (i == 2)
			sprintf(ll, "../Data/enemy.png", i);
		else if (i == 3)
			sprintf(ll, "../Data/died.png", i);

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

	if (Player.alive) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View

		// ------------le 3 righe qui sotto servono per spostare il background in orizz di px
		// vedo se gestire o no il salto

		// gravità mi sembra funzionante
		Player.Setup_position();
		glTranslatef(Player.player_horizontal_transl, Player.player_vertical_transl, 0);
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

		// Tiles + nemici
		int enemy_ids = 0;
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
						glVertex3f(-.975 + (.05*col) + tile[i].x + Player.player_horizontal_transl, .55 - (.05*row) + tile[i].y + Player.player_vertical_transl, tile[i].z);
					}
					glEnd();

					break;
				case '*':
					enemy_list.push_back(Enemy(col, row, enemy_ids));
					Set_tile(col, row, '.');
					enemy_ids += 1;
					break;
				default:
					break;
				}
			}
		}

		// Nemici
		std::list<Enemy>::iterator it;

		// definisco lista temp
		std::list<Enemy> temp_list(enemy_list);
		// cancello la lista dei nemici
		enemy_list.clear();

		int my_level_width = Get_level_width();
		// printo tutta la coda ad ogni iterazione
		for (it = temp_list.begin(); it != temp_list.end(); ++it) {

			if (it->alive) {
				glBindTexture(GL_TEXTURE_2D, texture[3]);
				glBegin(GL_QUADS);
				// ad ogni tic mi muovo o verso il personaggio, o dalla parte opposta oppure salto
				//it->random_move(Player.player_x, my_level_width);
				it->Setup_position();
				for (int i = 0; i < 4; i++) {
					glTexCoord2f(it->personaggio[i].u, it->personaggio[i].v);
					glVertex3f(it->personaggio[i].x - it->player_horizontal_transl + Player.player_horizontal_transl,
						it->personaggio[i].y - it->player_vertical_transl + Player.player_vertical_transl, it->personaggio[i].z);
				}
				glEnd();

				// se il nemico è ancora vivo lo appendo alla lista dei nemici, altrimenti muore
				enemy_list.push_back(*it);
			}


		}
		/*
		// bullets
		std::list<Bullet>::iterator it = bullet_list.begin();

		// printo tutta la coda ad ogni iterazione
		for (it = bullet_list.begin(); it != bullet_list.end(); ++it) {
			// it->update_position();
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(it->bullet[i].u, it->bullet[i].v);
				glVertex3f(it->bullet[i].x + Player.player_x + it->pos_x, it->bullet[i].y - Player.player_y - it->pos_y, it->bullet[i].z);
			}
			glEnd();

		}
		*/

		// check delle collisioni
		Check_collisions();

		//  Some text
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();						// Reset The Current Modelview Matrix
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

		//////// QUI ///////////////
		if (true) {
			glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(21),
				-4);
			this->glPrint("bt: %d, mbt: %d, tt: %d, lt: %d, mft: %d, rt: %d plx: %f, ply: %f, trx: %f try: %f",
				Player.bottom_tile, Player.middle_body_tile, Player.top_tile, Player.left_tile, Player.middle_feet_tile, Player.right_tile, Player.player_x, Player.player_y, Player.player_horizontal_transl, Player.player_vertical_transl);
		}

		glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	}

	// quando muoio printo il relativo messaggio
	else 
	{
		std::vector<Vertex> Background_dead;
		Background_dead.push_back(Vertex(-1, -0.6, -5, 0, 0));
		Background_dead.push_back(Vertex(1, -0.6, -5, 1, 0));
		Background_dead.push_back(Vertex(1, 0.6, -5, 1, 1));
		Background_dead.push_back(Vertex(-1, 0.6, -5, 0, 1));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View


		glBindTexture(GL_TEXTURE_2D, texture[4]);

		//  Background
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glTexCoord2f(Background_dead[i].u, Background_dead[i].v);
			glVertex3f(Background_dead[i].x, Background_dead[i].y, Background_dead[i].z);
		}
		glEnd();

	}
	
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
void Character::MoveOrCollide(double nvel_h)
{
	nvel_h = (abs(nvel_h) > MAX_VEL_H) ? (MAX_VEL_H * (nvel_h / abs(nvel_h))) : nvel_h;

	double npx = player_x + nvel_h;

	int next_pos_col = round((npx / .05)) / 100;

	if (nvel_h > 0)
		next_pos_col += 1;
	else
		next_pos_col -= 1;

	/*if (nvel_h < 0)
		next_pos_col = 21 - (int)((npx) / .05);
	else if (nvel_h > 0)
		next_pos_col = 19 - (int)((npx) / .05);*/

	/*if (player_y > 0)
		bottom_tile += 12;
	else
		bottom_tile += 11;*/

	if (Data.Get_tile(next_pos_col, bottom_tile) != '#' && Data.Get_tile(next_pos_col, middle_body_tile) != '#' && Data.Get_tile(next_pos_col, top_tile) != '#') {
		player_x = npx;
		vel_h = nvel_h;
	}
	else {
		vel_h = 0;
	}
}

void Character::Move_up_down_personaggio(int dir)
{
	player_y += .00025 * dir;
}

void Character::Setup_position()
{
	// setto posizione orizzontale
	middle_feet_tile = round(((player_x) / .05)) / 100;
	left_tile = middle_feet_tile - 1;
	right_tile = middle_feet_tile;
	int temp_last_pers_h = (int)(player_x * 100);
	if (temp_last_pers_h % 5 != 0) {
		right_tile += 1;
	}

	// setto posizione verticale
	middle_body_tile = round(((player_y) / .05)) / 100;

	top_tile = middle_body_tile - 1;
	bottom_tile = middle_body_tile;
	// erano rispettivamente: 100000 ; % 5000
	int temp_last_pers_v = (int)(player_y * 100);
	if (temp_last_pers_v % 5 != 0)
		bottom_tile += 1;
		
		

	// forza di gravità
	Gravity();

	// setto i valori di traslazione
	Convert_coordinate_for_translation();
}

void Character::Gravity()
{
	vel_v += .00025;

	if (Is_on_tile()) {
		vel_v = 0;
	}
	else {
		vel_v = (vel_v > MAX_VEL_V) ? MAX_VEL_V : vel_v;
		// posizione a metà corpo di mario + metà altezza di mario + spostamento
		double npy = player_y + (p_height / 2) + vel_h;

		int next_pos_row_bottom = round((npy / .05)) / 100;

		/*
		if (npy > 0)
			next_pos_row_bottom += 12;
		else
			next_pos_row_bottom += 11;
        */
		/*
		middle_feet_tile = (int)((player_x) / .05) - 1;
		right_tile = middle_feet_tile + 1;
		left_tile = middle_feet_tile;
		int temp_last_pers_h = (int)(player_x * 100000);
		if (temp_last_pers_h % 5000 != 0) {
			left_tile -= 1;
		}*/

		if (next_pos_row_bottom == 23) {
			vel_v = 0;
			Die();
		}

		if (Data.Get_tile(right_tile, next_pos_row_bottom) == '#'
			|| Data.Get_tile(middle_feet_tile, next_pos_row_bottom) == '#'
			|| Data.Get_tile(left_tile, next_pos_row_bottom) == '#') {
			player_y = (next_pos_row_bottom - 1) * .05;
			vel_v = 0;
		}

		int next_pos_row_top = next_pos_row_bottom - 2;
		if (Data.Get_tile(right_tile, next_pos_row_top) == '#'
			|| Data.Get_tile(middle_feet_tile, next_pos_row_top) == '#'
			|| Data.Get_tile(left_tile, next_pos_row_top) == '#') {
			player_y = ((next_pos_row_bottom + 0.01) * .05);
			vel_v = 0;
		}

	}
	 		

	player_y += vel_v;
}

void Character::Jump_personaggio()
{
	if (Is_on_tile()) {
		vel_v -= MAX_VEL_V;
		player_y += vel_v;
	}
	
}

bool Character::Is_on_tile(){
	
	int next_pos_row_bottom = round(((player_y+(p_height/2)) / .05)) / 100;
	/*
	if (player_y > 0)
		next_pos_row_bottom += 13;
	else
		next_pos_row_bottom += 12;
	*/
	int temp_last_pers_v = (int)(player_y * 100000);
	if (temp_last_pers_v % 5000 == 0 && (Data.Get_tile(right_tile, next_pos_row_bottom) == '#'
		|| Data.Get_tile(middle_feet_tile, next_pos_row_bottom) == '#' || Data.Get_tile(left_tile, next_pos_row_bottom) == '#')) {
		return true;
	}
	return false;
}

void Character::Convert_coordinate_for_translation() {


	player_horizontal_transl = ((p_width / 2 )* 20) - player_x;
	player_vertical_transl = player_y - ((p_height / 2) * 11);

}

void Character::Die()
{
	alive = false;
}


Bullet Character::shoot() {
	float start_x = player_x + 0.05;
	float start_y = player_y - 0.05;

	Bullet bullet = Bullet(start_x, start_y, player_x, player_y);

	return bullet;

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

}

void MyModel::Set_tile(int x, int y, char c) {
	if (x >= 0 && x < screen_width*num_of_screens && y >= 0 && y < level_height)
		level[y*screen_width*num_of_screens + x] = c;
}

int Character::round(double value) {

	value *= 100;
	value += .5;
	int int_value = (int)value;

	return int_value;
}

// verifico se il personaggio tocca un nemico, oppure se il nemico è toccato da un bullet, oppure se un bullet scontra una tile
void MyModel::Check_collisions()
{
	// Nemici
	std::list<Enemy>::iterator enemy_it;
	float player_right = Player.player_x + (Player.p_width / 2);
	float player_left = Player.player_x - (Player.p_width / 2);
	float player_top = Player.player_y - (Player.p_height / 2);
	float player_bottom = Player.player_y + (Player.p_height / 2);

	// printo tutta la coda ad ogni iterazione
	for (enemy_it = enemy_list.begin(); enemy_it != enemy_list.end(); ++enemy_it) {

		float enemy_right = enemy_it->player_x + (enemy_it->p_width / 2);
		float enemy_left = enemy_it->player_x - (enemy_it->p_width / 2);
		float enemy_top = enemy_it->player_y - (enemy_it->p_height / 2);
		float enemy_bottom = enemy_it->player_y + (enemy_it->p_height / 2);

		// QUI CONTROLLARE CHE FUNZIONI METTENDO BREAKPOINT A RIGA 623 + FARE APPROSSIMAZIONI COME QUELLA FATTA A RIGA 430 PER CONSIDERARE SOLO I PRIMI 2-3 DECIMALI (DA VEDERE)
		if (enemy_left > player_left && enemy_left <= player_right && ((enemy_bottom >= player_top && enemy_bottom < player_bottom) || (enemy_top <= player_bottom && enemy_top > player_top)))
			Player.Die();
	}
}

void Enemy::random_move(float hero_player_x, int level_width)
{
	int rand_var = (rand() % 10) + 1;

	// si ferma
	if (rand_var > 0 && rand_var < 4)
	{
		if (vel_h < 0)
			vel_h += .00025;
		else if (vel_h > 0)
			vel_h -= .00025;

		if (vel_h  < .00025 && vel_h > -.00025)
			vel_h = 0;
	}
	// va verso il personaggio
	if (rand_var > 3 && rand_var < 9)
	{
		double vel = .001;
		// vai verso destra se il personaggio è a destra
		if (hero_player_x >= player_x && player_x < (level_width * .05)) {
			double nvel_h = vel_h + vel;

			MoveOrCollide(nvel_h);
		}
		// vai verso sinistra se il personaggio è a sinistra
		else if (hero_player_x < player_x && player_x > 1) {
			double nvel_h = vel_h - vel;

			MoveOrCollide(nvel_h);
		}
	}
	// salta
	else
	{
		Jump_personaggio();
	}
}