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
	("../Data/sky_background1.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	if (texture[0] == 0) return false;

	//  Load 27 personaggio textures
	char ll[200];
	for (int i = 0; i < 26; i++) {
		// sfondo 2^ anno
		if (i == 0)
			sprintf(ll, "../Data/sky_background2.png", i);

		// sfondo 3^ anno
		else if (i==1)
			sprintf(ll, "../Data/sky_background3.png", i);

		// tile
		else if (i == 2)
			sprintf(ll, "../Data/tile.png", i);

		// menu iniziale
		else if (i == 3)
			sprintf(ll, "../Data/start_menu.png", i);

		// schermata game over
		else if (i == 4)
			sprintf(ll, "../Data/died.png", i);

		// schermata vittoria
		else if (i == 5)
			sprintf(ll, "../Data/won.png", i);

		// proiettile
		else if (i == 6)
			sprintf(ll, "../Data/bullet.png", i);

		// porta
		else if (i == 7)
			sprintf(ll, "../Data/door.jpeg", i);

		// aula B1
		else if (i == 8)
			sprintf(ll, "../Data/aula.png", i);

		// sprites di fabrizio, i numeri sono 9 + numeri da 1 a 4
		else if (i==9)
			sprintf(ll, "../Data/fabrizio_still.png", i);
		else if (i == 10)
			sprintf(ll, "../Data/fabrizio_move1.png", i);
		else if (i == 11)
			sprintf(ll, "../Data/fabrizio_move2.png", i);
		else if (i == 12)
			sprintf(ll, "../Data/fabrizio_jump.png", i);

		// sprites di jacopo, i numeri sono 13 + numeri da 1 a 4
		else if (i == 13)
			sprintf(ll, "../Data/jacopo_still.png", i);
		else if (i == 14)
			sprintf(ll, "../Data/jacopo_move1.png", i);
		else if (i == 15)
			sprintf(ll, "../Data/jacopo_move2.png", i);
		else if (i == 16)
			sprintf(ll, "../Data/jacopo_jump.png", i);

		//  per i prof i numeri sono 17+id
		else if (i == 17)
			// prof 1 Morro
			sprintf(ll, "../Data/morro.png", i); 
		else if (i == 18)
			// prof 2 Barberis
			sprintf(ll, "../Data/barberis.png", i);
		else if (i == 19)
			// prof 3 Zaccaria
			sprintf(ll, "../Data/zaccaria.png", i);
		else if (i == 20)
			// prof 4 Aicardi
			sprintf(ll, "../Data/aicardi.png", i);
		else if (i == 21)
			// prof 5 Boccalatte
			sprintf(ll, "../Data/boccalatte.png", i);
		else if (i == 22)
			// prof 6 Grattarola
			sprintf(ll, "../Data/grattarola.png", i);
		else if (i == 23)
			// prof 7 Cannata
			sprintf(ll, "../Data/cannata.png", i);
		else if (i == 24)
			// prof 8 Casalino
			sprintf(ll, "../Data/casalino.png", i);
		else if (i == 25)
			// prof 9 Tacchella
			sprintf(ll, "../Data/tacchella.png", i);

		this->texture[i + 1] = SOIL_load_OGL_texture(
			ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture[i + 1] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}



bool MyModel::DrawGLScene(void)
{
	if (game_started)
	{
		//  TIMING - start
		clock_t t = clock();
		// elapsed time in seconds from the last draw
		double elapsed = double(t - Tstamp) / (double)CLOCKS_PER_SEC;
		// elapsed time in milliseconds from the last draw
		int ms_elapsed = (int)(t - Tstamp);

		if (ms_elapsed < 8) return true;
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

			// gestione gravità e traslazione
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

			//  Background2 da qui---------------------
			glBindTexture(GL_TEXTURE_2D, texture[1]);

			
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(Background2[i].u, Background2[i].v);
				glVertex3f(Background2[i].x, Background2[i].y, Background2[i].z);
			}
			glEnd();
			//  Background2 fino a qui---------------------

			
			//  Background3 da qui---------------------
			glBindTexture(GL_TEXTURE_2D, texture[2]);

			
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(Background3[i].u, Background3[i].v);
				glVertex3f(Background3[i].x, Background3[i].y, Background3[i].z);
			}
			glEnd();
			//  Background3 fino a qui---------------------
			

			//  gestione del movimento delle gambe quando si corre in orizzontale
			if (Player.motion_status > 0 && Player.motion_status < 3)
				Player.motion_status = 1 + ((int((Get_last_motion_elapsed() * 10))) % 2);

			// sprite del personaggio sono variabili: 10 è la posizione base (fabrizio still) a cui viene sommato lo status 
			// (fermo, movimento, jump) e un eventuale delay per cambiare personaggio da fabrizio a jacopo
			int pos_texture = 10 + Player.motion_status + texture_delay;
			glBindTexture(GL_TEXTURE_2D, texture[pos_texture]);

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
				glVertex3f(Player.character_direction*Player.personaggio[i].x, Player.personaggio[i].y, Player.personaggio[i].z);
			}
			glEnd();

			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);

			// Tiles + nemici, lettura da file txt
			int enemy_ids = 0;
			int door_ids = 0;
			for (int col = 0; col < Data.screen_width * Data.num_of_screens; col++) {
				for (int row = 0; row < Data.level_height; row++) {
					char id = Data.Get_tile(col, row);
					switch (id)
					{
					case '#':
						glBindTexture(GL_TEXTURE_2D, texture[3]);
						glBegin(GL_QUADS);
						for (int i = 0; i < 4; i++) {
							glTexCoord2f(tile[i].u, tile[i].v);
							glVertex3f(-.975 + (.05*col) + tile[i].x + Player.player_horizontal_transl, .55 - (.05*row) + tile[i].y + Player.player_vertical_transl, tile[i].z);
						}
						glEnd();

						break;
					case '|':
						door_list.push_back(Door(col, row, door_ids));
						Set_tile(col, row, '.');
						door_ids += 1;
						break;
					case '.':
						break;
					default:
						int type = id - '0';
						enemy_list.push_back(Enemy(col, row, enemy_ids, type));
						Set_tile(col, row, '.');
						enemy_ids += 1;
						break;
					}
				}
			}

			// doors
			std::list<Door>::iterator door_it;

			// printo tutta la coda ad ogni iterazione
			for (door_it = door_list.begin(); door_it != door_list.end(); ++door_it) {

				glBindTexture(GL_TEXTURE_2D, texture[8]);
				glBegin(GL_QUADS);
				for (int i = 0; i < 4; i++) {
					glTexCoord2f(door_it->porta[i].u, door_it->porta[i].v);
					glVertex3f(door_it->porta[i].x - door_it->door_horizontal_transl + Player.player_horizontal_transl,
						door_it->porta[i].y - door_it->door_vertical_transl + Player.player_vertical_transl, door_it->porta[i].z);
				}
				glEnd();

				// se non è la prima porta, disegno l'aula intorno alla porta i-esima
				if (door_it != door_list.begin()) {
					// setto la nuova texture
					glBindTexture(GL_TEXTURE_2D, texture[9]);
					glBegin(GL_QUADS);

					// per ogni vertice...
					for (int i = 0; i < 4; i++) {
						// calcolati in tile
						int horizontal_adj;
						int vertical_adj = 0;

						// ...se è quello in basso a sinistra, voglio lo spostamento verticale a zero e quello orizzontale a -5...
						if (i == 0)
							horizontal_adj = -5;

						// ...se è quello in basso a destra, voglio lo spostamento verticale a zero e quello orizzontale a 5...
						else if (i == 1)
							horizontal_adj = 5;

						// ...se è quello in alto a destra, voglio lo spostamento verticale a 3 e quello orizzontale a 5...
						else if (i == 2) {
							horizontal_adj = 5;
							vertical_adj = 3;
						}

						// ...se è quello in alto a sinistra, voglio lo spostamento verticale a 3 e quello orizzontale a -5
						else if (i == 3) {
							horizontal_adj = -5;
							vertical_adj = 3;
						}


						glTexCoord2f(door_it->porta[i].u, door_it->porta[i].v);
						glVertex3f(door_it->porta[i].x + (horizontal_adj * 0.05) - door_it->door_horizontal_transl + Player.player_horizontal_transl,
							door_it->porta[i].y - door_it->door_vertical_transl + Player.player_vertical_transl + (vertical_adj * 0.05), door_it->porta[i].z - 1);
					}
					glEnd();
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
					glBindTexture(GL_TEXTURE_2D, texture[17+it->type]);

					// PER PNG TRASPARENTE DA QUI---------------------------
					glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
					glLoadIdentity();									// Reset The View

					// Personaggio
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0);
					// PER PNG TRASPARENTE FINO A QUI---------------------------
					glBegin(GL_QUADS);
					// ad ogni tic mi muovo o verso il personaggio, o dalla parte opposta oppure salto
					it->random_move(Player.player_x, my_level_width);
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

			// bullets
			std::list<Bullet>::iterator bullet_it;

			// definisco lista temp
			std::list<Bullet> temp_list_b(bullet_list);
			// cancello la lista dei nemici
			bullet_list.clear();
			// printo tutta la coda ad ogni iterazione
			for (bullet_it = temp_list_b.begin(); bullet_it != temp_list_b.end(); ++bullet_it) {

				if (bullet_it->alive) {
					bullet_it->Update_position();
					glBindTexture(GL_TEXTURE_2D, texture[7]);
					glBegin(GL_QUADS);
					for (int i = 0; i < 4; i++) {
						glTexCoord2f(bullet_it->bullet[i].u, bullet_it->bullet[i].v);
						glVertex3f(bullet_it->bullet[i].x - bullet_it->bullet_horizontal_transl + Player.player_horizontal_transl,
							bullet_it->bullet[i].y - bullet_it->bullet_vertical_transl + Player.player_vertical_transl, bullet_it->bullet[i].z);
					}
					glEnd();

					bullet_list.push_back(*bullet_it);
				}

			}

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
			char on_off_god[20];
			
			if (this->frames > 18) {
				this->fps = frames / frameTime;
				this->frames = 0; this->frameTime = 0;
			}
			if (Player.god_mode)
				strcpy(on_off_god, "ON");
			else
				strcpy(on_off_god, "OFF");
			this->glPrint("Elapsed time: %6.2f sec.  -  Fps %6.2f, God mode is %s",
				Full_elapsed, fps, on_off_god);

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

			std::vector<Vertex> Background_end;
			Background_end.push_back(Vertex(-1, -0.6, -5, 0, 0));
			Background_end.push_back(Vertex(1, -0.6, -5, 1, 0));
			Background_end.push_back(Vertex(1, 0.6, -5, 1, 1));
			Background_end.push_back(Vertex(-1, 0.6, -5, 0, 1));

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
			glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
			glLoadIdentity();									// Reset The View

			if (Player.has_won) {
				// carico texture di won game
				glBindTexture(GL_TEXTURE_2D, texture[6]);
			}
			else {
				// carico texture di lost game
				glBindTexture(GL_TEXTURE_2D, texture[5]);

			}

			//  Background
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(Background_end[i].u, Background_end[i].v);
				glVertex3f(Background_end[i].x, Background_end[i].y, Background_end[i].z);
			}
			glEnd();
			

		}
	}

	// mostro il menu iniziale
	else
	{
		std::vector<Vertex> Background_start_menu;
		Background_start_menu.push_back(Vertex(-1, -0.6, -5, 0, 0));
		Background_start_menu.push_back(Vertex(1, -0.6, -5, 1, 0));
		Background_start_menu.push_back(Vertex(1, 0.6, -5, 1, 1));
		Background_start_menu.push_back(Vertex(-1, 0.6, -5, 0, 1));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View


		glBindTexture(GL_TEXTURE_2D, texture[4]);

		//  Background
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glTexCoord2f(Background_start_menu[i].u, Background_start_menu[i].v);
			glVertex3f(Background_start_menu[i].x, Background_start_menu[i].y, Background_start_menu[i].z);
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

	int next_pos_col = round((npx / .05)) / 100000;

	if (nvel_h > 0)
		next_pos_col += 1;
	else
		next_pos_col -= 1;

	if (Data.Get_tile(next_pos_col, bottom_tile) != '#' && Data.Get_tile(next_pos_col, middle_body_tile) != '#' && Data.Get_tile(next_pos_col, top_tile) != '#') {
		player_x = npx;
		vel_h = nvel_h;
	}
	else {
		vel_h = 0;

		/* Per rumore quando scontri lateralmente
		if (Data.Get_last_bump_elapsed() > .25)
			bump = true;*/
	}

}

