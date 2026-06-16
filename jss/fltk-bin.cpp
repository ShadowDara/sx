#include "fltk-bin.hpp"

static JSValue window_proto = JS_UNDEFINED;
static JSValue button_proto = JS_UNDEFINED;

static void js_fl_window_finalizer(JSRuntime* rt, JSValue val)
{
    JSFlWindow* obj =
        (JSFlWindow*)JS_GetOpaque(val, js_fl_window_class_id);

    if (obj)
    {
        delete obj->win;
        js_free_rt(rt, obj);
    }
}

static void js_fl_button_finalizer(JSRuntime* rt, JSValue val)
{
    JSFlButton* obj =
        (JSFlButton*)JS_GetOpaque(val, js_fl_button_class_id);

    if (!obj)
        return;

    if (obj->btn)
    {
        obj->btn->callback(nullptr, nullptr);
    }

    if (!JS_IsUndefined(obj->callback))
    {
        JS_FreeValueRT(rt, obj->callback);
        obj->callback = JS_UNDEFINED;
    }

    delete obj->btn;
    obj->btn = nullptr;
    js_free_rt(rt, obj);
}

JSClassID js_fl_window_class_id;
JSClassID js_fl_button_class_id;

JSClassDef js_fl_window_class = {
    "FlWindow",
    js_fl_window_finalizer
};

JSClassDef js_fl_button_class = {
    "FlButton",
    js_fl_button_finalizer
};

static JSValue js_fl_window(JSContext* ctx, JSValueConst this_val,
                            int argc, JSValueConst* argv)
{
    int w, h;
    const char* title = nullptr;

    JS_ToInt32(ctx, &w, argv[0]);
    JS_ToInt32(ctx, &h, argv[1]);

    title = JS_ToCString(ctx, argv[2]);

    JSFlWindow* obj = (JSFlWindow*)js_mallocz(ctx, sizeof(JSFlWindow));

    obj->win = new Fl_Window(w, h, title ? title : "");

    if (title) JS_FreeCString(ctx, title);

    JSValue jsobj = JS_NewObjectClass(ctx, js_fl_window_class_id);

    JS_SetOpaque(jsobj, obj);

    return jsobj;
}

static JSValue js_fl_button(JSContext* ctx, JSValueConst this_val,
                            int argc, JSValueConst* argv)
{
    int x, y, w, h;
    const char* label = nullptr;

    JS_ToInt32(ctx, &x, argv[0]);
    JS_ToInt32(ctx, &y, argv[1]);
    JS_ToInt32(ctx, &w, argv[2]);
    JS_ToInt32(ctx, &h, argv[3]);

    label = JS_ToCString(ctx, argv[4]);

    JSFlButton* obj = (JSFlButton*)js_mallocz(ctx, sizeof(JSFlButton));

    obj->btn = new Fl_Button(x, y, w, h, label ? label : "");
    obj->ctx = ctx;
    obj->rt = JS_GetRuntime(ctx);
    obj->callback = JS_UNDEFINED;

    if (label) JS_FreeCString(ctx, label);

    JSValue jsobj = JS_NewObjectClass(ctx, js_fl_button_class_id);

    JS_SetOpaque(jsobj, obj);

    return jsobj;
}

static void button_cb(Fl_Widget* w, void* data)
{
    JSFlButton* obj = (JSFlButton*)data;

    if (!obj || !obj->rt || JS_IsUndefined(obj->callback))
        return;

    JSValue ret = JS_Call(
        obj->ctx,
        obj->callback,
        JS_UNDEFINED,
        0,
        nullptr);

    JS_FreeValue(obj->ctx, ret);
}

static JSValue js_button_onclick(JSContext* ctx, JSValueConst this_val,
                                 int argc, JSValueConst* argv)
{
    JSFlButton* obj =
        (JSFlButton*)JS_GetOpaque(this_val, js_fl_button_class_id);

    if (!obj || argc < 1)
        return JS_UNDEFINED;

    // free old callback (IMPORTANT FIX)
    if (!JS_IsUndefined(obj->callback))
    {
        JS_FreeValue(ctx, obj->callback);
    }

    obj->callback = JS_DupValue(ctx, argv[0]);

    obj->btn->callback(button_cb, obj);

    return JS_UNDEFINED;
}

static JSValue js_window_show(JSContext* ctx, JSValueConst this_val,
                              int argc, JSValueConst* argv)
{
    std::cout << "SHOW CALLED\n";
    JSFlWindow* obj =
    (JSFlWindow*)JS_GetOpaque(
        this_val,
        js_fl_window_class_id);
    obj->win->show();
    return JS_UNDEFINED;
}

static JSValue js_fltk_run(JSContext* ctx, JSValueConst this_val,
                           int argc, JSValueConst* argv)
{
    std::cout << "RUN CALLED\n";
    Fl::run();
    return JS_UNDEFINED;
}

static int js_fltk_init(JSContext* ctx, JSModuleDef* m)
{
    // =========================
    // WINDOW
    // =========================
    window_proto = JS_NewObject(ctx);

    JS_SetPropertyStr(
        ctx,
        window_proto,
        "show",
        JS_NewCFunction(ctx, js_window_show, "show", 0));

    JSValue window_ctor = JS_NewCFunction2(ctx, js_fl_window, "Window", 3, JS_CFUNC_constructor_or_func, 0);

    JS_SetConstructor(ctx, window_ctor, window_proto);
    JS_SetClassProto(ctx, js_fl_window_class_id, window_proto);

    JS_SetModuleExport(ctx, m, "Window", window_ctor);

    // =========================
    // BUTTON
    // =========================
    button_proto = JS_NewObject(ctx);

    JS_SetPropertyStr(
        ctx,
        button_proto,
        "onClick",
        JS_NewCFunction(ctx, js_button_onclick, "onClick", 1));

    JSValue button_ctor = JS_NewCFunction2(ctx, js_fl_button, "Button", 5, JS_CFUNC_constructor_or_func, 0);

    JS_SetConstructor(ctx, button_ctor, button_proto);
    JS_SetClassProto(ctx, js_fl_button_class_id, button_proto);

    JS_SetModuleExport(ctx, m, "Button", button_ctor);

    // =========================
    // RUN
    // =========================
    JS_SetModuleExport(ctx, m, "run",
        JS_NewCFunction(ctx, js_fltk_run, "run", 0));

    return 0;
}

JSModuleDef* create_fltk_module(JSContext* ctx)
{
    JSModuleDef* m = JS_NewCModule(ctx, "jss.fltk", js_fltk_init);

    JS_AddModuleExport(ctx, m, "Window");
    JS_AddModuleExport(ctx, m, "Button");
    JS_AddModuleExport(ctx, m, "run");

    return m;
}

void init_fltk_classes(JSRuntime* rt)
{
    JS_NewClassID(&js_fl_window_class_id);
    JS_NewClassID(&js_fl_button_class_id);

    JS_NewClass(rt,
                js_fl_window_class_id,
                &js_fl_window_class);

    JS_NewClass(rt,
                js_fl_button_class_id,
                &js_fl_button_class);
}

