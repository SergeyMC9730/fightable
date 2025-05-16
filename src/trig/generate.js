let sin_table = {};
let cos_table = {};
let tg_table = {};
let ctg_table = {};

const PRECISION = 100;

function degress_to_rad(d = 0) {
  return d * (Math.PI / 180);
}
function rad_to_index(r = 0) {
  return Math.floor(r * PRECISION) % Math.floor(2 * Math.PI * PRECISION);
}
function deg_to_index(d = 0) {
  return (
    Math.floor(degress_to_rad(d) * PRECISION) %
    Math.floor(2 * Math.PI * PRECISION)
  );
}

for (let i = 0; i <= 360; i++) {
  const index = deg_to_index(i);

  sin_table[index] = Math.sin(degress_to_rad(i));
}

//console.log(sin_table);

console.log(sin_table[deg_to_index(30)]);
