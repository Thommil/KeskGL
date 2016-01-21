#include "GLES20Utils.h"

#include <GLES2/gl2.h>

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "GLES20Utils", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GLES20Utils", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "GLES20Utils", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "GLES20Utils", __VA_ARGS__))

void Java_fr_kesk_libgl_tools_GLES20Utils_glVertexAttribPointer
  (JNIEnv *env, jclass c, jint index, jint size, jint type, jboolean normalized, jint stride, jint offset)
{
	glVertexAttribPointer(index, size, type, normalized, stride, (void*) offset);
}

void Java_fr_kesk_libgl_tools_GLES20Utils_glDrawElements
  (JNIEnv *env, jclass c, jint mode, jint count, jint type, jint offset)
{
	glDrawElements(mode, count, type, (void*) offset);
}
