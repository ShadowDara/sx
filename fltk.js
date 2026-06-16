import { Window, Button, run } from "jss.fltk";

let win = Window(400, 300, "JS FLTK");
let btn = Button(100, 100, 200, 50, "Click me", () => {
    console.log("Button clicked!");
});
win.add(btn);
win.show();
run();