/*
 * GlView.h
 *
 *	Greatly based on NVidia work, see "resources" in current project
 *
 *  Created on: Apr 12, 2013
 *      Author: Thomas MILLET
 */
#include <android/input.h>

#include <EGL/eglplatform.h>
#include <EGL/egl.h>

#include <nv_egl_util/nv_egl_util.h>

#include "kesk_native_app_glue.h"
#include "ContextManager.h"
#include "GlContext.h"
#include "ui/GlElement.h"

using namespace std;

#ifndef GLVIEW_H_
#define GLVIEW_H_

/**
 *	Main GL entry point :
 *		- bridge between GL/UI threads
 *		- handle EGL
 *		- main controller based on NV/Google code use
 */
class GlView : public ContextManager{
public:
	/**
	 * Listener notified of FPS QoS
	 */
	 class FPSListener{
	 public :

		/**
		 * Called each time the FPS is calculated
		 */
		virtual void onFpsRefresh(int fps) = 0;

		virtual ~FPSListener() {}
	};

public:
	//EGL Config
	static EGLint redSize;
	static EGLint greenSize;
	static EGLint blueSize;
	static EGLint alphaSize;
	static EGLint depthSize;
	static EGLint stencilSize;

protected:
	GlContext *glContext;
	struct android_app* uiContext;

	GlView::FPSListener *fpsListener;

	NvEGLUtil* egl;

	GlElement* rootElement;

	bool resizePending;

	bool paused;

	int forceRender;

	bool mustRecreate;

private:
	int fpsBuffer[100];
	int fpsBufferIndex;
	int screenSize[2];

public:
	GlView(struct android_app* uiContext);
	virtual ~GlView();

	/**
	 * Default GlView EGL config chooser (RGBA8 BUFFER16 STENCIL0)
	 */
	static bool defaultEGLChooser(EGLDisplay disp, EGLConfig& bestConfig);

	/**
	 * Set the config chooser based on settings
	 */
	static void prepareConfigChooser(const int redSize, const int greenSize, const int blueSize, const int alphaSize, const int depthSize, const int stencilSize){
		GlView::redSize = redSize;
		GlView::greenSize = greenSize;
		GlView::blueSize = blueSize;
		GlView::alphaSize = alphaSize;
		GlView::depthSize = depthSize;
		GlView::stencilSize = stencilSize;
	}

	/**
	 * Set the root element
	 */
	void setRootElement(const GlElement* rootElement){ this->rootElement = (GlElement*)rootElement; }

	/**
	 * Get the root element
	 */
	GlElement* getRootElement(){ return this->rootElement;}

	/**
	 * Set fps listener
	 */
	void setFPSListener(const GlView::FPSListener* fpsListener){ this->fpsListener = (FPSListener*)fpsListener; }

	/**
	 * Removes the current fps listener
	 */
	void removeFPSListener(){this->fpsListener = NULL;}

	/**
	 * Called before rendering if some resize is needed
	 */
	bool resizeIfNeeded();

	/**
	 * Main loop startup
	 */
	virtual void loop();

	/**
	 * Play if paused
	 */
	virtual void play();

	/**
	 * Pause if running
	 */
	virtual void pause();

	/**
	 * Static thunk on GlView::runOnGLTread()
	 */
	static void handleUserCmdThunk(struct android_app* app, void** data);

	/**
	 * Static thunk on GlView::handleCommand()
	 */
	static void handleCmdThunk(struct android_app* app, int32_t cmd);

	/**
	 * Static thunk on GlView::handleInput()
	 */
	static int32_t handleInputThunk(struct android_app* app, AInputEvent* event);

	/**
	 * Process the next input command.
	 */
	void handleCommand(int cmd);

	/**
	 * Called each time the frame must be updated
	 */
	void updateFrame(bool interactible, long deltaTime);

	/**
	 * ContextManager Impl.
	 */

	virtual GlContext* getGLContext() const{ return this->glContext; }

	virtual struct android_app* getUIContext() const{ return this->uiContext; }

	virtual void runOnUITread(ContextManager::Runnable *runnable);

	virtual void runOnGLTread(ContextManager::Runnable *runnable);

protected:
	/**
	 * Indicates if rendering update has been queried
	 */
	bool isForcedRenderPending() { return this->forceRender > 0; }

	/**
	 * Asks for a forced rendering
	 */
	void requestForceRender() { this->forceRender = 4; }

	/**
	 * Check if window has been resized
	 */
	bool checkWindowResized();

	/**
	 * Draw method
	 */
	bool renderFrame(bool allocateIfNeeded, long deltaTime);

};
#endif /* GLVIEW_H_ */

