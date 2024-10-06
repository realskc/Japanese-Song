#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
void warn_invalid_id(string str) {
	cerr << "[Warning] Invalid Song ID in " << str << endl;
}
bool is_kana(char32_t c){
	if (c >= 0x3040 && c <= 0x309F) return 1;
	if (c >= 0x30A0 && c <= 0x30FF) return 1;
	if (c >= 0x31F0 && c <= 0x31FF) return 1;
	return 0;
}
u32string utf8_to_utf32(const string &str) {
	wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
	return converter.from_bytes(str);
}
string utf32_to_utf8(const u32string &u32str) {
	wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
	return converter.to_bytes(u32str);
}
int main() {
	ifstream song_list("songs/deduplicated_songs.txt");
	if(!song_list) {
		cerr << "[Error] Cannot open deduplicated_songs.txt" << endl;
		return 1;
	}
	string line;
	vector<ll> songs;
	while(getline(song_list, line)) {
		ll x = stoll(line);
		if(x>0) songs.push_back(x);
		else warn_invalid_id("deduplicated_songs.txt");
	}
	song_list.close();

	set<ll> banned_songs;
	song_list.open("songs/banned_songs.txt");
	if(song_list) {
		while(getline(song_list, line)) {
			ll x = stoll(line);
			if(x>0) banned_songs.insert(x);
			else warn_invalid_id("banned_songs.txt");
		}
	}
	song_list.close();

	ifstream lyricsFile;
	vector<ll> cleaned_songs;
	for(ll id:songs){
		string filename = "lyrics/"+to_string(id)+".txt";
		lyricsFile.open(filename);
		string content((istreambuf_iterator<char>(lyricsFile)), istreambuf_iterator<char>());
		u32string utf32_content(utf8_to_utf32(content));
		int cnt = 0, cntEoln = 0;
		for(char32_t ch:utf32_content){
			if (is_kana(ch)) ++cnt;
			if (ch=='\n') ++cntEoln;
		}
		if(cnt <= 10 || cntEoln <= 4) {
			if(cntEoln > 4) {
				banned_songs.insert(id);
			}
		}
		else {
			cerr << "Processing #" << id << endl;
			cleaned_songs.push_back(id);
			size_t cur = 0;
			ofstream cleaned_song("cleaned_lyrics/"+to_string(id)+".txt");
			bool hasKana = 0;
			while(cur < utf32_content.size()) {
				size_t pos = utf32_content.find_first_of(U"\r\n", cur);
				u32string line = utf32_content.substr(cur, pos-cur);
				if(pos<utf32_content.size() && utf32_content[pos]=='\r') ++pos;
				pos = min(pos, utf32_content.size()-1);
				cur = pos+1;
				bool flag=1;
				for(char32_t ch:line) {
					if(!isspace(ch)){
						flag=0;
						break;
					}
				}
				if(flag) continue;
				if(!hasKana){
					for(char32_t ch:line) {
						if(is_kana(ch)){
							hasKana = 1;
							break;
						}
					}
				}
				if(!hasKana) continue;
				if(line.find(U"作词") != line.npos) continue;
				if(line.find(U"作曲") != line.npos) continue;
				if(line.find(U"编曲") != line.npos) continue;
				if(line.find(U"纯音乐，请欣赏") != line.npos) break;
				while(1) {
					size_t pos=line.find(U"]");
					if (pos==line.npos) break;
					line = line.substr(pos+1);
				}
				while(!line.empty()) {
					if(isspace(line[0])) {
						line = line.substr(1);
						continue;
					}
					if(isspace(line.back())) {
						line.pop_back();
						continue;
					}
					break;
				}
				if(line.empty()) continue;
				cleaned_song << utf32_to_utf8(line) << '\n';
			}
			cleaned_song.close();
		}
		lyricsFile.close();
	}

	ofstream cleaned_list("songs/cleaned_songs.txt");
	for(ll id:cleaned_songs) {
		cleaned_list << id << '\n';
	}
	cleaned_list.close();
	return 0;
}