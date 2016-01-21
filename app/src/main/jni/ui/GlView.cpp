/*
 * GlView.cpp
 *
 *  Created on: Apr 12, 2013
 *      Author: Thomas MILLET
 */
#include <deque>

#include <pthread.h>

#include <android/input.h>

#include <EGL/eglplatform.h>
#include <EGL/egl.h>

#include <nv_egl_util/nv_egl_util.h>

#include "kesk_native_app_glue.h"
#include "ContextManager.h"
#include "GlContext.h"

#include "GlView.h"

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "GlView", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GlView", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "GlView", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "GlView", __VA_ARGS__))

//ConfigChooser is handled statically to avoid refactoring of NVidia code
EGLint GlView::redSize = 8;
EGLint GlView::greenSize = 8;
EGLint GlView::blueSize = 8;
EGLint GlView::alphaSize = 8;
EGLint GlView::depthSize = 16;
EGLint GlView::stencilSize = 0;

GlView::GlView(struct android_app* uiContext){
	//LOGD("NEW");
	this->glContext = new GlContext();
	this->uiContext = uiContext;
	this->egl = NvEGLUtil::create(GlView::defaultEGLChooser);
	this->rootElement = NULL;
	this->fpsListener = NULL;
	this->resizePending = false;
	this->paused = false;
	this->forceRender = 4;
	this->fpsBufferIndex = 0;
	this->screenSize[0] = this->screenSize[1] = 0;

	uiContext->userData = this;
	uiContext->onAppCmd = &GlView::handleCmdThunk;
	uiContext->onInputEvent = &GlView::handleInputThunk;
	uiContext->onUserCmd = &GlView::handleUserCmdThunk;
	uiContext->userCmdDataSize = sizeof(ContextManager::Runnable);
}

GlView::~GlView() {
	//LOGD("DELETE");
	delete this->glContext;
	delete this->egl;
}

bool GlView::defaultEGLChooser(EGLDisplay disp, EGLConfig& bestConfig){
	//LOGD("defaultEGLChooser()");
	EGLint count = 0;
	if (!eglGetConfigs(disp, NULL, 0, &count))
	{
		LOGE("defaultEGLChooser cannot query count of all configs");
		return false;
	}

	//LOGD("Config count = %d", count);

	EGLConfig* configs = new EGLConfig[count];
	if (!eglGetConfigs(disp, configs, count, &count))
	{
		LOGE("defaultEGLChooser cannot query all configs");
		return false;
	}

	int bestMatch = 1<<30;
	int bestIndex = -1;

	int i;
	for (i = 0; i < count; i++)
	{
		int match = 0;
		EGLint surfaceType = 0;
		EGLint blueBits = 0;
		EGLint greenBits = 0;
		EGLint redBits = 0;
		EGLint alphaBits = 0;
		EGLint depthBits = 0;
		EGLint stencilBits = 0;
		EGLint renderableFlags = 0;

		eglGetConfigAttrib(disp, configs[i], EGL_SURFACE_TYPE, &surfaceType);
		eglGetConfigAttrib(disp, configs[i], EGL_BLUE_SIZE, &blueBits);
		eglGetConfigAttrib(disp, configs[i], EGL_GREEN_SIZE, &greenBits);
		eglGetConfigAttrib(disp, configs[i], EGL_RED_SIZE, &redBits);
		eglGetConfigAttrib(disp, configs[i], EGL_ALPHA_SIZE, &alphaBits);
		eglGetConfigAttrib(disp, configs[i], EGL_DEPTH_SIZE, &depthBits);
		eglGetConfigAttrib(disp, configs[i], EGL_STENCIL_SIZE, &stencilBits);
		eglGetConfigAttrib(disp, configs[i], EGL_RENDERABLE_TYPE, &renderableFlags);
		//LOGD("Config[%d]: R%dG%dB%dA%d D%dS%d Type=%04x Render=%04x", i, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, surfaceType, renderableFlags);

		if ((surfaceType & EGL_WINDOW_BIT) == 0)
			continue;
		if ((renderableFlags & EGL_OPENGL_ES2_BIT) == 0)
			continue;
		if (depthBits < GlView::depthSize)
			continue;
		if (stencilBits < GlView::stencilSize)
			continue;
		if ((redBits < GlView::redSize) || (greenBits < GlView::greenSize) || (blueBits < GlView::blueSize) || (alphaBits < GlView::alphaSize))
			continue;

		int penalty = depthBits - GlView::depthSize;
		match += penalty * penalty;
		penalty = redBits - GlView::redSize;
		match += penalty * penalty;
		penalty = greenBits - GlView::greenSize;
		match += penalty * penalty;
		penalty = blueBits - GlView::blueSize;
		match += penalty * penalty;
		penalty = alphaBits - GlView::alphaSize;
		match += penalty * penalty;
		penalty = stencilBits - GlView::stencilSize;
		match += penalty * penalty;

		if ((match < bestMatch) || (bestIndex == -1))
		{
			bestMatch = match;
			bestIndex = i;
			//LOGD("Config[%d] is the new best config", i, configs[i]);
		}
	}

	if (bestIndex < 0)
	{
		delete[] configs;
		return false;
	}

	bestConfig = configs[bestIndex];
	delete[] configs;

	return true;
}

