#include "side.cpp"
// TODO : able perform multi-paragraph article typing

void main_scr(){
	mvprintw(1, 0, "test your typing limit!\n");
}

void clock_win(){
	refresh();
	WINDOW *c_win=newwin(3,30, 0, 30);
	box(c_win, 0, 0);
	mvwprintw(c_win,0, 2, "count down");
	int count=10;
	mvwprintw(c_win, 1,1, "0");
	wrefresh(c_win);
	time_t tt=time(NULL), origin=time(NULL);
	while (count>tt-origin){
		tt=time(NULL);
		mvwprintw(c_win, 1,1, "%ld", tt-origin);
		wrefresh(c_win);
	}
}

int main(){
	
	curse_init();
	// TODO menu
	printw("typing test---------------------------------------------");
	article();

	//getch();
	endwin();
	return 0;

}

void article(){
	WINDOW *win=newwin(LINES, COLS, 2, 0);
	refresh();
	touchwin(win);
	wrefresh(win);

	


	//random_device rd;
	//int rrand = rd()%;
	ifstream fp;
	fp.open("articles/1.txt");
	
	// read from file
	stringstream ss;
	ss << fp.rdbuf();
	string text = ss.str();
	int ch, spot=0;
	int size_save=text.size()-1;
	vector< pair<int, int> > pos = alignment(text, COLS); // access it when started typing
	
	// print it out
	wprintw(win, text.c_str());
	fp.close();

	// setting for ready
	wmove(win, 0, 0);
	waddch(win, text[0] | A_UNDERLINE);
	wrefresh(win);
	init_pair(1, COLOR_RED, -1);
	init_pair(2, COLOR_GREEN, -1);
	init_pair(3, -1, COLOR_RED);
	vector< pair<int, int> >::iterator it = pos.begin();
	int errors=0;
	spot=0;

	//// starting timing
	auto START = steady_clock::now();

	//// start typing
	while (spot < text.size()-1){
		ch=wgetch(win);
		if(ch=='1'){
			return;
		}
		if(ch==DELETE){
			if(spot!=0){
				mvwaddch(win, spot/COLS, spot%COLS, text[spot]);
				--spot;
				if(spot == (it-1)->first+(it-1)->second-1){
					--it;
					spot-=it->second-1;
				}
			}
			else{
				continue;
			}
		}
		else{
			if(ch==text[spot]){
				mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(2));
			}
			else{
				++errors;
				if(text[spot] == ' '){
					mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(3));
				}
				else{
					mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(1));
				}
			}
			// check if the there is extra space for alignment
			if(spot == it->first){
				spot+=it->second;
				++it;
			}
			else {
				++spot;
			}
		}
		mvwaddch(win, spot/COLS, spot%COLS, text[spot] | A_UNDERLINE);
	}
	auto END = steady_clock::now();
	auto dur = duration_cast<seconds>(END-START);
	mvwprintw(win, (text.size()/COLS)+1, 0, "wpm : %.*f", 0, ((size_save/5)-errors)/(dur.count()/60.000));
	wgetch(win);
}

