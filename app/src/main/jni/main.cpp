#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>

#include <nv_egl_util/nv_egl_util.h>

#include "kesk_native_app_glue.h"
#include "ui/GlView.h"

using namespace std;

//Change these 2 lines to implement your custom renderer
//#include "samples/s1_context/Renderer.h"
#include "samples/s2_program/Renderer.h"
#define RENDERER_CLASS Renderer

//Applications should override the APP_NAME
#define APP_NAME "KeskGL"

//Default logger implementation to avoid errors
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, APP_NAME, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  APP_NAME, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  APP_NAME, __VA_ARGS__))

/**
 *	Main entry point.
 *
 *	Implement
 */
void android_main(struct android_app* app)
{
    // Make sure glue isn't stripped.
    app_dummy();

    //Renderer instance
    RENDERER_CLASS renderer;

    // GlView instance
    GlView glView(app);

    //Root element of GlView
    glView.setRootElement(&renderer);
    glView.setFPSListener(&renderer);

    //Main loop
    glView.loop();
}
