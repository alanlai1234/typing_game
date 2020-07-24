#include "side.cpp"
// TODO : able perform multi-paragraph article typing
// TODO : enhance wpm calculation

int main(){
	
	curse_init();
	printw("typing test---------------------------------------------\n\n");
	// menu
	vector <string> menu = {"1) article 2) count down q) exit"};
	int cur=0;
	char key;
	addstr("1) article 2) count down q) exit");
	key=getch();
	switch (key){
		case '1':
			article();
			break;
		case 'q':
			break;
	}
	
	//getch();
	endwin();
	return 0;

}

void article(){
	WINDOW *win=newwin(LINES, COLS, 2, 0);
	refresh();
	touchwin(win);
	//wrefresh(win);
	PANEL *pan=new_panel(win);
	update_panels();
	doupdate();

	
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
	vector<string> split=doSegment(text); // split the text with \n
	vector < vector< pair<int, int> > > pos; // access it when started typing
	for ( string i : split ){
		// generate alignment and print it out
		pos.push_back(alignment(i, COLS));
		wprintw(win, "%s\n", i.c_str());
	}

	fp.close();

	// setting for ready
	wmove(win, 0, 0);
	waddch(win, split[0][0] | A_UNDERLINE);
	vector< pair<int, int> >::iterator it = pos[0].begin();
	int errors=0;
	spot=0;

	// starting countdown
	clock_win("starting in", 3, 15, 2);

	// start timing
	auto START = steady_clock::now();

	//// start typing
	while(1){
		// TODO : split the string here
		while (text[spot-1]!='\n' || text[spot-1]!='\0'){
			ch=wgetch(win);
			if(ch=='1'){
				delwin(win);
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
					mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(green));
				}
				else{
					++errors;
					if(text[spot] == ' '){
						mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(bg_red));
					}
					else{
						mvwaddch(win, spot/COLS, spot%COLS, text[spot] | COLOR_PAIR(red));
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
			
			mvwaddch(win, spot/COLS, spot%COLS, text[spot] | A_UNDERLINE);
		}
		
		if(text[spot-1]=='\0'){
			break;
		}
		//else{
			
		//}
		
		}
	}
	auto END = steady_clock::now();
	auto dur = duration_cast<seconds>(END-START);
	mvwprintw(win, (text.size()/COLS)+1, 0, "wpm : %.*f", 0, ((size_save/5))/(dur.count()/60.000));
	wgetch(win);
	delwin(win);
}

