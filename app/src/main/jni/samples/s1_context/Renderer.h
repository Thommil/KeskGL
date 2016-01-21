/*
 * Renderer.h
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include <jni.h>

#include <android/input.h>

#include "ContextManager.h"
#include "GlContext.h"
#include "ui/GlElement.h"
#include "ui/GlView.h"

using namespace std;

#ifndef S1_CONTEXT_RENDERER_H_
#define S1_CONTEXT_RENDERER_H_

/**
 * Context and view simple demo and minimum implementation
 * of renderer.
 */
class Renderer : public GlElement, public GlView::FPSListener{
public :
	class RunnableTest : public ContextManager::Runnable{
		private:
			const char* text;
		public:
			RunnableTest(const char* text){this->text = text;}
			void run(ContextManager *contextManager);
	};

protected:
	ContextManager *contextManager;

public:
	Renderer();
	virtual ~Renderer();

	inline virtual GLuint getId() const{
		return 0;
	}

	void onFpsRefresh(int fps);

protected:
	void onCreate(ContextManager *contextManager);
	void onResize(int width, int height);
	void onLayout(long elapsedTime);
	void onDraw();
	int onEvent(AInputEvent* event);
	void onFree();
};

#endif /* S1_CONTEXT_RENDERER_H_ */
