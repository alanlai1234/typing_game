#include <utility>
#include <chrono>
#include <vector>
#include <fstream>
#include <ncurses.h>
#include <sstream>
#include <thread>
#include <iostream>
#include <string>
#include <random>

#define DELETE 127

using namespace std;
using namespace std::chrono;

void article();	

void curse_init(){
	initscr();
	use_default_colors();
	start_color();

	refresh();
	keypad(stdscr, TRUE);
	curs_set(0);
	noecho();
}

vector < pair<int, int> > alignment (string &s, int col){
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
vector < pair<int, int> > alignment (string &s, int col){
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

void choose(){
	string ls;
	ls << system("ls");
}