void Character::Move_up_down_personaggio(int dir)
{
	player_y += .00025 * dir;
}

void Character::Setup_position()
{
	// setto posizione orizzontale
	middle_feet_tile = round(((player_x) / .05)) / 100000;
	left_tile = middle_feet_tile - 1;
	right_tile = middle_feet_tile;
	int temp_last_pers_h = round(player_x);
	if (temp_last_pers_h % 5000 != 0) {
		right_tile += 1;
	}

	// setto posizione verticale
	middle_body_tile = round(((player_y) / .05)) / 100000;

	top_tile = middle_body_tile - 1;
	bottom_tile = middle_body_tile;
	// erano rispettivamente: 100000 ; % 5000
	int temp_last_pers_v = round(player_y);
	if (temp_last_pers_v % 5000 != 0)
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

		int next_pos_row_bottom = round((npy / .05)) / 100000;

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
			player_y = ((next_pos_row_bottom + 0.05) * .05);
			vel_v = 0;

			bump = true;
		}

	}
	 		

	player_y += vel_v;
}

bool Character::Jump_personaggio()
{
	if (Is_on_tile()) {
		vel_v -= MAX_VEL_V;
		player_y += vel_v;

		return true;
	}
	
	return false;
}

bool Character::Is_on_tile(){
	
	int next_pos_row_bottom = round(((player_y+(p_height/2)) / .05)) / 100000;

	int temp_last_pers_v = round(player_y);
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
	if (!god_mode)
		alive = false;
}


