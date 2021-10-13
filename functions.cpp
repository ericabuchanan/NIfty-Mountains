#include "functions.h"

/* This function uses the file name to open the file and reads the
altitude data into an array if it is opened correctly */
bool MapDataDrawer(const char *fname, apmatrix<int> &map){
    // Open file
    ifstream inFile(fname);
    // Check if it is open
    if (!inFile.is_open()){
        return false;
    // If it is open, read the data into map array
    }else{
        for (int i = 0; i < map.numrows(); i++){
            for (int j = 0; j < map.numcols(); j++){
                inFile >> map[i][j];
            }
        }
        // Close file
        inFile.close();
        return true;
    }
}

/* This function takes the map array and find the minimum
value in it */
int findMin(const apmatrix<int> &map){
    int minimum = map[0][0];
    // Iterate through all values in the array to find smallest value
    for (int i = 0; i < map.numrows(); i++){
        for (int j = 0; j < map.numcols(); j++){
            if (map[i][j] < minimum){
                minimum = map[i][j];
            }
        }
    }
    // Return smallest value
    return minimum;
}

/* This function takes the map array and find the maximum
value in it */
int findMax(const apmatrix<int> &map){
    int maximum = map[0][0];
    // Iterate through all values in the array to find largest value
    for (int i = 0; i < map.numrows(); i++){
        for (int j = 0; j < map.numcols(); j++){
            if (map[i][j] > maximum){
                maximum = map[i][j];
            }
        }
    }
    // Return largest value
    return maximum;
}

// Draw graphical representation of map altitudes with dark green for low and bright blue for high
void drawMap(const apmatrix<int> &map, const int minimum, const int maximum){
    // Set colours for low and high altitudes
    int highAltColour[3] = {0,230,255};
    int lowAltColour[3] = {0,120,50};
    int finalColour[3];
    float decimalAltitude = 0.0;
    // Iterate through all altitudes and find corresponding grayscale value
    for (int i = 0; i < map.numrows(); i++){
        for (int j = 0; j < map.numcols(); j++){
            // This repesented the altitude by a number from 0 (low) to 1 (high)
            decimalAltitude = (map[i][j] - minimum) / float(maximum-minimum);
            // Find final rgb colour using interpolation
            for (int k = 0; k < 3; k++){
                finalColour[k] = lowAltColour[k] + (highAltColour[k] - lowAltColour[k])*decimalAltitude;
            }
            ALLEGRO_COLOR colour = al_map_rgb(finalColour[0], finalColour[1], finalColour[2]);
            // Draw rectangle of that colour
            al_draw_filled_rectangle(j, i, j+1, i+1, colour);
        }
    }
    // Update display
    al_flip_display();
}

/* Use array of map altitudes to find lowest elevation path starting in each row and colour the path
either red or green if the boolean draw is true */
int drawLowestElevPath(const apmatrix<int> &map, const int startRow, bool draw, ALLEGRO_COLOR colour){
    // Declare and initialize variables
    int elevation = 0;
    int fwd_up = 0;
    int fwd_down = 0;
    int fwd = 0;
    int row = startRow;

    // Start path at the beginning of the row
    al_draw_filled_rectangle(0, row, 1, row+1, colour);
    // Iterate through all columns for a given row
    for (int i = 0; i < map.numcols()-1; i++){
        // Find the elevation changes going forward, forward and up, and forward and down
        fwd = abs(map[row][i]-map[row][i+1]);
        // If statements to avoid going outside of the matrix index
        if (row != 0){
            fwd_up = abs(map[row][i]-map[row-1][i+1]);
        }else {
            fwd_up = fwd;
        }
        if (row != map.numrows()-1){
           fwd_down = abs(map[row][i]-map[row+1][i+1]);
        }else{
            fwd_down = fwd;
        }
        // Assign value that corresponds with lowest elevation change and draw path if draw is true
        if (fwd <= fwd_up && fwd <= fwd_down){
            if (draw){
                al_draw_filled_rectangle(i+1,row,i+2,row+1,colour);
            }
            elevation += fwd;
        }else if (fwd_up < fwd && fwd_up < fwd_down){
            if (draw){
                al_draw_filled_rectangle(i+1,row-1,i+2,row,colour);
            }
            elevation += fwd_up;
            row--;
        }else if (fwd_down < fwd_up && fwd_down < fwd){
            if (draw){
                al_draw_filled_rectangle(i+1,row+1,i+2,row+2,colour);
            }
            elevation += fwd_down;
            row++;
        }else if (fwd_down == fwd_up){
            srand(time(NULL));
            int number = rand()%2;
            if (number == FWD_UP){
                if (draw){
                    al_draw_filled_rectangle(i+1,row-1,i+2,row,colour);
                }
                elevation += fwd_up;
                row--;
            }else if (number == FWD_DOWN){
                if (draw){
                    al_draw_filled_rectangle(i+1,row+1,i+2,row+2,colour);
                }
                elevation += fwd_down;
                row++;
            }else {
                cout << "error in random" << endl;//can probably take these out?
            }
        }else {
            cout << "ERROR - CHECK MATH" << endl;
        }
    }
    // Update display
    al_flip_display();
    // Return lowest elevation
    return elevation;
}

/* Iterate through all of the rows to find the path with the lowest
elevation change and return the index of that row */
int indexOfLowestElevPath(const apmatrix<int> &map, int &lowestElevChange){
    // Declare and initialize variables
    int index = -1;
    int elevChange = -1;
    bool draw = false;

    // Assign allegro colours
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    ALLEGRO_COLOR red = al_map_rgb(255,0,0);
    ALLEGRO_COLOR green = al_map_rgb(0,255,0);

    // Iterate through rows to find the path with lowest elevation change
    for (int i = 0; i < map.numrows(); i++){
        elevChange = drawLowestElevPath(map, i, draw, black);
        // Initialize value so it can be compared to others
        if (i == 0){
            lowestElevChange = elevChange;
            index = i;
        // Compare the values
        }else if (elevChange < lowestElevChange){
            lowestElevChange = elevChange;
            index = i;
        }
    }
    // Iterate through rows and draw the paths
    draw = true;
    for (int i = 0; i < map.numrows(); i++){
        // If lowest elevation change draw path in green
        if (i == index){
            elevChange = drawLowestElevPath(map, i, draw, green);
        // If not lowest elevation change draw path in red
        }else {
            elevChange = drawLowestElevPath(map, i, draw, red);
        }

    }
    // Return index of lowest elevation path
    return index;
}
