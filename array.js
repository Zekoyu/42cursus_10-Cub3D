const fs = require("fs");
const content = fs.readFileSync("./test1.xpm", "utf8");

const list = JSON.parse(`[${[...content.matchAll(/{(.+)}/gs)][0][1]}]`);
const [width, height, numberOfColors, keyLength] = list[0].split(" ").map(Number);

const colorsReg = new RegExp(`^"([^\\s]{${keyLength}}) c #([\\dA-Fa-f]{6})",$`, "gm");
const colors = [...content.matchAll(colorsReg)].reduce((a, c) => {
	a[c[1]] = c[2];
	return a;
}, {});

function chunkify(string, length)
{
	return ([...string.matchAll(new RegExp(`.{1,${length}}`, "g"))]).map(s => s.join(""));
}

let map = list.slice(list.length - height, list.length);
map = map.map(line => chunkify(line, keyLength));
map = map.map(line => line.map(key => `0x00${colors[key]}`));
fs.writeFileSync("./out", `int texture[${height}][${width}] = {\n${map.map((l, i) => `\t{${l.join(", ")}}${i != map.length - 1 ? ',' : ''}`).join("\n")}\n}`);
