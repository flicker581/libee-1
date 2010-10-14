/**
 * @file ctx.h
 * @brief The library context object.
 * @class ee_ctx ctx.h
 *
 * The library context object is at the top of libee's class
 * hierarchy. It serves as the "environement" (acutally context ;))
 * in which all other ee library objects operate.
 *//*
 *
 * Libee - a CEE-based event expression library.
 * Copyright 2010 by Rainer Gerhards and Adiscon GmbH.
 *
 * This file is part of libee.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * A copy of the LGPL v2.1 can be found in the file "COPYING" in this distribution.
 */
#ifndef LIBEE_EE_H_INCLUDED
#define	LIBEE_EE_H_INCLUDED
//#include <stdlib.h>	/* we need size_t */

#define ObjID_None 0xFDFD0001
#define ObjID_CTX 0xFDFD0001

struct ee_ctx_s {
	unsigned objID;	/**< a magic number to prevent some memory adressing errors */
	void (*dbgCB)(void *cookie, char *msg, size_t lenMsg);
		/**< user-provided debug output callback */
	void *dbgCookie; /**< cookie to be passed to debug callback */
};


/**
 * The library context descriptor.
 * This is used to permit multiple independednt instances of the
 * library to be called within a single program. This is most 
 * useful for plugin-based architectures.
 * @note
 * For the current library version, multi-threaded applications
 * must use \b different library contexts for concurrent access.
 */
typedef struct ee_ctx_s* ee_ctx;

/**
 * Return library version string.
 *
 * Returns the version of the currently used library.
 *
 * @return Zero-Terminated library version string.
 */
/* Note: this MUST NOT be inline to make sure the actual library
 * has the right version, not just what was used to compile!
 */
char *ee_version(void);

/**
 * Initialize a library context.
 *
 * To prevent memory leaks, ee_exitCtx() must be called on a library
 * context that is no longer needed.
 *
 * @memberof ee_ctx
 * @public
 *
 * @return new library context or NULL if an error occured
 */
ee_ctx ee_initCtx(void);

/**
 * Discard a library context.
 *
 * Free's the ressources associated with the given library context. It
 * MUST NOT be accessed after calling this function.
 *
 * @memberof ee_ctx
 * @public
 *
 * @param ctx The context to be discarded.
 *
 * @return Returns zero on success, something else otherwise.
 */
int ee_exitCtx(ee_ctx ctx);

/**
 * Set a debug message handler (callback).
 *
 * Libee can provide helpful information for debugging
 * - it's internal processing
 * - the way a log message is being normalized
 *
 * It does so by emiting "interesting" information about its processing
 * at various stages. A caller can obtain this information by registering
 * an entry point. When done so, ctx will call the entry point
 * whenever it has something to emit. Note that debugging can be rather
 * verbose.
 *
 * The callback will be called with the following three parameters in that order:
 * - the caller-provided cookie
 * - a zero-terminated string buffer
 * - the length of the string buffer, without the trailing NUL byte
 *
 * @note
 * The provided callback function <b>must not</b> call any ctx
 * APIs except when specifically flagged as safe for calling by a debug
 * callback handler.
 *
 * @memberof ee_ctx
 * @private
 *
 * @param[in] ctx The library context to apply callback to.
 * @param[in] cb The function to be called for debugging
 * @param[in] cookie Opaque cookie to be passed down to debug handler. Can be
 *                   used for some state tracking by the caller. This is defined as
 *                   void* to support pointers. To play it safe, a pointer should be
 *                   passed (but advantorous folks may also use an unsigned).
 *
 * @return Returns zero on success, something else otherwise.
 */
int ee_setDebugCB(ee_ctx ctx, void (*cb)(void*, char*, size_t), void *cookie);

/* internal functions */
void ee_dbgprintf(ee_ctx ctx, char *fmt, ...) __attribute__((format(printf, 2, 3)));

#endif /* #ifndef LIBEE_EE_H_INCLUDED */
