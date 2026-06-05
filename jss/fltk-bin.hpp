#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

#include <quickjs/quickjs.h>

typedef struct {
    Fl_Window* win;
} JSFlWindow;

typedef struct {
    Fl_Button* btn;
    JSValue callback;
    JSContext* ctx;
} JSFlButton;

static JSValue js_fl_window(JSContext* ctx, JSValueConst this_val,
                            int argc, JSValueConst* argv);

static JSValue js_fl_button(JSContext* ctx, JSValueConst this_val,
                            int argc, JSValueConst* argv);

static void button_cb(Fl_Widget* w, void* data);

static JSValue js_button_onclick(JSContext* ctx, JSValueConst this_val,
                                 int argc, JSValueConst* argv);

static JSValue js_window_show(JSContext* ctx, JSValueConst this_val,
                              int argc, JSValueConst* argv);

static JSValue js_fltk_run(JSContext* ctx, JSValueConst this_val,
                           int argc, JSValueConst* argv);

static int js_fltk_init(JSContext* ctx, JSModuleDef* m);

static JSModuleDef* create_fltk_module(JSContext* ctx);
