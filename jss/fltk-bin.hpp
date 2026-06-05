#pragma once

#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

#include <quickjs/quickjs.h>

extern JSClassID js_fl_window_class_id;
extern JSClassID js_fl_button_class_id;

extern JSClassDef js_fl_window_class;
extern JSClassDef js_fl_button_class;

typedef struct {
    Fl_Window* win;
} JSFlWindow;

typedef struct {
    Fl_Button* btn;
    JSValue callback;
    JSRuntime* rt;  
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

JSModuleDef* create_fltk_module(JSContext* ctx);

void init_fltk_classes(JSRuntime* rt);
