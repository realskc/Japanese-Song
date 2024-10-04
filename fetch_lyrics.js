const fs = require('fs');
const { exec } = require('child_process');

const lyricsURL = [];
try {
	const data = fs.readFileSync('songs/deduplicated_songs.txt', 'utf-8');
	const lines = data.split(/\r?\n/);
	lines.forEach((line) => {
		id = Number(line.split('//')[0]);
		if(!isNaN(id)){
			lyricsURL.push([`https://music.163.com/api/song/lyric?os=pc&id=${id}&lv=-1`, id]);
		}
	})
	console.log('Songs reading completed.');
} catch (err) {
	console.log(err);
}

const verbose = true;
(async () => {
	for (const [url, id] of lyricsURL) {
		if(verbose) console.log(`Fetching ${url}`);
		const response = await fetch(url);
		const data = await response.json();
		if (data.lrc) {
			if (data.lrc.lyric) {
				fs.writeFile(`lyrics/${id}.txt`, data.lrc.lyric, (err) => {
					if (err) console.error(`Error writing to lyrics/${id}.txt\n`, err);
					else {
						exec(`md5sum lyrics/${id}.txt > lyrics/md5sum/${id}.md5`, (error, stdout, stderr) => {
							if (error) {
								console.error(`Error: ${error.message}`);
								return;
							}
							if (stderr) {
								console.error(`stderr: ${stderr}`);
								return;
							}
							if (stdout) {
								console.log(`stdout: ${stdout}`);
							}
						});
					}
				});
			}
			else console.error(url+' have no lyric.');
		}
		else console.error(url+' have no lrc.');
	}
})();