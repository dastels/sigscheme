/*===========================================================================
 *  FileName : alloc.c
 *  About    : memory allocators
 *
 *  Copyright (C) 2005-2006 Kazuki Ohta <mover AT hct.zaq.ne.jp>
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of authors nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 *  IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
===========================================================================*/

/*=======================================
  System Include
=======================================*/
#include <stdint.h> /* FIXME: make C99-independent */
#include <stdlib.h>
#include <assert.h>

/*=======================================
  Local Include
=======================================*/
#include "sigscheme.h"
#include "sigschemeinternal.h"

/*=======================================
  File Local Macro Definitions
=======================================*/

/*=======================================
  File Local Type Definitions
=======================================*/

/*=======================================
  Variable Declarations
=======================================*/

/*=======================================
  File Local Function Declarations
=======================================*/

/*=======================================
  Function Implementations
=======================================*/
/* FIXME: ensure safety in a portable way */
void *
scm_malloc_aligned(size_t size)
{
    void *p;

#if HAVE_POSIX_MEMALIGN
    /*
     * Cited from manpage of posix_memalign(3) of glibc:
     *
     * CONFORMING TO
     *     The function valloc() appeared in 3.0 BSD. It is documented as being
     *     obsolete in BSD 4.3, and as legacy in SUSv2. It no longer occurs in
     *     SUSv3.  The function memalign() appears in SunOS 4.1.3 but not in
     *     BSD 4.4.  The function posix_memalign() comes from POSIX 1003.1d.
     */
    /* FIXME: replace the '16' with sizeof(ScmCell) if not required */
    posix_memalign(&p, 16, size);
    SCM_ENSURE_ALLOCATED(p);
#if SCM_DEBUG
    /* check for buggy allocator */
    assert(!((uintptr_t)p % 16));
#endif
#else
    p = scm_malloc(size);
    /* heaps must be aligned to sizeof(ScmCell) */
    assert(!((uintptr_t)p % sizeof(ScmCell)));
#endif

    return p;
}

void *
scm_malloc(size_t size)
{
    void *p;

    p = malloc(size);
    ENSURE_ALLOCATED(p);

    return p;
}

void *
scm_calloc(size_t number, size_t size)
{
    void *p;

    p = calloc(number, size);
    ENSURE_ALLOCATED(p);

    return p;
}

void *
scm_realloc(void *ptr, size_t size)
{
    void *p;

    p = realloc(ptr, size);
    ENSURE_ALLOCATED(p);

    return p;
}

/*=======================================
   Extendable Local Buffer
=======================================*/
void
scm_lbuf_init(struct ScmLBuf_void_ *lbuf, void *init_buf, size_t init_size)
{
    lbuf->buf  = lbuf->init_buf  = init_buf;
    lbuf->size = lbuf->init_size = init_size;
    lbuf->extended_cnt = 0;
}

void
scm_lbuf_free(struct ScmLBuf_void_ *lbuf)
{
    if (lbuf->buf != lbuf->init_buf)
        free(lbuf->buf);
}

void
scm_lbuf_alloc(struct ScmLBuf_void_ *lbuf, size_t size)
{
    lbuf->buf = scm_malloc(size);
    lbuf->size = size;
}

void
scm_lbuf_realloc(struct ScmLBuf_void_ *lbuf, size_t size)
{
    if (lbuf->buf == lbuf->init_buf) {
        if (size < lbuf->size)
            lbuf->size = size;
        lbuf->buf = memcpy(scm_malloc(size), lbuf->buf, lbuf->size);
    } else {
        lbuf->buf = scm_realloc(lbuf->buf, size);
    }
    lbuf->size = size;
}

void
scm_lbuf_extend(struct ScmLBuf_void_ *lbuf,
                size_t (*f)(struct ScmLBuf_void_ *), size_t least_size)
{
    size_t new_size;

    if (lbuf->size < least_size) {
        new_size = (*f)(lbuf);
        if (new_size < lbuf->size)
            ERR("local buffer exceeded");
        if (new_size < least_size)
            new_size = least_size;
        scm_lbuf_realloc(lbuf, new_size);
        lbuf->extended_cnt++;
    }
}

size_t
scm_lbuf_f_linear(struct ScmLBuf_void_ *lbuf)
{
    return (lbuf->size + lbuf->init_size);
}

size_t
scm_lbuf_f_exponential(struct ScmLBuf_void_ *lbuf)
{
    return (lbuf->size << 1);
}