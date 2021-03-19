#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200201L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

void disclaimer();
void gameplay_text();
void fullscreen_text();
void art();
void draw_logo();
void controls();
void sniper();
int game();

int main(){
    srand(time(NULL));

    // initialize the library
    initscr();

    // set implicit modes
    cbreak();
    noecho();
    start_color();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    
    //get screen size
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);

    //colors
    if (!has_colors()) {
        endwin();
        printw("Error — no color support on this terminal\n");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    //color for main screen
    wattron(stdscr, COLOR_PAIR(1));
    attrset(COLOR_PAIR(1));
    wbkgd(stdscr, COLOR_PAIR(1));

    //fullscreen
    fullscreen_text();

    //disclaimer
    disclaimer();

    //text about game
    gameplay_text();

    //window for menu
    WINDOW *menuwin = newwin(LINES, xMax-xMax, yMax-yMax, 0);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);

    //keyboard for menu
    keypad(menuwin, TRUE);

    //color for menu
    wattron(menuwin, COLOR_PAIR(1));
    attrset(COLOR_PAIR(1));
    wbkgd(menuwin, COLOR_PAIR(1));

    //buffer text
    char buffer[] = "2020 Aleksey Kobel";
    mvwprintw(menuwin, LINES - 1, COLS/2 - (strlen(buffer)/2), "%s", buffer);
    refresh();
    wrefresh(menuwin);

    //drawing logo
    draw_logo();

    //drawing rifle
    art();

    //main cycle
    while(1){
        //making menu
        char alts[][100] = {{"Start Game"},     
                            {"Controls"},     
                            {"Quit Game"},};
        int choice;
        int highlight = 0;
        
        while(1){
            for (int i = 0; i < 3; i++){
                if(i == highlight)
                    wattron(menuwin, A_REVERSE);
                mvwprintw(menuwin, i + LINES/2, COLS/2 - 30, alts[i]);
                wattroff(menuwin, A_REVERSE);
            }
            choice = wgetch(menuwin);
            switch(choice){
                case KEY_UP:
                    highlight--;
                    if(highlight == -1) highlight = 2;
                    break;
                case KEY_DOWN:
                    highlight++;
                    if(highlight == 3) highlight = 0;
                    break;
                default:
                    break;
            }
            if(choice == 10){
                break;             //if "enter" break
            }     
        }

        //check for choice
        if(highlight == 0){
            while(1){
                sleep(1);
                game();
                //if(game() == 1) break;
            }
        }
        if(highlight == 1){
            controls();
            refresh();
            wrefresh(menuwin);
        }
        if(highlight == 2){
            clear();
            mvprintw(LINES/2, COLS/2 - 33,"Thank you for playing, goodbye. Follow my instagram @aleksey.021:)");
            refresh();
            sleep(2);
            endwin();
            break;
        }
    }
    
    // end curses
    sleep(5);
    endwin();

return 0;
}

void art(){
    char rifle[][100] = {
                            {"                                     ____    _     __     _    ____                          "},
                            {"                                    |####`--|#|---|##|---|#|--'##|#|                         "},
                            {"     _                              |____,--|#|---|##|---|#|--.__|_|                         "},
                            {"   _|#)__________________________________,--'EEEEEEEEEEEEEE'_=-.                             "},
                            {"  ((_____((______________________,--------[JW](___(____(____(_==)        _________           "},
                            {"                              .--|##,----o  o  o  o  o  o  o__|/`---,-,-'===========`=+==.   "},
                            {"                              |##|_Y__,__.-._,__,  __,-.___/ J \\ .----.###############|##|   "},
                            {"                              |##|              `-.|#|##|#|`===l##\\   _\\##############|##|   "},
                            {"                              =======-===l         |_|__|_|     \\##`-___,====+===.####|##|   "},
                            {"                                                                 \\__,'           '======='   "},
                        };

    mvprintw(LINES/2 - 3, COLS - 94, rifle[0]);
    mvprintw(LINES/2 - 3 + 1, COLS - 94, rifle[1]);
    mvprintw(LINES/2 - 3 + 2, COLS - 94, rifle[2]);
    mvprintw(LINES/2 - 3 + 3, COLS - 94, rifle[3]);
    mvprintw(LINES/2 - 3 + 4, COLS - 94, rifle[4]);
    mvprintw(LINES/2 - 3 + 5, COLS - 94, rifle[5]);
    mvprintw(LINES/2 - 3 + 6, COLS - 94, rifle[6]);
    mvprintw(LINES/2 - 3 + 7, COLS - 94, rifle[7]);
    mvprintw(LINES/2 - 3 + 8, COLS - 94, rifle[8]);
    mvprintw(LINES/2 - 3 + 9, COLS - 94, rifle[9]);
    refresh();
}

