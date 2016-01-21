#include <jni.h>

#ifndef _Included_fr_kesk_libgl_tools_GLES20Utils
#define _Included_fr_kesk_libgl_tools_GLES20Utils
#ifdef __cplusplus
extern "C" {
#endif

// JNI -> fr.kesk.gl.GLES20Utils.glVertexAttribPointer(int index, int size, int type, boolean normalized, int stride, int offset)
JNIEXPORT void JNICALL Java_fr_kesk_libgl_tools_GLES20Utils_glVertexAttribPointer(JNIEnv *, jclass, jint, jint, jint, jboolean, jint, jint);

// JNI -> fr.kesk.gl.GLES20Utils.glDrawElements(int mode, int count, int type, int offset)
JNIEXPORT void JNICALL Java_fr_kesk_libgl_tools_GLES20Utils_glDrawElements(JNIEnv *, jclass, jint, jint, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
