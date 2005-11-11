/*===========================================================================
 *  FileName : main.c
 *  About    : main function
 *
 *  Copyright (C) 2005      by Kazuki Ohta (mover@hct.zaq.ne.jp)
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
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
===========================================================================*/
/*=======================================
  System Include
=======================================*/
#include <stdlib.h>

/*=======================================
  Local Include
=======================================*/
#include "sigscheme.h"
#include "sigschemeinternal.h"

/*=======================================
  File Local Macro Declarations
=======================================*/
#define PROMPT_STR "sscm> "

#if SCM_COMPAT_SIOD
#define FEATURE_ID_SIOD "siod"
#endif

/*=======================================
  File Local Struct Declarations
=======================================*/

/*=======================================
  Variable Declarations
=======================================*/
static ScmObj feature_id_siod;

/*=======================================
  File Local Function Declarations
=======================================*/
static void repl(void);
static void repl_loop(void);
static int  is_repl_prompt(void);

/*=======================================
  Function Implementations
=======================================*/
static void repl(void)
{
#if !SCM_GCC4_READY_GC
    ScmObj stack_start = NULL;
#endif

#if !SCM_GCC4_READY_GC
    SigScm_GC_ProtectStack(&stack_start);
#endif

    feature_id_siod = Scm_NewStringCopying(FEATURE_ID_SIOD);

    repl_loop();

#if !SCM_GCC4_READY_GC
    SigScm_GC_UnprotectStack(&stack_start);
#endif
}

static void repl_loop(void)
{
    ScmObj s_exp  = SCM_FALSE;
    ScmObj result = SCM_FALSE;
    int is_prompt = is_repl_prompt();

    if (is_prompt)
        SigScm_PortPrintf(scm_current_output_port, PROMPT_STR);

    while (s_exp = SigScm_Read(scm_current_input_port), !EOFP(s_exp)) {
#if SCM_USE_SRFI34
        /*
         * Error Aware repl_loop
         *
         * (guard (err (else #f))
         *   (eval exp '()))
         */
        result = ScmExp_SRFI34_guard(LIST_2(Scm_Intern("err"),
                                            LIST_2(Scm_Intern("else"), SCM_UNDEF)),
                                     LIST_1(LIST_3(Scm_Intern("eval"),
                                                   LIST_2(SYM_QUOTE, s_exp),
                                                   SCM_INTERACTION_ENV)),
                                     SCM_INTERACTION_ENV);
#else /* SCM_USE_SRFI34 */
        result = EVAL(s_exp, SCM_INTERACTION_ENV);
#endif

#if SCM_USE_SRFI38
        SigScm_WriteToPortWithSharedStructure(scm_current_output_port, result);
#else
        SigScm_WriteToPort(scm_current_output_port, result);
#endif
        SigScm_PortNewline(scm_current_output_port);

        if (is_prompt)
            SigScm_PortPrintf(scm_current_output_port, PROMPT_STR);
    }
}

static int is_repl_prompt(void)
{
#if SCM_COMPAT_SIOD
    return (FALSEP(ScmOp_providedp(feature_id_siod))
            || SigScm_GetVerboseLevel() >= 2);
#else
    return TRUE;
#endif
}

int main(int argc, char **argv)
{
    char *filename = argv[1];

    SigScm_Initialize();

    if (argc < 2) {
#if SCM_GCC4_READY_GC
        SCM_GC_PROTECTED_CALL_VOID(repl, ());
#else
        repl();
#endif
        /*        SigScm_Error("usage : sscm <filename>"); */
    } else {
        SigScm_load(filename);
    }

    SigScm_Finalize();
    return EXIT_SUCCESS;
}
