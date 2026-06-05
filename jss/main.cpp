#include <quickjs/quickjs.h>

#include <iostream>
#include <cstring>

#include <fstream>
#include <sstream>
#include <filesystem>

#include "fltk-bin.hpp"

/// =========================
/// 1. MODULE: jss
/// =========================

static JSValue js_add(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    double a, b;
    JS_ToFloat64(ctx, &a, argv[0]);
    JS_ToFloat64(ctx, &b, argv[1]);
    return JS_NewFloat64(ctx, a + b);
}

static int js_jss_init(JSContext *ctx, JSModuleDef *m)
{
    JS_SetModuleExport(ctx, m, "add",
                       JS_NewCFunction(ctx, js_add, "add", 2));

    JS_SetModuleExport(ctx, m, "pi",
                       JS_NewFloat64(ctx, 3.141592653589793));

    return 0;
}

static JSValue js_console_log(
    JSContext *ctx,
    JSValueConst this_val,
    int argc,
    JSValueConst *argv)
{
    for (int i = 0; i < argc; i++)
    {
        const char *str = JS_ToCString(ctx, argv[i]);

        if (str)
        {
            std::cout << str;
            JS_FreeCString(ctx, str);
        }

        if (i + 1 < argc)
            std::cout << " ";
    }

    std::cout << std::endl;

    return JS_UNDEFINED;
}

void install_console(JSContext *ctx)
{
    JSValue global = JS_GetGlobalObject(ctx);

    JSValue console = JS_NewObject(ctx);

    JS_SetPropertyStr(
        ctx,
        console,
        "log",
        JS_NewCFunction(
            ctx,
            js_console_log,
            "log",
            1));

    JS_SetPropertyStr(
        ctx,
        global,
        "console",
        console);

    JS_FreeValue(ctx, global);
}

static JSModuleDef *create_jss_module(JSContext *ctx)
{
    JSModuleDef *m = JS_NewCModule(ctx, "jss", js_jss_init);
    if (!m)
        return nullptr;

    JS_AddModuleExport(ctx, m, "add");
    JS_AddModuleExport(ctx, m, "pi");
    return m;
}

/// =========================
/// 2. MODULE: LOADER
/// =========================

// Helper function to read a File
static std::string read_file(const char *path)
{
    std::ifstream file(path);

    if (!file)
        return {};

    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

static char *module_normalize(
    JSContext *ctx,
    const char *base_name,
    const char *name,
    void *opaque)
{
    namespace fs = std::filesystem;

    fs::path result;

    if (name[0] == '.')
    {
        fs::path base(base_name);

        result =
            base.parent_path() /
            fs::path(name);
    }
    else
    {
        result = fs::path(name);
    }

    result = result.lexically_normal();

    std::string s = result.string();

    char *out = static_cast<char *>(malloc(s.size() + 1));

    memcpy(out, s.c_str(), s.size() + 1);

    return out;
}

static JSModuleDef *module_loader(
    JSContext *ctx,
    const char *module_name,
    void *opaque)
{
    // 1. Native Module zuerst!
    if (strcmp(module_name, "jss") == 0)
    {
        return create_jss_module(ctx);
    }

    if (strcmp(module_name, "jss.fltk") == 0)
    {
        return create_fltk_module(ctx);
    }

    // 2. Datei laden
    std::string source = read_file(module_name);

    if (source.empty())
    {
        JS_ThrowReferenceError(
            ctx,
            "cannot load %s",
            module_name);

        return nullptr;
    }

    JSValue val = JS_Eval(
        ctx,
        source.c_str(),
        source.size(),
        module_name,
        JS_EVAL_TYPE_MODULE |
            JS_EVAL_FLAG_COMPILE_ONLY);

    if (JS_IsException(val))
        return nullptr;

    JSModuleDef *m = (JSModuleDef *)JS_VALUE_GET_PTR(val);

    JS_FreeValue(ctx, val); // 🔥 wichtig

    return m;
}

/// =========================
/// 3. MAIN
/// =========================

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        JSRuntime *rt = JS_NewRuntime();
        JSContext *ctx = JS_NewContext(rt);

        install_console(ctx);

        // 👇 DER WICHTIGE TEIL: Loader aktivieren
        JS_SetModuleLoaderFunc(rt, module_normalize, module_loader, nullptr);

        std::string code = read_file(argv[1]);

        JSValue result = JS_Eval(
            ctx,
            code.c_str(),
            code.size(),
            argv[1],
            JS_EVAL_TYPE_MODULE);

        if (JS_IsException(result))
        {
            JSValue exc = JS_GetException(ctx);
            const char *str = JS_ToCString(ctx, exc);
            std::cout << "JS ERROR: " << str << std::endl;
            JS_FreeCString(ctx, str);
            JS_FreeValue(ctx, exc);
            return 1;
        }

        JSValue global = JS_GetGlobalObject(ctx);
        JSValue res = JS_GetPropertyStr(ctx, global, "result");

        // double out;
        // JS_ToFloat64(ctx, &out, res);

        // std::cout << "Result = " << out << std::endl;

        JS_FreeValue(ctx, res);
        JS_FreeValue(ctx, global);
        JS_FreeValue(ctx, result);

        JS_RunGC(rt);

        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);

        return 0;
    }
    else
    {
        std::cout << "Add argument to run a file!" << std::endl;
    }
}