Bullet Character::shoot() {
	float b_x = player_x - (0.05 * character_direction);
	float b_y = player_y;
	float trasl_x = ((p_width / 2) * 20) - b_x;
	float trasl_y = b_y - ((p_height / 2) * 11);

	Bullet bullet = Bullet(b_x, b_y, trasl_x, trasl_y, (0.01)*character_direction);

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

	value *= 100000;
	value += .5;
	int int_value = (int)value;

	return int_value;
}

// verifico se il personaggio tocca un nemico, oppure se il nemico è toccato da un bullet, oppure se un bullet scontra una tile (o esce dallo schermo)
void MyModel::Check_collisions()
{
	bool there_are_enemies = true;

	if (enemy_list.size() == 0)
	{
		enemy_list.push_back(Enemy(0, 0, -1, 0));
		there_are_enemies = false;
	}

	std::list<Enemy>::iterator enemy_it;
	int player_right = Player.round(Player.player_x + (Player.p_width / 2));
	int player_left = Player.round(Player.player_x - (Player.p_width / 2));
	int player_top = Player.round(Player.player_y - (Player.p_height / 2));
	int player_bottom = Player.round(Player.player_y + (Player.p_height / 2));

	// Nemici toccano il personaggio oppure vengono toccati da bullet
	for (enemy_it = enemy_list.begin(); enemy_it != enemy_list.end(); ++enemy_it) {

		int enemy_right = enemy_it->round(enemy_it->player_x + (enemy_it->p_width / 2));
		int enemy_left = enemy_it->round(enemy_it->player_x - (enemy_it->p_width / 2));
		int enemy_top = enemy_it->round(enemy_it->player_y - (enemy_it->p_height / 2));
		int enemy_bottom = enemy_it->round(enemy_it->player_y + (enemy_it->p_height / 2));

		// tocco tra personaggio e nemico
		/*if (((enemy_left >= player_left && enemy_left <= player_right) || (enemy_right > player_left && enemy_right < player_right)) 
			&& ((enemy_bottom > player_top && enemy_bottom < player_bottom) || (enemy_top < player_bottom && enemy_top > player_top)))*/
		if ((((player_bottom > enemy_top && player_bottom < enemy_bottom) || (player_top < enemy_bottom && player_top > enemy_top)) &&
			((player_right < enemy_right && player_right > enemy_left) || (player_left > enemy_left && player_left < enemy_right))) 
			|| (player_bottom == enemy_top && player_left == enemy_left && player_right == enemy_right) 
			|| (player_top == enemy_bottom && player_left == enemy_left && player_right == enemy_right))
			Player.Die();
		else 
		{
			std::list<Bullet>::iterator bullet_it;
			// per ogni bullet controllo che non ci sia collisione con:
			// 1) nemico -> nemico muore, bullet muore
			// 2) tile -> bullet muore
			// 3) bordo schermo -> bullet muore
			for (bullet_it = bullet_list.begin(); bullet_it != bullet_list.end(); ++bullet_it) {
				int bullet_rounded_x = bullet_it->round(bullet_it->bullet_x);
				int bullet_rounded_y = bullet_it->round(bullet_it->bullet_y);
				int row_bullet = bullet_it->round(bullet_it->bullet_y / .05) / 100000;
				int col_bullet = bullet_it->round(bullet_it->bullet_x / .05) / 100000;

				// caso 1)
				if ((bullet_rounded_x < enemy_right && bullet_rounded_x > enemy_left) && (bullet_rounded_y > enemy_top && bullet_rounded_y < enemy_bottom))
				{
					enemy_it->Die();
					enemy_hit_by_bullet = true;
					bullet_it->Die();
				}

				// caso 2)
				else if (Get_tile(col_bullet, row_bullet) == '#')
					bullet_it->Die();

				// caso 3)
				else if (abs(bullet_it->bullet_x - Player.player_x) > ((screen_width / 2) * (Player.p_width / 2)))
					bullet_it->Die();
			}

		}
	}

	if (!there_are_enemies)
		enemy_list.clear();

}

