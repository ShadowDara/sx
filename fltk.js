import { Window, Button, run } from "jss.fltk";

console.log("FLZK WIndows")

let win = Window(400, 300, "JS FLTK");

console.log("After Win");

let btn = Button(100, 100, 200, 50, "Click me");

console.log("After But");


console.log("After Onclick");


win.show();

console.log("After Show");

run();

console.log("End");