void sniper(){

    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    char rifle[][100] = {   {"   ____                                           "},
                            {"--`####|                                          "},
                            {"--,____|                                      _   "},   
                            {"--,__________________________________________(#|_ "},
                            {"]]-------,_____________________________))_______))"},
                            {"  o----,##|--.                                    "},
                            {"___,__Y___|##|                                    "},
                            {"          |##|                                    "},
                            {"   l======-===                                    "},


    };

    char fire[][10] = {   
                        {" \\ | /"},
                        {"-- o --"},
                        {" / | \\"},       
    };

    attron(COLOR_PAIR(3));
    mvprintw(LINES/2 - 3, 1, rifle[0]);
    mvprintw(LINES/2 - 3 + 1, 1, rifle[1]);
    mvprintw(LINES/2 - 3 + 2, 1, rifle[2]);
    mvprintw(LINES/2 - 3 + 3, 1, rifle[3]);
    mvprintw(LINES/2 - 3 + 4, 1, rifle[4]);
    mvprintw(LINES/2 - 3 + 5, 1, rifle[5]);
    mvprintw(LINES/2 - 3 + 6, 1, rifle[6]);
    mvprintw(LINES/2 - 3 + 7, 1, rifle[7]);
    mvprintw(LINES/2 - 3 + 8, 1, rifle[8]);
    refresh();
    attroff(COLOR_PAIR(3));
    sleep(1);

    attron(COLOR_PAIR(4));
    mvprintw(LINES/2 - 3 + 3, 48, fire[0]);
    mvprintw(LINES/2 - 3 + 4, 48, fire[1]);
    mvprintw(LINES/2 - 3 + 5, 48, fire[2]);
    refresh();
    attroff(COLOR_PAIR(4));

    char bullet[] = "  ==>\0";
    for(int i = 55; i < COLS - 2; i++){
        attron(COLOR_PAIR(3));
        mvprintw(LINES/2 - 3 + 4, i, bullet);
        refresh();
        usleep(4000);
    }
    attroff(COLOR_PAIR(3));

    clear();

}
void disclaimer(){

    //color initialization
    init_pair(2, COLOR_RED, COLOR_BLACK);

    //window for disclaimer
    WINDOW *dis = newwin(LINES, COLS, 0, 0);
    box(dis, 0, 0);
    refresh();
    wrefresh(dis);

    //color for dis
    wattron(dis, COLOR_PAIR(2));
    wbkgd(dis, COLOR_PAIR(2));

    //main text
    mvwprintw(dis, LINES/2.5, COLS/2 - 5, "DISCLAIMER");
    char text[] = "All coincidences with the real world and people are random.";
    char text2[] = "The game has no message."; 
    char text3[] = "The game does not call for murders.";
    char text4[] = "This game is made for fun.";

    mvwprintw(dis, LINES/2, COLS/2 - (strlen(text)/2), "%s", text);
    mvwprintw(dis, LINES/2+2, COLS/2 - (strlen(text)/2), "%s", text2);
    mvwprintw(dis, LINES/2+4, COLS/2 - (strlen(text)/2), "%s", text3);
    mvwprintw(dis, LINES/2+6, COLS/2 - (strlen(text)/2), "%s", text4);
    mvwprintw(dis, LINES - LINES/4, COLS/2 - 14, "Press any key to continue");

    refresh();
    wrefresh(dis);

    int skip;
    skip = getch();
    switch(skip){
        case KEY_ENTER:
        wclear(dis);
        refresh();
        wrefresh(dis);
        break;
    }

}

