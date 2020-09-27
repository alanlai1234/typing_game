#include "side.cpp"
// TODO: finish broken countdown mode

int main(){
	
	curse_init();
	printw("typing test---------------------------------------------\n\n");
	// menu
	char key;
	addstr("1) article 2) count down(broken) 3) stats q) exit");
	
	key=getch();
	switch (key){
		case '1':
			article();
			break;

		case '2':
			countdown();
			break;

		case '3':
			show_graph();
			break;

		case 'q':
			break;
	}
	
	endwin();
	return 0;

}

void show_graph(){
	ifstream fp;
	fp.open("stats.txt");
	vector <int> points;
	string res;

	// read from file
	while ( fp >> res ){
		points.push_back(stoi(res));
	}
	
	int unit = (int)((Max(points))/10); //how much point a row represents in the graph

	for ( int i=10;i>=0;--i ){
		printw("\n%2d|", unit*i);
		for ( auto j : points ){
		    if( (int)j/unit == i ){
			  printw(" * ");
		    }
			else{
				printw("     ");
			}
		}
	}

	fp.close();
	getch();
}

void article(){
	WINDOW *win=newwin(LINES, COLS, 2, 0);
	refresh();
	touchwin(win);

	PANEL *pan=new_panel(win);
	update_panels();
	doupdate();


	char ch;
	int spot=0, size_save;
	vector<int> errors;
	string text;
	vector< pair<int, int> >pos;
	text_init(win, text, size_save, pos);

	vector< pair<int, int> >::iterator it = pos.begin();

	// starting countdown
	clock_win("starting in", 3, 15, 2);

	// start timing
	auto START = steady_clock::now();

	 //start typing
	while (spot<text.size()-1){
		ch=wgetch(win);
		// control+d to stop 
		if(ch == ctrl('d')){
			del_panel(pan);
			delwin(win);
			return;
		}
		if(ch==DELETE){
			if(spot!=0){
				mvwaddch(win, spot/COLS, spot%COLS, text[spot]);
				--spot;
				// check if it is on a alignment
				if(spot == (it-1)->first+(it-1)->second-1){
					--it;
					spot-=it->second-1;
				}
				// check if it is on a error made previously
				if(errors.back()==spot){
					errors.pop_back();
				}
			}
			else{
				continue;
			}
		}
		else{
			if(ch==text[spot]){
				mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(green));
			}
			else{
				// acount the errors
				errors.push_back(spot);
				if(text[spot] == ' '){
					mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(bg_red));
				}
				else{
					mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(red));
				}
			}
			// check if the there it is in a alignment
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
	//wdelch(win);
	auto END = steady_clock::now();
	auto dur = duration_cast<seconds>(END-START);
	int score = (float)((size_save/5)-errors.size())/((float)dur.count()/60.00);
	mvwprintw(win, (text.size()/COLS)+1, 0, "wpm : %d", score);
	input_stats(score);
	wgetch(win);
	del_panel(pan);
	delwin(win);
}

void countdown(){
	WINDOW *win=newwin(LINES, COLS, 2, 0);
	refresh();
	touchwin(win);
	PANEL *pan=new_panel(win);
	update_panels();
	doupdate();

	auto clock_a = async(launch::async, clock_win, "countdown", 10, 0, LINES/2);

	
	char ch;
	int spot=0, size_save;
	vector<int> errors;
	string text;
	vector< pair<int, int> >pos;
	text_init(win, text, size_save, pos);

	vector< pair<int, int> >::iterator it = pos.begin();

	// start timing
	auto START = steady_clock::now();

	 //start typing
	while (spot<text.size()-1){
		ch=wgetch(win);
		// control+d to stop 
		if(ch == ctrl('d')){
			del_panel(pan);
			delwin(win);
			return;
		}
		if(ch==DELETE){
			if(spot!=0){
				mvwaddch(win, spot/COLS, spot%COLS, text[spot]);
				--spot;
				// check if it is on a alignment
				if(spot == (it-1)->first+(it-1)->second-1){
					--it;
					spot-=it->second-1;
				}
				// check if it is on a error made previously
				if(errors.back()==spot){
					errors.pop_back();
				}
			}
			else{
				continue;
			}
		}
		else{
			if(ch==text[spot]){
				mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(green));
			}
			else{
				// acount the errors
				errors.push_back(spot);
				if(text[spot] == ' '){
					mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(bg_red));
				}
				else{
					mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(red));
				}
			}
			// check if the there it is in a alignment
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
	wgetch(win);
	del_panel(pan);
	delwin(win);
}