double MyModel::Get_last_shot_elapsed()
{
	return Full_elapsed - Shot_elapsed;
}

void MyModel::Set_shot_elapsed()
{
	Shot_elapsed = Full_elapsed;
}

double MyModel::Get_last_bump_elapsed()
{
	return Full_elapsed - Bump_elapsed;
}

void MyModel::Set_bump_elapsed()
{
	Bump_elapsed = Full_elapsed;
}

double MyModel::Get_last_motion_elapsed()
{
	return Full_elapsed - Motion_elapsed;
}

void MyModel::Set_motion_elapsed()
{
	Motion_elapsed = Full_elapsed;
}

double MyModel::Get_last_jump_elapsed()
{
	return Full_elapsed - Jump_elapsed;
}

void MyModel::Set_jump_elapsed()
{
	Jump_elapsed = Full_elapsed;
}

void Enemy::random_move(float hero_player_x, int level_width)
{
	int rand_var = (rand() % 10) + 1;

	// si ferma
	/*if (rand_var > 0 && rand_var < 4)
	{
		if (vel_h < 0)
			vel_h += .00025;
		else if (vel_h > 0)
			vel_h -= .00025;

		if (vel_h  < .00025 && vel_h > -.00025)
			vel_h = 0;
	}*/
	// va verso il personaggio
	if (rand_var > 0 && rand_var < 9)
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

void Bullet::Update_position() {

	bullet_horizontal_transl += bullet_vel_h;

	bullet_x = (.05 * 20) - bullet_horizontal_transl;
}

void Bullet::Die()
{
	alive = false;
}

int Bullet::round(double value) {

	value *= 100000;
	value += .5;
	int int_value = (int)value;

	return int_value;
}