void gameplay_text(){

    //color initialization
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    //window for text
    WINDOW *txt = newwin(LINES, COLS, 0, 0);
    box(txt, 0, 0);

    //color for window
    wattron(txt, COLOR_PAIR(1));
    wbkgd(txt, COLOR_PAIR(1));

    //main
    refresh();
    wrefresh(txt);
    char text[][100] = {{"In this game, you need to prevent the assassination of the president.             "},
                        {"You can navigate the map using the arrows on your keyboard.                       "},
                        {"To prevent murder, you must sacrifice yourself and stop the bullet with your body."}, 
                        {"If you move, the bullet is accelerating.                                          "},
                        {"If the bullet hits its target (the president), you lose.                          "},
                        {"Good luck, don't let him die.                                                     "}
                       };

    for(int i = 0; i < 6; i++){
        mvwprintw(txt, i + LINES/2.5, COLS/2 - (strlen(text[i])/2), text[i]);
        refresh();
        wrefresh(txt);
        sleep(1);
    }

    mvwprintw(txt, LINES-LINES/4, COLS/2-14, "Press any key to continue");
    wrefresh(txt);
    int skip;
    skip = getch();
    switch(skip){
        case KEY_ENTER:
        wclear(txt);
        refresh();
        wrefresh(txt);
        break;
    }

}
void draw_logo(){

    //was taken from file 'courses-example' at bomber tutorial
    char logo[7][100] = {{"                                                                        "},
                         {" 888888ba   888888ba   88888888b dP     dP  88888888b 888888ba  d888888P"}, 
                         {" 88    `8b  88    `8b  88        88     88  88        88    `8b    88   "},    
                         {"a88aaaa8P' a88aaaa8P' a88aaaa    88    .8P a88aaaa    88     88    88   "},    
                         {" 88         88   `8b.  88        88    d8'  88        88     88    88   "},    
                         {" 88         88     88  88        88  .d8P   88        88     88    88   "},    
                         {" dP         dP     dP  88888888P 888888'    88888888P dP     dP    dP   "}    
    };

    /*char logo[12][100] = { {"                                                                             "}, 
                           {"   ========================================================================= "},
                           {"   =       ===       ===        ==  ====  ==        ==  =======  ==        = "},
                           {"   =  ====  ==  ====  ==  ========  ====  ==  ========   ======  =====  ==== "},
                           {"   =  ====  ==  ====  ==  ========  ====  ==  ========    =====  =====  ==== "},
                           {"   =  ====  ==  ===   ==  ========  ====  ==  ========  ==  ===  =====  ==== "},
                           {"   =       ===      ====      ====   ==   ==      ====  ===  ==  =====  ==== "},
                           {"   =  ========  ====  ==  =========  ==  ===  ========  ====  =  =====  ==== "},
                           {"   =  ========  ====  ==  =========  ==  ===  ========  =====    =====  ==== "},
                           {"   =  ========  ====  ==  ==========    ====  ========  ======   =====  ==== "},
                           {"   =  ========  ====  ==        =====  =====        ==  =======  =====  ==== "},
                           {"   ========================================================================= "}  

    };*/

                                                                              
    int center = COLS/2 - strlen(logo[0])/2 - 10;

    int target = LINES/2 - 8;
    for( int row_count = 6; row_count >= 1; row_count-- ){

        for(int y = 1; y <= target; y++ ){
            move(y, center);
            printw("%s", logo[row_count]);
            refresh();
            usleep( 1 * 60000 );

            // clear
            move(y,center);
            printw(logo[0]);
        }

        move(target,center);
        printw(logo[row_count]);

        target--;
    }
    usleep( 2 * 900000 );
    
}

void fullscreen_text(){

    //color initialization
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    //window for text
    WINDOW *txt = newwin(LINES, COLS, 0, 0);
    box(txt, 0, 0);

    //color for window
    wattron(txt, COLOR_PAIR(1));
    wbkgd(txt, COLOR_PAIR(1));

    //main
    refresh();
    wrefresh(txt);
    char text[150] = "Please, if your terminal is not running in full screen mode, turn it on and restart the game for better display!";

    mvwprintw(txt, LINES/2, COLS/2 - (strlen(text)/2), text);
    mvwprintw(txt, LINES - LINES/4, COLS/2 - 14, "Press any key to continue");

    refresh();
    wrefresh(txt);

    int skip;
    skip = getch();
    switch(skip){
        case KEY_ENTER:
        wclear(txt);
        refresh();
        wrefresh(txt);
        break;
        
    }
}

void controls(){

    //initializong varibles
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    char control[][50] = {{"Go up - up arrow"},     
                          {"Go down - down arrow"},     
                          {"Go right - right arrow"},
                          {"Go left - left arrow"},
                          {"P - pause the game"}
                         };

    //window for controls
    WINDOW *controls = newwin(LINES/4, COLS-2, LINES/1.5, 1);
    box(controls, 0, 0);
    refresh();
    wrefresh(controls);

    //adding color
    wattron(controls, COLOR_PAIR(1));
    attrset(COLOR_PAIR(1));
    wbkgd(controls, COLOR_PAIR(1));

    //getting size of window
    int x, y;
    getmaxyx(controls, y, x);

    //printing text
    for (int i = 0; i < 5; i++){
        mvwprintw(controls, i + y/3, x/2 - 10, control[i]);
        wrefresh(controls);
    }
    char text[] = "Press 'C' to close";
    mvwprintw(controls, y - 1, x/2 - strlen(text)/2, text);
    wrefresh(controls);

    //close controls
    int c = getch();
    switch(c){
        case 'c': {
            wclear(controls);
            refresh();
            wrefresh(controls);
            break;
        }
    }
}

