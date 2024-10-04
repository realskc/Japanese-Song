const fs = require('fs');

const verbose = true;
const debug = true;

const playlistsURL = [];
try {
	const data = fs.readFileSync('playlists.txt', 'utf-8');
	const lines = data.split(/\r?\n/);
	lines.forEach((line) => {
		id = Number(line.split('//')[0]);
		if(!isNaN(id)){
			playlistsURL.push(`https://api.yimian.xyz/msc/?type=playlist&id=${id}`);
		}
	})
	console.log('Playlists reading completed.');
} catch (err) {
	console.log(err);
}

const artistsURL = [];
try {
	const data = fs.readFileSync('artists.txt', 'utf-8');
	const lines = data.split(/\r?\n/);
	lines.forEach((line) => {
		id = Number(line.split('//')[0]);
		if(!isNaN(id)){
			for (let offset=0; offset<1000; offset+=100){
				artistsURL.push(`https://music.163.com/api/search/get?s=${id}&type=1&offset=${offset}&limit=100`);
			}
		}
	})
	console.log('Artists reading completed.');
} catch (err) {
	console.log(err);
}

const songs = [];
const delay = (ms) => new Promise(resolve => setTimeout(resolve, ms));
const promisePlaylists = (async () => {
	for (const url of playlistsURL){
		if(verbose) console.log(`Fetching ${url}`);
		const response = await fetch(url);
		const data = await response.json();
		for(let i of data){
			songs.push(i.id);
		}
		await delay(500);
	}
	console.log('Fetching playlists finished.')
})();
const promiseArtists = (async () => {
	for (const url of artistsURL){
		if(verbose) console.log(`Fetching ${url}`);
		const response = await fetch(url);
		const data = await response.json();
		if(data.result.songCount == 0) continue;
		for(let i of data.result.songs){
			songs.push(i.id);
		}
		await delay(500);
	}
	console.log('Fetching artists finished.')
})();
Promise.all([promisePlaylists, promiseArtists])
.then(() => {
	fs.writeFileSync('songs.txt', songs.join('\n'));
})
.catch((err) => {
	console.error(err);
	fs.writeFileSync('songs.txt', songs.join('\n'));
});