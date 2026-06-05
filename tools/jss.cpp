#include <quickjs/quickjs.h>
#include <iostream>
#include <cstring>

/// =========================
/// 1. MODULE: jss
/// =========================

static JSValue js_add(JSContext* ctx, JSValueConst this_val,
                      int argc, JSValueConst* argv)
{
    double a, b;
    JS_ToFloat64(ctx, &a, argv[0]);
    JS_ToFloat64(ctx, &b, argv[1]);
    return JS_NewFloat64(ctx, a + b);
}

static int js_jss_init(JSContext* ctx, JSModuleDef* m)
{
    JS_SetModuleExport(ctx, m, "add",
        JS_NewCFunction(ctx, js_add, "add", 2));

    JS_SetModuleExport(ctx, m, "pi",
        JS_NewFloat64(ctx, 3.141592653589793));

    return 0;
}

static JSModuleDef* create_jss_module(JSContext* ctx)
{
    JSModuleDef* m = JS_NewCModule(ctx, "jss", js_jss_init);
    if (!m) return nullptr;

    JS_AddModuleExport(ctx, m, "add");
    JS_AddModuleExport(ctx, m, "pi");
    return m;
}

/// =========================
/// 2. MODULE: LOADER
/// =========================

static JSModuleDef* module_loader(JSContext* ctx,
                                  const char* module_name,
                                  void* opaque)
{
    // unser eigenes Modul-System
    if (strcmp(module_name, "jss") == 0)
    {
        return create_jss_module(ctx);
    }

    JS_ThrowReferenceError(ctx, "module not found: %s", module_name);
    return nullptr;
}

/// =========================
/// 3. MAIN
/// =========================

int main()
{
    JSRuntime* rt = JS_NewRuntime();
    JSContext* ctx = JS_NewContext(rt);

    // 👇 DER WICHTIGE TEIL: Loader aktivieren
    JS_SetModuleLoaderFunc(rt, nullptr, module_loader, nullptr);

    const char* code = R"(
        import { add, pi } from "jss";

        globalThis.result = add(2, 3) + pi + 10;
    )";

    JSValue result = JS_Eval(
        ctx,
        code,
        strlen(code),
        "<input>",
        JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_STRICT
    );

    if (JS_IsException(result))
    {
        JSValue exc = JS_GetException(ctx);
        const char* str = JS_ToCString(ctx, exc);
        std::cout << "JS ERROR: " << str << std::endl;
        JS_FreeCString(ctx, str);
        JS_FreeValue(ctx, exc);
        return 1;
    }

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue res = JS_GetPropertyStr(ctx, global, "result");

    double out;
    JS_ToFloat64(ctx, &out, res);

    std::cout << "Result = " << out << std::endl;

    JS_FreeValue(ctx, res);
    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, result);

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    return 0;
}
