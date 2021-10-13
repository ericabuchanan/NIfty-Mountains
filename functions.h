// Include all necessary allegro and c++ libraries
#include <fstream>
#include <iostream>
#include <string.h>
#include <apmatrix.h>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <time.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

// Define constants
#define FWD_UP 0
#define FWD_DOWN 1
#define NUM_COLS 844
#define NUM_ROWS 480
#define TEXT_SIZE 18
#define LEGEND 80

using namespace std;

// Prototypes
bool MapDataDrawer(const char *fname, apmatrix<int> &map);
int findMin(const apmatrix<int> &map);
int findMax(const apmatrix<int> &map);
void drawMap(const apmatrix<int> &map, const int minimum, const int maximum);
int drawLowestElevPath(const apmatrix<int> &map, const int startRow, bool draw, ALLEGRO_COLOR colour);
int indexOfLowestElevPath(const apmatrix<int> &map, int &lowestElevChange);
bool map_legend(ALLEGRO_DISPLAY *display);
int drawLowestAltPath(const apmatrix<int> &map, const int startRow, bool draw, ALLEGRO_COLOR colour, int &elevChange);
int indexOfLowestAltPath(const apmatrix<int> &map, int &lowestAltChange, int &elevChange);
void indexOfMinRow(const apmatrix<int> &map); // what part is this used for?
int indexOfLowestEastWestPath(const apmatrix<int> &map);

