#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
set<ll> songs;
void warnInvalidId(string str) {
	cerr << "[Warning] Invalid Song ID in " << str << endl;
}
int main(int argc, char **argv) {
	fstream file;
	file.open("songs/deduplicated_songs.txt", ios::in);
	if(file) {
		string line;
		while(getline(file, line)) {
			long long x = stoll(line);
			if(x>0) songs.insert(x);
			else warnInvalidId("deduplicated_songs.txt");
		}
	}
	file.close();
	cerr << "Finished loading deduplicated_songs.txt" << endl;

	file.open("songs/songs.txt", ios::in);
	if(!file) {
		cerr << "[Error] Cannot open songs.txt" << endl;
		return 1;
	}
	string line;
	while(getline(file, line)) {
		long long x = stoll(line);
		if(x>0) songs.insert(x);
		else warnInvalidId("songs.txt");
	}
	file.close();
	cerr << "Finished loading songs.txt" << endl;

	file.open("songs/deduplicated_songs.txt", ios::out);
	for(auto id:songs){
		file << id << '\n';
	}
	file.close();
	cerr << "Finished generating deduplicated_songs.txt" << endl;
	return 0;
}