/**********************************************
* Erica Buchanan    Nifty Mountain Assignment *
* 5/05/2021                                   *
*                                             *
* This program prints to the screen a visual  *
* representation of a data file containing    *
* the elevations of a mountain range. It then *
* prints a series of optimal paths through    *
* the mountains based off of elevation change *
* or altitude in general.                     *
**********************************************/

#include "functions.h"

int main(){
    // Declare and initialize variables
    apmatrix<int> mountainMap(NUM_ROWS,NUM_COLS);
    int minimum = -1;
    int maximum = -1;
    int lowestElevChange = -1;
    int lowestElevIndex = -1;
    int lowestAltChange = 0;
    int lowestAltIndex = -1;
    int elevChange = -1;
    char answer = ' ';
    char fileName[13] = "Colorado.dat";

    // Print error message if can't open file
    if (!MapDataDrawer(fileName, mountainMap)){
        cout << "Error opening file." << endl;
        return -1;
    // Read from file and display if open
    }else{
        // Find minimum and maximum values
        minimum = findMin(mountainMap);
        maximum = findMax(mountainMap);

        // Initialize allegro and display
        al_init();
        al_init_primitives_addon();
        ALLEGRO_DISPLAY *display = al_create_display(NUM_COLS,NUM_ROWS+LEGEND);

        // Check if display was created
        if (!display){
            al_show_native_message_box(display, "Error", "Error", "Failed to initialize display.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            return -1;
        }

        // Set title for graphics window
        al_set_window_title(display, "Nifty Mountains");

        // Draw map using altitudes from file
        drawMap(mountainMap, minimum, maximum);

        // Find path with lowest elevation using greedy method
        lowestElevIndex = indexOfLowestElevPath(mountainMap, lowestElevChange);

        // Print lowest elevation change and index its row
        cout << "The path with the lowest overall elevation change starts on line: " << lowestElevIndex << " and has an elevation change of: " << lowestElevChange << endl;

        // Print the legend
        if (!map_legend(display)){
            return -1;
        }

        // Find path with lowest overall altitude using greedy method
        lowestAltIndex = indexOfLowestAltPath(mountainMap, lowestAltChange, elevChange);

        // Print the lowest altitude path and a comparison between the paths
        cout << "The path with the lowest overall altitude starts on line: " << lowestAltIndex << " and the overall elevation change is: " << elevChange << endl;
        cout << "The lowest elevation path results in an elevation that is " << elevChange-lowestElevChange << " units lower than than path for lowest altitude." << endl;

        // Destroy display once user enters character
        cout << "Return any character to exit the display: ";
        cin >> answer;
        al_destroy_display(display);
    }
    return 0;
}
