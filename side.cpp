#include <utility>
#include <panel.h>
#include <chrono>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <ncurses.h>
#include <sstream>
#include <thread>
#include <iostream>
#include <string>
#include <random>

#define DELETE 127
#define red 1
#define green 2
#define bg_red 3
#define yellow 4

using namespace std;
using namespace std::chrono;

void article();	
void getready();

void curse_init(){
	initscr();
	use_default_colors();
	start_color();
	refresh();
	keypad(stdscr, TRUE);
	curs_set(0);
	noecho();
	init_pair(1, COLOR_RED, -1);
	init_pair(4, COLOR_YELLOW, -1);
	init_pair(2, COLOR_GREEN, -1);
	init_pair(3, -1, COLOR_RED);
}

vector <pair<int, int> > alignment (string &s, int col){
	int spot=0;
	pair <int, int> spc; // first is the position second is the count after the space
	vector< pair<int, int> > pos; // access it when started typing

	while(spot < s.size()){
		if(spot%col==1){
			spc.first=-1;
			spc.second=0;
		}
		else if(s[spot]!=' ' && spc.first>-1){
			spc.second+=1;
			if(spot%col==0 ){
				s.insert(spc.first, string(spc.second-1, ' '));
				pos.push_back(spc);
				spot+=spc.second;
				continue;
			}
			
		}
		else{
			spc.first=spot;
			spc.second=0;
		}
		++spot;
	}
	return pos;
}

void clock_win(string text, int count, int x, int y){
	refresh();
	WINDOW *c_win=newwin(3,30, y, x);
	PANEL *c_pan=new_panel(c_win);
	update_panels();
	doupdate();
	wattron(c_win, COLOR_PAIR(4));
	box(c_win, 0, 0);
	mvwprintw(c_win,0, 2, text.c_str());
	wrefresh(c_win);
	for (int i=count;i>=0;--i){
		mvwprintw(c_win, 1,1, "%d", i);
		this_thread::sleep_for(seconds(1));
		wrefresh(c_win);
	}
	wattroff(c_win, COLOR_PAIR(yellow));
	hide_panel(c_pan);
	del_panel(c_pan);
	delwin(c_win);
	refresh();
}

