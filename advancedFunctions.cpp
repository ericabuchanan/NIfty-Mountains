#include "functions.h"

/* This function uses a ttf file and writes a legend to the screen*/
bool map_legend(ALLEGRO_DISPLAY *display){
    // Define colours
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    ALLEGRO_COLOR purple = al_map_rgb(200,0,255);
    ALLEGRO_COLOR red = al_map_rgb(255,0,0);
    ALLEGRO_COLOR green = al_map_rgb(0,255,0);

    // Initialize allegro fonts
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font = al_load_ttf_font("OpenSans-Regular.ttf", TEXT_SIZE, 0);

    // Check if it was initialized correctly
    if (!font){
        al_show_native_message_box(display, "Error", "Error", "Could not load OpenSans-Regular.ttf", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // Print legend to screen
    al_draw_text(font, white, 0, NUM_ROWS, ALLEGRO_ALIGN_LEFT, "Legend");
    al_draw_text(font, red, 0, NUM_ROWS+TEXT_SIZE, ALLEGRO_ALIGN_LEFT, "All possible paths following the route of lowest elevation change");
    al_draw_text(font, green, 0, NUM_ROWS+(TEXT_SIZE*2), ALLEGRO_ALIGN_LEFT, "The path of overall lowest elevation change");
    al_draw_text(font, purple, 0, NUM_ROWS+(TEXT_SIZE*3), ALLEGRO_ALIGN_LEFT, "The path of lowest overall altitude");

    // Update display
    al_flip_display();
    return true;
}

/* Use array of map altitudes to find path of lowest altitude starting in each row and colour the path
either blue if the boolean draw is true and it is the lowest path */
int drawLowestAltPath(const apmatrix<int> &map, const int startRow, bool draw, ALLEGRO_COLOR colour, int &elevChange){
    // Declare and initialize variables
    int altitudeChange = 0;
    int fwd_up = 0;
    int fwd_down = 0;
    int fwd = 0;
    int row = startRow;

    // Start path at the beginning of the row
    al_draw_filled_rectangle(0, row, 1, row+1, colour);
    // Iterate through all columns for a given row
    for (int i = 0; i < map.numcols()-1; i++){
        // Find the elevation changes going forward, forward and up, and forward and down
        fwd = map[row][i]-map[row][i+1];
        // If statements to avoid going outside of the matrix index
        if (row != 0){
            fwd_up = map[row][i]-map[row-1][i+1];
        }else {
            fwd_up = fwd;
        }
        if (row != NUM_ROWS-1){
           fwd_down = map[row][i]-map[row+1][i+1];
        }else{
            fwd_down = fwd;
        }
        // Assign value that corresponds with lowest altitude and draw path if draw is true
        if (fwd <= fwd_up && fwd <= fwd_down){
            if (draw){
                al_draw_filled_rectangle(i+1,row,i+2,row+1,colour);
                elevChange += abs(fwd);
            }
            altitudeChange += fwd;
        }else if (fwd_up < fwd && fwd_up < fwd_down){
            if (draw){
                al_draw_filled_rectangle(i+1,row-1,i+2,row,colour);
                elevChange += abs(fwd_up);
            }
            altitudeChange += fwd_up;
            row--;
        }else if (fwd_down < fwd_up && fwd_down < fwd){
            if (draw){
                al_draw_filled_rectangle(i+1,row+1,i+2,row+2,colour);
                elevChange += abs(fwd_down);
            }
            altitudeChange += fwd_down;
            row++;
        }else if (fwd_down == fwd_up){
            srand(time(NULL));
            int number = rand()%2;
            if (number == FWD_UP){
                if (draw){
                    al_draw_filled_rectangle(i+1,row-1,i+2,row,colour);
                    elevChange += abs(fwd_up);
                }
                altitudeChange += fwd_up;
                row--;
            }else if (number == FWD_DOWN){
                if (draw){
                    al_draw_filled_rectangle(i+1,row+1,i+2,row+2,colour);
                    elevChange += abs(fwd_down);
                }
                altitudeChange += fwd_down;
                row++;
            }
        }
    }
    // Update display
    al_flip_display();
    // Return lowest elevation
    return altitudeChange;
}

/* Iterate through all of the rows to find the path with the lowest
altitude change and return the index of that row */
int indexOfLowestAltPath(const apmatrix<int> &map, int &lowestAltChange, int &elevChange){
    // Declare and initialize variables
    int index = -1;
    int altChange = -1;
    bool draw = false;

    // Assign allegro colours
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    ALLEGRO_COLOR purple = al_map_rgb(200,0,255);

    // Iterate through rows to find the path with lowest overall altitude
    for (int i = 0; i < map.numrows(); i++){
        altChange = drawLowestAltPath(map, i, draw, black, elevChange);
        // Initialize value so it can be compared to others
        if (i == 0){
            lowestAltChange = altChange;
            index = i;
        // Compare the values
        }else if (altChange < lowestAltChange){
            lowestAltChange = altChange;
            index = i;
        }
    }
    // Iterate through rows and draw the paths
    draw = true;
    for (int i = 0; i < map.numrows(); i++){
        // If lowest altitude change draw path in blue, don't draw otherwise
        if (i == index){
            altChange = drawLowestAltPath(map, i, draw, purple, elevChange);
        }
    }
    // Return index of lowest altitude change path
    return index;
}
