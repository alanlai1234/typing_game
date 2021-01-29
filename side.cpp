#include <utility>
#include <panel.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <ncurses.h>
#include <sstream>
#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>

#define DELETE 127
#define red 1
#define green 2
#define bg_red 3
#define yellow 4
#define ctrl(x)   ((x) & 0x1f)

using namespace std;
using namespace std::chrono;

void article();	
void countdown();
void show_graph();
condition_variable cv;
mutex m; //just for condition variable
int time_count;

int Max(vector <int> in){
	int M=in.at(0);
	for (auto i : in){
		if(i>M){
			M=i;
		}
	}

	return M;
}

void curse_init(){
	initscr();
	use_default_colors();
	start_color();
	refresh();
	keypad(stdscr, TRUE);
	curs_set(0);
	noecho();
	cbreak();
	nonl();
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
		else if(s[spot]=='\n'){
			spc.first=spot;
			spc.second=col-(spot%col);
			s[spot]=' ';
			s.insert(spot, string(spc.second-1, ' '));
			spot+=spc.second+1;
			pos.push_back(spc);
			continue;
		}
		else if(s[spot]!=' ' && spc.first>-1){
			++spc.second;
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
	wattron(c_win, COLOR_PAIR(yellow));
	box(c_win, 0, 0);
	mvwprintw(c_win,0, 2, text.c_str());
	wrefresh(c_win);
	for (int i=count;i>=0;--i){
		mvwprintw(c_win, 1,1, "%d   ", i);
		this_thread::sleep_for(seconds(1));
		wrefresh(c_win);
	}
	wattroff(c_win, COLOR_PAIR(yellow));
	hide_panel(c_pan);
	del_panel(c_pan);
	delwin(c_win);
	refresh();
}

// set up text for typing
void text_init(WINDOW *win, string &text, int &size_save, vector< pair<int, int> >&pos){

	//random_device rd;
	//int rrand = rd()%;
	ifstream fp;
	fp.open("articles/1.txt");
	
	// read from file
	stringstream ss;
	ss << fp.rdbuf();
	text=ss.str();
	text[text.size()-1]=' ';
	size_save = text.size()-1;
	pos = alignment(text, COLS); // access it when started typing

	wprintw(win, text.c_str());
	fp.close();

}

void input_stats(int point){
	ofstream fp;
	fp.open("stats.txt", ios_base::app);
	fp << point << endl;
	fp.close();
}

class t_clock{
	private:
		string text;
		int count, x, y, work;
		mutex mu;
		void win();
		void except();
		void sleep();

	public:
		t_clock(string t, int c, int a, int b);
		
		void start();
};

t_clock::t_clock(string t, int c, int a, int b){
	this->text=t;
	this->count=c;
	this->x=a;
	this->y=b;
}

void t_clock::win(){
	refresh();
	WINDOW *c_win=newwin(3,30, y, x);
	PANEL *c_pan=new_panel(c_win);
	update_panels();
	doupdate();
	wattron(c_win, COLOR_PAIR(yellow));
	box(c_win, 0, 0);
	mvwprintw(c_win,0, 2, text.c_str());
	wrefresh(c_win);
	int i=count;
	while(1){
		if( work == 1 ){
			mvwprintw(c_win, 1,1, "%d   ", i--);
			wrefresh(c_win);
			mu.lock();
			work=0;
			mu.unlock();
		}
		else if( work == 2 ){
			break;
		}
		else{
			continue;
		}
	}
	wattroff(c_win, COLOR_PAIR(yellow));
	hide_panel(c_pan);
	del_panel(c_pan);
	delwin(c_win);
	refresh();
}

void t_clock::sleep(){
	for ( int a=0;a<count;++a){
		this_thread::sleep_for(seconds(1));
		mu.lock();
		if(work==2){
			break;
		}
		else{
			work=1;
		}
		mu.unlock();
	}
	mu.lock();
	work=2;
	mu.unlock();
	cv.notify_all();
}

void t_clock::except(){
	//use conditional variable
	unique_lock<mutex> lock(m);
	cv.wait(lock);

	mu.lock();
	work=2;
	mu.unlock();
}

void t_clock::start(){
	thread w(&t_clock::win, this);
	thread e(&t_clock::except, this);
	//thread s(&t_clock::sleep, this);

	w.join();
	e.join();
	//s.join();

	work=0;
}

