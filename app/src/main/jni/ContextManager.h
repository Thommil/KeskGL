/*
 * ContextManager.h
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include "kesk_native_app_glue.h"
#include "GlContext.h"

#include <hash_map>

using namespace std;

#ifndef CONTEXTMANAGER_H_
#define CONTEXTMANAGER_H_

// If we cause an exception in JNI, we print the exception info to
// the log, we clear the exception to avoid a pending-exception
// crash, and we force the function to return.
#define EXCEPTION_RETURN(env) \
	if (env->ExceptionOccurred()) { \
		env->ExceptionDescribe(); \
		env->ExceptionClear(); \
	}

/**
 * Implementation classes should store and give access to
 * application contexts and threads :
 * 		- OpenGL Context as GPU Server
 * 		- Main UI Thread on CPU as GPU client
 *
 * 	ContextManager is also the bridge between Java and Native code
 * 	and can be used with ContextManager::fromId() when trying to recall
 * 	a native thread from Java code (propagate the id for that).
 */
class ContextManager {
public:

	/**
	 * Message class used to run a process on UI Tread (JAVA)
	 */
	class Runnable{
		public:

		/**
		 * Called using runOnUITread()
		 */
		virtual void run(ContextManager *contextManager) = 0;

		/*
		 * Default destructor
		 */
		virtual ~Runnable(){};

	};

public:
	static hash_map<unsigned int, ContextManager*> CTX_MAP;

private:
	unsigned int id;

public:
	ContextManager();
	virtual ~ContextManager();

	/**
	 * Used to pass data on Java<->Native bridge.
	 *
	 * ContextManager created are stored in a static container
	 * and can be retrieved a static way.
	 */
	static ContextManager* fromId(unsigned int id);

	/**
	 * Gets the GL thread and GPU context
	 *
	 * @return The context in a GlContext instance
	 */
	virtual GlContext* getGLContext() const = 0;

	/**
	 * Gets the main thread and UI application context
	 *
	 * @return The context in a struct android_app instance
	 */
	virtual struct android_app* getUIContext() const = 0;

	/**
	 * Run the specified UIRunnable on UI Thread (Java only)
	 */
	virtual void runOnUITread(Runnable *runnable) = 0;

	/**
	 * Run the specified UIRunnable on GL Thread (Native only)
	 */
	virtual void runOnGLTread(Runnable *runnable) = 0;
};

#endif /* CONTEXTMANAGER_H_ */
