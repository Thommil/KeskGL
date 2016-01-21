/*
 * GlElement.h
 *
 *  Created on: Apr 12, 2013
 *      Author: Thomas MILLET
 */
#include <GLES2/gl2.h>

#include "ContextManager.h"

using namespace std;

#ifndef GLELEMENT_H_
#define GLELEMENT_H_

/**
 * Generic graphic element definition.
 *
 * When used in GlView, all method are called in GL Thread
 */
class GlElement {
public:
	virtual ~GlElement(){}

	/**
	 * Get the element ID
	 *
	 * @return The element id
	 */
	virtual GLuint getId() const = 0;

	/**
	 * Create and initialize the element.
	 *
	 * Default implementation just call the onCreate method.
	 *
	 * @param contextManager The ContextManager based on owner state
	 *
	 */
	void create(ContextManager *contextManager) {
		this->onCreate(contextManager);
	}

	/**
	 * Called when viewport dimensions has changed
	 *
	 * Default implementation just call the onLayout method.
	 *
	 * @param width width of the viewport
	 * @param height height of the viewport
	 *
	 */
	void resize(GLsizei width, GLsizei height){
		this->onResize(width, height);
	}

	/**
	 * Recalculate element assets based elapsed time since last draw call.
	 *
	 * Default implementation just call the onLayout method.
	 *
	 * @param elapsedTime The time elapsed since last draw call in ms
	 *
	 */
	void layout(long elapsedTime){
		this->onLayout(elapsedTime);
	}

	/**
	 * Draws element on GL surface.
	 *
	 * Default implementation just call the onDraw method.
	 */
	void draw(){
		this->onDraw();
	}

	/**
	 * Called when events are fired at GlElement
	 */
	int handleInput(AInputEvent* event){
		return this->onEvent(event);
	}

	/**
	 * Called to free any resource attached to this element
	 *
	 * Default implementation just call the onFree method.
	 */
	void free(){
		this->onFree();
	}

protected:

	/**
	 * Called at element creation, should be overridden by subclasses for extra init.
	 *
	 * @param contextManager The ContextManager based on owner state
	 *
	 */
	virtual void onCreate(ContextManager *contextManager) = 0;

	/**
	 * Called when viewport dimensions has changed, should be overridden by subclasses
	 * to implement specific layout operations on resize.
	 *
	 * @param width width of the viewport
	 * @param height height of the viewport
	 *
	 */
	virtual void onResize(GLsizei width, GLsizei height) = 0;

	/**
	 * Called at element assets layout, should be overridden by subclasses to implement
	 * specific layout operations
	 *
	 * @param elapsedTime The time elapsed since last draw call in ms
	 *
	 */
	virtual void onLayout(long int elapsedTime) = 0;

	/**
	 * Called at element drawing
	 *
	 * This method MUST be dedicated to drawing only. Size, position and
	 * any vertex attribute must be calculated in layout().
	 */
	virtual void onDraw() = 0;

	/**
	 * Called when events are fired at GlElement
	 */
	virtual int onEvent(AInputEvent* event) = 0;

	/**
	 * Called at element destruction
	 *
	 * This method MUST be called in the GL Thread
	 *
	 */
	virtual void onFree() = 0;

};
#endif /* GLELEMENT_H_ */