void GlView::loop(){
	//LOGD("doLoop");
	if (!this->egl || !this->rootElement){
		// if we have a basic EGL failure, we need to exit immediately; nothing else we can do
		nv_app_force_quit_no_cleanup(this->uiContext);
		return;
	};

	this->mustRecreate = true;

	long lastTime = this->egl->getSystemTime();

	// loop waiting for stuff to do.

	while (nv_app_status_running(this->uiContext)){
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not rendering, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident = ALooper_pollAll(((nv_app_status_focused(this->uiContext) &&
										  !this->paused) ? 1 : 250),
										NULL,
										&events,
										(void**)&source)) >= 0){

			// If we timed out, then there are no pending messages
			if (ident == ALOOPER_POLL_TIMEOUT){
				break;
			}

			// Process this event.
			if (source != NULL && source->process != NULL){
				source->process(this->uiContext, source);
			}

			// Check if we are exiting.  If so, dump out
			if (!nv_app_status_running(this->uiContext)){
				break;
			}

			//Force only ONE user cmd per frame
			if(ident == LOOPER_ID_USER){
				break;
			}
		}

		long currentTime = egl->getSystemTime();

		// clamp time - it must not go backwards, and we don't
		// want it to be more than a half second to avoid huge
		// delays causing issues.  Note that we do not clamp to above
		// zero because some tools will give us zero delta
		long deltaTime = currentTime - lastTime;
		if (deltaTime < 0){
			deltaTime = 0;
		}
		else if (deltaTime > 500){
			deltaTime = 500;
		}

		lastTime = currentTime;

		// Update the frame, which optionally updates time and animations
		// and renders
		this->updateFrame(nv_app_status_interactable(this->uiContext), deltaTime);
	}
}

void GlView::handleCommand(int cmd){
	//LOGD("handleCommand()");
	switch (cmd){
		// The window is being shown, get it ready.
		// Note that on ICS, the EGL size will often be correct for the new size here
		// But on HC it will not be.  We need to defer checking the new res until the
		// first render with the new surface!
		case APP_CMD_INIT_WINDOW:
		case APP_CMD_WINDOW_RESIZED:
			this->resizePending = true;
			this->egl->setWindow(this->uiContext->window);
			this->requestForceRender();
			break;

		case APP_CMD_TERM_WINDOW:
			// The window is being hidden or closed, clean it up.
			this->egl->setWindow(NULL);
			break;

		case APP_CMD_GAINED_FOCUS:
			this->play();
			break;
		case APP_CMD_LOST_FOCUS:
		case APP_CMD_PAUSE:
			// Move out of gameplay mode if we are in it.  But if we are
			// in another dialog mode, leave it as-is
			this->pause();
			break;

		// ICS does not appear to require this, but on GB phones,
		// not having this causes rotation changes to be delayed or
		// ignored when we're in a non-rendering mode like autopause.
		// The forced renders appear to allow GB to process the rotation
		case APP_CMD_CONFIG_CHANGED:
			this->requestForceRender();
			break;
		case APP_CMD_DESTROY:
			this->rootElement->free();
			break;
	}
}

