LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#Module name
LOCAL_MODULE := keskgl_app

#Native Glue code
LOCAL_SRC_FILES := kesk_native_app_glue.c

#Kesk LIB
LOCAL_SRC_FILES += loader/API.cpp
LOCAL_SRC_FILES += shader/GlProgram.cpp
LOCAL_SRC_FILES += shader/GlShader.cpp
LOCAL_SRC_FILES += texture/GlTexture.cpp
LOCAL_SRC_FILES += texture/GlTextureSet.cpp
LOCAL_SRC_FILES += tools/GLES20Utils.cpp
LOCAL_SRC_FILES += ui/GlElement.cpp
LOCAL_SRC_FILES += ui/GlView.cpp
LOCAL_SRC_FILES += ContextManager.cpp
LOCAL_SRC_FILES += GlAsset.cpp
LOCAL_SRC_FILES += GlContext.cpp
LOCAL_SRC_FILES += main.cpp

#To exclude from the final build and replace by application code (Renderer)
#Don't forget to set the renderer in main.cpp too
//LOCAL_SRC_FILES += samples/s1_context/Renderer.cpp
LOCAL_SRC_FILES += samples/s2_program/Renderer.cpp
LOCAL_SRC_FILES += samples/s2_program/AmbientShader.cpp

#Target in local
LOCAL_ARM_MODE   := arm

#Libs to add
LOCAL_LDLIBS := -lstdc++ -lc -lm -llog -landroid -ldl -lGLESv2 -lEGL
LOCAL_STATIC_LIBRARIES := nv_egl_util nv_bitfont nv_math nv_glesutil nv_hhdds nv_log nv_shader nv_file nv_thread 

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, ./libs/jni)

$(call import-module,nv_egl_util)
$(call import-module,nv_bitfont)
$(call import-module,nv_math)
$(call import-module,nv_glesutil)
$(call import-module,nv_hhdds)
$(call import-module,nv_log)
$(call import-module,nv_shader)
$(call import-module,nv_file)
$(call import-module,nv_thread)