int game(){

    //initialize colors
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    //window for level menu
    WINDOW *levels = newwin(LINES, COLS, 0, 0);
    box(levels, 0, 0);
    keypad(levels, TRUE);
    char buffer_levels[] = "Press ESC for return to main menu";
    mvwprintw(levels, LINES-1, COLS/2 - strlen(buffer_levels)/2, buffer_levels);
    refresh();
    wrefresh(levels);

    //color for levels
    wattron(levels, COLOR_PAIR(1));
    attrset(COLOR_PAIR(1));
    wbkgd(levels, COLOR_PAIR(1));

    //levels array
    char levels_array[9][10] = {{"1 level"},
                                {"2 level"},
                                {"3 level"},
                                {"4 level"},
                                {"5 level"},
                                {"6 level"},
                                {"7 level"},
                                {"8 level"},
                                {"9 level"}
                               };                

    //making level menu
    int choice;
    int highlight = 0;
    while(1){
        for (int i = 0; i < 9; i++){
            if(i == highlight)
                wattron(levels, A_REVERSE);
            if(i < 3)    
                mvwprintw(levels, ((i*4) + 0.5) * LINES/9, ((i*4) + 1) * 2, levels_array[i]);
            else if(i >= 3 && i < 6) 
                mvwprintw(levels, ((i-3)*4 + 0.5) * LINES/9, ((i*4) + (COLS/8)) * 2, levels_array[i]);  
            else if(i >= 6 && i < 9) 
                mvwprintw(levels, ((i-6)*4 + 0.5) * LINES/9, ((i*4) + (COLS/4)) * 2, levels_array[i]);
            wattroff(levels, A_REVERSE);
        }
        choice = wgetch(levels);
        switch(choice){
            case KEY_UP:
                highlight--;
                if(highlight == -1) highlight = 8;
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == 9) highlight = 0;
                break;
            case KEY_RIGHT:
                highlight += 3;
                if(highlight == 11 ) highlight = 2;
                if(highlight == 10 ) highlight = 1;
                if(highlight == 9 ) highlight = 0;
                break;
            case KEY_LEFT:
                highlight -= 3;
                if(highlight == -3 ) highlight = 6;
                if(highlight == -2 ) highlight = 7;
                if(highlight == -1 ) highlight = 8;
                break;     
            default:
                break;
        }
        if(choice == 10){
            break;             //if "enter" break
        }     
        if(choice == 27){
            main();
        }
    }    

    //choosing level
    int selected_level;
    if(highlight == 0){
        selected_level = 1;
    }
    if(highlight == 1){
        selected_level = 2;
    }
    if(highlight == 2){
        selected_level = 3;
    }
    if(highlight == 3){
        selected_level = 4;
    }
    if(highlight == 4){
        selected_level = 5;
    }
    if(highlight == 5){
        selected_level = 6;
    }
    if(highlight == 6){
        selected_level = 7;
    }
    if(highlight == 7){
        selected_level = 8;
    }
    if(highlight == 8){
        selected_level = 9;
    }

    //making window for game
    WINDOW *game = newwin(LINES, COLS, 0, 0);
    box(game, 0, 0);
    refresh();
    wrefresh(game);

    //color for game window
    wattron(game, COLOR_PAIR(1));
    attrset(COLOR_PAIR(1));
    wbkgd(game, COLOR_PAIR(1));

    //window for mr.president
    WINDOW *mr = newwin(LINES/3, COLS/6, LINES/2 - ((LINES/3)/2), COLS - COLS/4.5);
    box(mr, 0, 0);
    int xmr;
    xmr = COLS - COLS/4;
    int xMr, yMr;
    getmaxyx(mr, xMr, yMr);
    int wallmr = COLS - COLS/4.5;
    int wallymr = LINES/2 - ((LINES/3)/2);

    //color for mr
    wattron(mr, COLOR_PAIR(1));
    attrset(COLOR_PAIR(1));
    wbkgd(mr, COLOR_PAIR(1));

    //initializing all variables for main cycle
    char arrow[20] = "     ==> \0";
    int xAr, yAr;
    xAr = 0;
    yAr = LINES/2;

    //making time delay
    struct timespec ts = {
        .tv_sec = 0,                    // nr of secs
        .tv_nsec = 0.001 * 1000000000L  // nr of nanosecs
    };

    //making buffer text
    char buffer[] = "To start game press any arrow button. To make pause press 'p'.";
    mvwprintw(game, LINES-1, 1, buffer);

    ts.tv_nsec = 0.1 * 100000000L;

    bool gameover = false;
    bool gamewin = false;

    //main cycle

    //LEVEL 1          LEVEL 1          LEVEL 1
    if(selected_level == 1){

        sniper();

        //player start coords
        int y = LINES / 2;     
        int x = COLS / 2;

        //initializing player
        char player = '>', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 3; xAr <= xmr; xAr++){
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));
                attron(COLOR_PAIR(3));
                mvwprintw(game, yAr, xAr, "%s", arrow);
                attroff(COLOR_PAIR(3));

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(100);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(x > 1) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                    if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);
    }

    // LEVEL 2            LEVEL 2                 LEVEL 2
    else if(selected_level == 2){

        sniper();

        //drawing obstacles
        for(int i = 1; i <= COLS/3; i++)
            mvwprintw(game, LINES - LINES/3, i + 0, "-");
        refresh();
        wrefresh(game);

        //player start coords
        int x = COLS/6;
        int y = LINES - LINES/5;

        //initializing player
        char player = '>', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 3; xAr <= xmr; xAr++){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);
                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(100);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(x == (COLS/3 + 1) && y == (LINES - LINES/3)) break;
                                else if(x > 1) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == ((LINES - LINES/3) + 1) && x < COLS/3 + 1) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == ((LINES - LINES/3) - 1) && x < COLS/3 + 1) break;
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);
                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);
    }

    //LEVEL 3            LEVEL 3             LEVEL 3
    else if(selected_level == 3){

        sniper();

        //drawing obstacles
        for(int i = 1; i <= COLS - COLS/3; i++){
            mvwprintw(game, LINES/2.5, i + 0, "-");
            mvwprintw(game, LINES - LINES/2.5, i + 0, "-");
        }    
        for(int i = 1; i <= LINES/2.5 - 1; i++){
            mvwprintw(game, i + 0, COLS - COLS/3, "|");
        }
        for(int i = (LINES - LINES/2.5) + 1; i < LINES - 1; i++){
            mvwprintw(game, i + 0, COLS - COLS/3, "|");
        }
        refresh();
        wrefresh(game);

        //player start coords
        int x = (COLS - COLS/3.5);
        int y = 3;

        //initializing player
        char player = '>', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 3; xAr <= xmr+5; xAr+=5){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);
                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((xAr >= x) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(100);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(x == ((COLS - COLS/3) + 1) && y < (LINES/2.5)) break;
                                else if(x == ((COLS - COLS/3) + 1) && y > (LINES - LINES/2.5) - 1) break;
                                else if(x > 1 ) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y < (LINES/2.5 + 1) && x < ((COLS - COLS/3) + 1)) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y > ((LINES - LINES/2.5) - 2) && x < ((COLS - COLS/3) + 1)) break;
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((xAr >= x) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }

                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);
    }
    //LEVEL 4                   LEVEL 4                 LEVEL 4
    else if(selected_level == 4){

        sniper();

        int x_mr, ymr;
        getmaxyx(mr, ymr, x_mr);

        int x_wall;

        //drawing obstacles
        for(int i = 0; i <= COLS/5-2; i++){
            mvwprintw(game,  LINES/2 - ((LINES/3)/2), COLS - COLS/4.5 - i, "-");
            x_wall = i;
        }
        for(int i = 0; i <= COLS - ((COLS - COLS/4) + xMr); i++){
            mvwprintw(game, (LINES/2 - ((LINES/3)/2)) + ymr/2, (COLS - 2) - i, "-");
        }
        refresh();
        wrefresh(game);

        //player start coords
        int x = ((COLS - COLS/4) + x_mr) + 10;
        int y = (LINES/2 - ((LINES/3)/2));

        //initializing player
        char player = '>', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 1; xAr <= xmr+5; xAr++){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }

                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(100);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(x > 1 ) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(y == ((LINES/2 - ((LINES/3)/2))) && x > ((COLS - COLS/4.5) - x_wall) - 2) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == ((LINES/2 - ((LINES/3)/2)) + 1) && ((x >= ((COLS - COLS/4.5) - x_wall) - 1) && x <= (COLS - COLS/5))) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == (((LINES/2 - ((LINES/3)/2)) + ymr/2) - 1) && (x >= (((COLS - COLS/4) + xMr)) && x <= (COLS-1))) break;
                                else if(y == ((LINES/2 - ((LINES/3)/2)) - 1) && ((x >= ((COLS - COLS/4.5) - x_wall) - 1) && x <= (COLS - COLS/5))) break;
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                } 

                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);

    }
    //LEVEL 5                  LEVEL 5                 LEVEL 5
    if(selected_level == 5){

        sniper();

        int first_wall, second_wall;
        //drawing obstacles
        for(int i = 0; i <= COLS/2 + 10; i++){
            mvwprintw(game, LINES - LINES/3, 1 + i, "-");
            first_wall = i;
        }
        for(int i = 0; i <= COLS/2 + 10; i++){
            mvwprintw(game, LINES - (LINES/3)/2, (COLS-2) - i, "-");
            second_wall = i;
        }
        refresh();
        wrefresh(game);

        //player start coords
        int x = (COLS - COLS/3) + 10;
        int y = LINES - LINES/8;

        //initializing player
        char player = '>', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 1; xAr <= xmr+5; xAr++){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }

                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(400);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(y == (LINES - LINES/3) && x < (1 + first_wall) + 2) break;
                                else if(x > 1 ) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(y == (LINES - (LINES/3)/2) && x > ((COLS-2) - second_wall) - 2) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == (LINES - (LINES/3)/2 + 1) && x > ((COLS-2) - second_wall) - 1) break;
                                else if(y == ((LINES - LINES/3) + 1) && x < (1 + first_wall) + 1) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == (LINES - (LINES/3)/2 - 1) && x > ((COLS-2) - second_wall) - 1) break;
                                else if(y == ((LINES - LINES/3) - 1) && x < (1 + first_wall) + 1) break;
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                } 

                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);
    }
    //LEVEL 6                   LEVEL 6                 LEVEL 6
    if(selected_level == 6){

        sniper();   

        int first, second, third, fifth, ninth;
        //drawing obstacles
        for(int i = 1; i <= COLS/5; i++){
            mvwprintw(game, LINES/4+1, i, "-");
            first = i;
        }
        for(int i = 0; i <= LINES/10; i++){
            mvwprintw(game, LINES/4 - i, first, "|");
            second = i;
        }
        for(int i = 0; i <= LINES*2; i++){
            mvwprintw(game, LINES/4 - second, first + i, "-");
            third = i;
        }
        for(int i = 0; i <= LINES/10-1; i++){
            mvwprintw(game, LINES/4 - i, first + third, "|");
        }
        for(int i = first + third; i <= COLS-2; i++){
            mvwprintw(game, LINES/4+1, i, "-");
            first = i;
        }
        for(int i = 0; i <= LINES*1.5 - 5; i++){
            mvwprintw(game, LINES/4+1, COLS/4 + i, "-");
            fifth = i;
        }
        for(int i = 0; i < 1; i++){
            mvwprintw(game, LINES/4 - i, COLS/4 + fifth, "|");
        }
        for(int i = 0; i < 1; i++){
            mvwprintw(game, LINES/4 - i, COLS/4, "|");
        }
        for(int i = 0; i <= LINES*1.5 - 5; i++){
            mvwprintw(game, LINES/4-1, COLS/4 + i, "-");
            ninth = i;
        }
        refresh();
        wrefresh(game);

        //player start coords
        int x = COLS/2.5;
        int y = LINES/4 - 2;

        //initializing player
        char player = '<', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 5; xAr <= xmr+5; xAr++){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }

                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(100);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                   if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break; 
                                   else if(y < LINES/4 + 2 && y > LINES/4 - 2 && x == COLS/4 + fifth + 1);
                                   else if(x == COLS/5 + 1 && y > LINES/4 - second - 1 && y < LINES/4 + 2) break;
                                   else if(x > 1 ) x--;
                                   break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(y < LINES/4 + 2 && y > LINES/4 - 2 && x == COLS/4 - 1) break;
                                    else if(y > LINES/4 - second - 1 && y < LINES/4 + 2 && x == COLS/5 + third - 1) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == LINES/4 + 2 && x > COLS/4 - 1 && x < COLS/4 + ninth + 1) break;
                                else if(y == LINES/4 + 2 && x < COLS/5+1) break;
                                else if(y == LINES/4 - second + 1 && x > COLS/5 && x < COLS/5 + third) break;
                                else if(y == LINES/4 + 2 && x < COLS-1 && x > COLS/5 + third - 1) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                   if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                   else if(y == LINES/4 - 2 && x > COLS/4 - 1 && x < COLS/4 + ninth + 1) break;
                                   else if(y < LINES - 2) y++;
                                   break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wclear(game);
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                } 

                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);        

    }
    //LEVEL 7               LEVEL 7                 LEVEL 7
    if(selected_level == 7){

        sniper();

        int first, second;
        //drawing obstacles
        for(int i = 1; i <= COLS/3.5; i++){
            mvwprintw(game, LINES - LINES/3, i, "-");
            first = i;
        }
        for(int i = COLS-2; i >= COLS - COLS/3.5; i--){
            mvwprintw(game, LINES - LINES/3, i, "-");
            second = i;
        }
        for(int i = first + 10; i <= second - 10; i++){
            mvwprintw(game, LINES - LINES/3, i, "-");
        }
        refresh();
        wrefresh(game);

        //player start coords
        int x = COLS/2;
        int y = LINES - LINES/8;

        //initializing player
        char player = '<', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 14; xAr <= xmr+5; xAr++){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wclear(game);
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }

                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(100);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(y == LINES - LINES/3 && x == first + 1) break;
                                else if(y == LINES - LINES/3 && x == second - 10 + 1) break;
                                else if(x > 1 ) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(y == LINES - LINES/3 && x == first + 10 - 1) break;
                                    else if(y == LINES - LINES/3 && x == second - 1) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == LINES - LINES/3 + 1 && x < COLS/3.5) break;
                                else if(y == LINES - LINES/3 + 1 && x > COLS - COLS/3.5 - 1) break;
                                else if(y == LINES - LINES/3 + 1 && x > COLS/3.5 + 10 - 1 && x < COLS - COLS/3.5 - 10 + 1) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == LINES - LINES/3 - 1 && x < COLS/3.5) break;
                                else if(y == LINES - LINES/3 - 1 && x > COLS - COLS/3.5 - 1) break;
                                else if(y == LINES - LINES/3 - 1 && x > COLS/3.5 + 10 - 1 && x < COLS - COLS/3.5 - 10 + 1) break;
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                } 

                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);        

    }
    //LEVEL 8                   LEVEL 8                    LEVEL 8
    if(selected_level == 8){

        sniper();

        //int first, second, third;
        //drawing obstacles
        for(int i = 0; i <= COLS/6; i++){
            mvwprintw(game, LINES/4, 1 + i, "-");
        }
        for(int i = COLS/6 + 10; i <= COLS/2 - 12; i++){
            mvwprintw(game, LINES/4, i, "-");
        }
        for(int i = 0; i <= LINES/11; i++){
            mvwprintw(game, LINES/4 - 1 - i, COLS/2 - 12, "|");
        }
        mvwprintw(game, LINES/4 - LINES/11 - 2, COLS/2 - 12, "/");
        for(int i = LINES/4 - LINES/11 - 3; i > 0; i--){
            mvwprintw(game, i, COLS/2 - 12, "|");
        }
        refresh();
        wrefresh(game);

        //player start coords
        int x = COLS/4 + 1;
        int y = LINES/8;

        //initializing player
        char player = 'v', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 15; xAr <= xmr+5; xAr++){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }

                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(100);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(x == COLS/2 + 1 - 12 && y > 0 && y < LINES/4 - LINES/11 - 2) break;
                                else if(x == COLS/2 + 1 - 12 && y > LINES/4 - LINES/11 - 2 && y < LINES/4 + 1) break;
                                else if(y == LINES/4 && x == COLS/6+2) break;
                                else if(x > 1 ) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                    else if(x == COLS/2 - 1 - 12 && y > 0 && y < LINES/4 - LINES/11 - 2) break;
                                    else if(x == COLS/2 - 1 - 12 && y > LINES/4 - LINES/11 - 2 && y < LINES/4) break;
                                    else if(y == LINES/4 && x == COLS/6 + 9) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == LINES/4 + 1 && x < COLS/6+2) break; 
                                else if(y == LINES/4 + 1 && x > COLS/6+9 && x < COLS/2 - 12) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == LINES/4 - 1 && x < COLS/6+2) break; 
                                else if(y == LINES/4 - 1 && x > COLS/6+9 && x < COLS/2 - 12) break; 
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                } 

                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);        

    }
    //LEVEL 9                   LEVEL 9                     LEVEL 9
        if(selected_level == 9){

        sniper();

        //drawing obstacles
        for(int i = 1; i <= COLS/2.5; i++){
            mvwprintw(game, LINES - LINES/3, i, "-");
        }
        mvwprintw(game, LINES - LINES/3, COLS/2.5 + 1, ">");
        mvwprintw(game, LINES - LINES/3, COLS/2.5 + 2, ">");
        for(int i = COLS/2.5 + 3; i < COLS - 1; i++){
            mvwprintw(game, LINES - LINES/3, i, "-");
        }
        refresh();
        wrefresh(game);

        //player start coords
        int x = (COLS - COLS/3) + 10;
        int y = LINES - LINES/8;

        //initializing player
        char player = '>', previous = ' ';
        mvwprintw(game, y, x, "%c", player);
        refresh();
        wrefresh(game);

        //initializing window mr
        refresh();
        wrefresh(mr);
        mvwprintw(mr, 1, xMr/2+2, "Mr. President");
        mvwprintw(mr, yMr/8, xMr/2+2, "  _________  ");
        mvwprintw(mr, yMr/8+1, xMr/2+3, "| __   __ | ");
        mvwprintw(mr, yMr/8+2, xMr/2+3, "|  -   -  | ");
        mvwprintw(mr, yMr/8+3, xMr/2+3, "|    |    | ");
        mvwprintw(mr, yMr/8+4, xMr/2+3, "|   ---   | ");
        mvwprintw(mr, yMr/8+5, xMr/2+3, " \\___^___/ ");
        wrefresh(mr);

        do{
            
            if(gamewin == true) break;
            if(gameover == true) break;

            for(xAr = 1; xAr <= xmr+5; xAr++){
                
                //making window for pause
                WINDOW *pause = newwin(LINES/8, COLS/8, 1, COLS/2 - ((COLS/8)/2));
                box(pause, 0, 0);
                mvwprintw(pause, 1, (getmaxx(pause)/2) - 3, "PAUSED");
                mvwprintw(pause, 2, (getmaxx(pause)/2) - 8, "'P' for continue");
                mvwprintw(pause, 3, (getmaxx(pause)/2) - 7, "Enter for exit");

                //color for pause window
                wattron(pause, COLOR_PAIR(1));
                attrset(COLOR_PAIR(1));
                wbkgd(pause, COLOR_PAIR(1));

                mvwprintw(game, yAr, xAr, "%s", arrow);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }

                int input = getch();

                //previous = (y == LINES - 1 && x < 34) ? buffer[x] : ' ';
                if(input == KEY_LEFT || input == KEY_RIGHT || input == KEY_UP || input == KEY_DOWN){
                    mvwprintw(game, y, x, "%c", previous);
                }
                timeout(600);
                //KEYBOARD
                switch(input){
                    case 'p':
                        refresh();
                        wrefresh(pause);
                        while(1){
                            input = getch();
                            if(input == 'p'){
                                wclear(pause);
                                refresh();
                                wrefresh(pause);
                                wrefresh(game);
                                break;
                            }
                            else if(input == 10){
                                main();
                            }
                        }
                        break;
                    case KEY_LEFT: player = '<';
                                if(x == wallmr + COLS/6 && y > wallymr - 1 && y < wallymr + LINES/3) break;
                                else if(y == LINES - LINES/3 && x == COLS/2.5 + 5) break;
                                else if(x > 1 ) x--;
                                break;
                    case KEY_RIGHT: player = '>';
                                    if(x == wallmr - 1 && y > wallymr - 1 && y < wallymr + LINES/3) break; 
                                    else if(y == LINES - LINES/3 && x == COLS/2.5 + 5) break;
                                    else if(x < COLS - 2) x++; 
                                    break;
                    case KEY_UP: player = '^';
                                if(y == wallymr + LINES/3 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == LINES - LINES/3 + 1 && x < COLS/2.5 + 5) break;
                                else if(y == LINES - LINES/3 + 1 && x > COLS/2.5 + 5) break;
                                else if(y > 1) y--;
                                break;
                    case KEY_DOWN: player = 'v';
                                if(y == wallymr - 1 && x > wallmr - 1 && x < wallmr + COLS/6) break;
                                else if(y == LINES - LINES/3 - 1 && x < COLS/2.5 + 5) break;
                                else if(y == LINES - LINES/3 - 1 && x > COLS/2.5 + 5) break;
                                else if(y < LINES - 2) y++;
                                break;
                    default: input = 0; break;
                }

                mvwprintw(game, y, x, "%c", player);

                if(xAr >= xmr) {
                    gameover = true;
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2-20, "Oh no, Mr. President is dead, try again!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                }
                else if((x == xAr + 6) && (y == yAr)){
                    gamewin = true; 
                    clear();
                    wclear(game);
                    mvwprintw(game, LINES/2, COLS/2 - 15, "Congratulations, you save him!");
                    refresh();
                    wrefresh(game);
                    sleep(5);
                    break;
                } 

                refresh();
                wrefresh(game);
                nanosleep(&ts, NULL); 
            }

        } while(1);        

    }
    //if(gameover == true) return gameover;
    //else if (gamewin == true) return gamewin;
    
return 0;    
}