void GlView::updateFrame(bool interactible, long deltaTime){
	//LOGD("updateFrame()");
	if (interactible){
		// Each frame, we check to see if the window has resized.  While the
		// various events we get _should_ cover this, in practice, it appears
		// that the safest move across all platforms and OSes is to check at
		// the top of each frame
		this->checkWindowResized();

		// Time stands still when we're auto-paused, and we don't
		// automatically render
		if(!this->paused){

			// This will try to set up EGL if it isn't set up
			// When we first set up EGL completely, we also load our GLES resources
			// If these are already set up or we succeed at setting them all up now, then
			// we go ahead and render.
			this->renderFrame(true, deltaTime);
		}
		// forced rendering when needed for UI, etc
		else if (this->isForcedRenderPending()){
			this->renderFrame(true, deltaTime);
		}
	}
	else{
		// Even if we are not interactible, we may be visible, so we
		// HAVE to do any forced renderings if we can.  We must also
		// check for resize, since that may have been the point of the
		// forced render request in the first place!
		if (this->isForcedRenderPending() && this->egl->isReadyToRender(false)){
			this->checkWindowResized();
			this->renderFrame(false, deltaTime);
		}
	}
}

bool GlView::renderFrame(bool allocateIfNeeded, long deltaTime){
	//LOGD("renderFrame()");
	if(!this->egl->isReadyToRender(allocateIfNeeded)){
		return false;
	}

	this->resizeIfNeeded();

	if (this->forceRender > 0){
		this->forceRender--;
	}

	if(this->fpsListener){
		if(this->fpsBufferIndex < sizeof(this->fpsBuffer)/sizeof(int)){
			this->fpsBuffer[this->fpsBufferIndex++] = deltaTime;
		}
		else{
			int total = 0;
			for(int index=0; index < sizeof(this->fpsBuffer)/sizeof(int); index++){
				total += this->fpsBuffer[index];
			}
			if(total > this->fpsBufferIndex){
				this->fpsListener->onFpsRefresh(1000/(total/fpsBufferIndex));
			}
			this->fpsBufferIndex = 0;
		}
	}

	this->rootElement->layout(deltaTime);

	this->rootElement->draw();

	this->egl->swap();

	return true;
}

bool GlView::resizeIfNeeded(){
	//LOGD("resizeIfNeeded()");
	if(this->mustRecreate){
		this->rootElement->create(this);
		this->mustRecreate = false;
	}

	if (!this->resizePending){
		return false;
	}

	this->screenSize[0] = this->egl->getWidth();
	this->screenSize[1] = this->egl->getHeight();
	this->rootElement->resize(this->screenSize[0], this->screenSize[1]);
	this->resizePending = false;

	return true;
}

void GlView::play(){
	//LOGD("play()");
	if(this->paused){
		this->paused = false;
		this->requestForceRender();
	}
}

void GlView::pause(){
	//LOGD("pause()");
	if(!this->paused){
		this->paused = true;
		this->requestForceRender();
	}
}

bool GlView::checkWindowResized(){
	//LOGD("checkWindowResized()");
	if (this->egl->checkWindowResized()
			&& (this->screenSize[0] != this->egl->getWidth() || this->screenSize[1] != this->egl->getHeight())){
		this->requestForceRender();
		this->resizePending = true;
		return true;
	}

	return false;
}

int32_t GlView::handleInputThunk(struct android_app* uiContext, AInputEvent* event){
	GlView* glView = (GlView*)uiContext->userData;
	if (!glView){
		return 0;
	}

	return glView->rootElement->handleInput(event);
}

/**
 * Static thunk on GlView::handleCommand()
 */
void GlView::handleCmdThunk(struct android_app* uiContext, int32_t cmd){
	GlView* glView = (GlView*)uiContext->userData;
	if (!glView){
		return;
	}

	glView->handleCommand(cmd);
}

/**
 * Static thunk on ContextManager::Runnable->run()
 */
void GlView::handleUserCmdThunk(struct android_app* uiContext, void **data){
	//LOGD("handleUserCmdThunk()");
	GlView* glView = (GlView*)uiContext->userData;
	ContextManager::Runnable* runnable = reinterpret_cast<ContextManager::Runnable*>(data);
	if (!glView){
		return;
	}
	runnable->run(glView);
}

void GlView::runOnUITread(ContextManager::Runnable *runnable){
	kesk_app_write_ui_user_cmd(this->uiContext,runnable);
}

void GlView::runOnGLTread(ContextManager::Runnable *runnable){
	kesk_app_write_gl_user_cmd(this->uiContext,runnable);
}
