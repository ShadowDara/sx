import { Window, Button, run } from "jss.fltk";

let win = Window(400, 300, "JS FLTK");

let btn = Button(100, 100, 200, 50, "Click me");

btn.onClick(() => {
    console.log("clicked!");
});

win.show();
run();
