.pragma library

function isLight(color) {
    // Convert to RGB
    color = +("0x" + color.toString().slice(1))
    var r = color >> 16;
    var g = color >> 8 & 255;
    var b = color & 255;

    // HSP (Highly Sensitive Poo) equation from http://alienryderflex.com/hsp.html
    var hsp = Math.sqrt(0.299 * (r * r) + 0.587 * (g * g) + 0.114 * (b * b))

    // Using the HSP value, determine whether the color is light or dark
    return hsp > 127.5
}
