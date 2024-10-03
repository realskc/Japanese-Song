//爬取歌单搜索页面的结果
function decodeHtmlEntities(str) {
    const parser = new DOMParser();
    const doc = parser.parseFromString(str, "text/html");
    return doc.documentElement.textContent;
}
let playlists = document.querySelector('table > tbody');
let lists = '';
for(let item of playlists.childNodes){
	let x = item.querySelector('div.tt > div.ttc > span > a');
	let id = Number(x.href.split('id=').pop());
	let str = x.title;
	lists += String(id) + ' //' + decodeHtmlEntities(str) + '\n';
}
console.log(lists);