#include "fltk-bin.hpp"

static JSClassID js_fl_window_class_id;
static JSClassID js_fl_button_class_id;

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
    obj->callback = JS_UNDEFINED;

    if (label) JS_FreeCString(ctx, label);

    JSValue jsobj = JS_NewObjectClass(ctx, js_fl_window_class_id);

    JS_SetOpaque(jsobj, obj);

    return jsobj;
}

static void button_cb(Fl_Widget* w, void* data)
{
    JSFlButton* obj = (JSFlButton*)data;

    if (!JS_IsUndefined(obj->callback))
    {
        JSValue ret = JS_Call(
            obj->ctx,
            obj->callback,
            JS_UNDEFINED,
            0,
            nullptr);

        JS_FreeValue(obj->ctx, ret);
    }
}

static JSValue js_button_onclick(JSContext* ctx, JSValueConst this_val,
                                 int argc, JSValueConst* argv)
{
    JSFlButton* obj = (JSFlButton*)JS_GetOpaque(this_val, 0);

    obj->callback = JS_DupValue(ctx, argv[0]);

    obj->btn->callback(button_cb, obj);

    return JS_UNDEFINED;
}

static JSValue js_window_show(JSContext* ctx, JSValueConst this_val,
                              int argc, JSValueConst* argv)
{
    JSFlWindow* obj = (JSFlWindow*)JS_GetOpaque(this_val, 0);
    obj->win->show();
    return JS_UNDEFINED;
}

static JSValue js_fltk_run(JSContext* ctx, JSValueConst this_val,
                           int argc, JSValueConst* argv)
{
    Fl::run();
    return JS_UNDEFINED;
}

static int js_fltk_init(JSContext* ctx, JSModuleDef* m)
{
    JS_SetModuleExport(ctx, m, "Window",
        JS_NewCFunction(ctx, js_fl_window, "Window", 3));

    JS_SetModuleExport(ctx, m, "Button",
        JS_NewCFunction(ctx, js_fl_button, "Button", 5));

    JS_SetModuleExport(ctx, m, "run",
        JS_NewCFunction(ctx, js_fltk_run, "run", 0));

    return 0;
}

static JSModuleDef* create_fltk_module(JSContext* ctx)
{
    JSModuleDef* m = JS_NewCModule(ctx, "jss.fltk", js_fltk_init);

    JS_AddModuleExport(ctx, m, "Window");
    JS_AddModuleExport(ctx, m, "Button");
    JS_AddModuleExport(ctx, m, "run");

    return m;
}
