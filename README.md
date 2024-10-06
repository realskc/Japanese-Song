# Japanese-Song
整理日语歌曲中的高频词/汉字词

## 具体流程

1. 收集一些歌单或歌手。
1. 爬取他们的歌，得到歌曲列表。
1. 对歌曲 ID 去重。
1. 爬取歌词，得到若干歌词文件。

	https://note.ldper.com/netease-music-api-interface.html
	https://blog.csdn.net/tsumikistep/article/details/140356305

1. 扫描每一首歌，把非日语歌加入 ban list，并将歌词浓缩进若干文本文件中。
1. （使用其它开源软件）对歌词进行词语划分。

    Mecab: https://zhuanlan.zhihu.com/p/136691735

1. 如果可以的话，将重复单词进行合并（例如一个汉字词和它的假名，或动词的不同时态）。
1. 对于一个单词 $x$，定义它的权值为 $\sum\limits_{y\in\mathrm{SONG}}\ln(\mathrm{occur}(y,x)+1)$。计算所有单词的权值。
1. 将所有单词按权值排序，得到高频词列表。
1. 给汉字标注读音，最好能爬取释义并附着其上。
1. 附加：给歌按热度排序，选取热门歌中的例句附着在高频词列表上。

### 收集歌单与歌手

歌单是直接搜的，歌手是乱选的。

歌单存放于 `songs/playlists.txt` 中，歌手存放于 `songs/artists.txt` 中。

### 生成歌曲列表

歌单使用 `https://api.yimian.xyz/msc/?type=playlist&id=ID` 爬取。

歌手使用 `https://music.163.com/api/search/get?s=ID&type=1&offset=OFFSET&limit=100` 爬取前 $1000$ 首作品。

得到的歌曲 ID 存放于 `songs/songs.txt` 中。

### 对歌曲去重

使用 C++ 去重，去重后的歌曲 ID 存放于 `songs/deduplicated_songs.txt` 中。

### 爬取歌词

通过 `https://music.163.com/api/song/lyric?os=pc&id=SONGID&lv=-1` 爬取。

得到的歌词存放于 `lyrics/SONGID.txt` 中，同时将其 md5 存放于 `lyrics/md5sum/SONGID.md5` 中，通过校验的歌词视为爬取完毕，无需再次爬取。

### 提纯歌词

提纯后的歌词存放于 `cleaned_lyrics/SONGID.txt` 中，清洗后的歌曲列表为 `songs/cleaned_songs.txt`。