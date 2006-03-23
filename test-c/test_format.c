/*===========================================================================
 *  FileName : test_format.c
 *  About    : unit test for format functions
 *
 *  Copyright (C) 2006 YamaKen <yamaken AT bp.iij4u.or.jp>
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

#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include "cutter-sscm.h"

#include "my-stdint.h"
#include "sigscheme.h"
#include "sigschemeinternal.h"

#define I32 (SIZEOF_INT    == SIZEOF_INT32_T)
#define L32 (SIZEOF_LONG   == SIZEOF_INT32_T)
#define P32 (SIZEOF_VOID_P == SIZEOF_INT32_T)

#define I64 (SIZEOF_INT    == SIZEOF_INT64_T)
#define L64 (SIZEOF_LONG   == SIZEOF_INT64_T)
#define P64 (SIZEOF_VOID_P == SIZEOF_INT64_T)

#define IMAX32 (SIZEOF_INTMAX_T == SIZEOF_INT32_T)
#define IMAX64 (SIZEOF_INTMAX_T == SIZEOF_INT64_T)

#define PDIFF32 (SIZEOF_PTRDIFF_T == SIZEOF_INT32_T)
#define PDIFF64 (SIZEOF_PTRDIFF_T == SIZEOF_INT64_T)

#define STR SCM_STRING_STR

static ScmObj
format(const char *fmt, ...)
{
    va_list args;
    ScmObj ret;

    va_start(args, fmt);
    ret = scm_vformat(SCM_FALSE, SCM_FMT_INTERNAL, fmt, args);
    va_end(args);

    return ret;
}

UT_DEF2(test_1, "no directives")
{
    UT_ASSERT_EQUAL_STRING("",   STR(format("")));
    UT_ASSERT_EQUAL_STRING("\"", STR(format("\"")));
    UT_ASSERT_EQUAL_STRING("\\", STR(format("\\")));
    UT_ASSERT_EQUAL_STRING("a",  STR(format("a")));
    UT_ASSERT_EQUAL_STRING("aBc",     STR(format("aBc")));
    UT_ASSERT_EQUAL_STRING("あ",      STR(format("あ")));
    UT_ASSERT_EQUAL_STRING("あい",    STR(format("あい")));
    UT_ASSERT_EQUAL_STRING("aあBいc", STR(format("aあBいc")));
}

UT_DEF2(test_2, "~C")
{
    UT_ASSERT_EQUAL_STRING("",   STR(format("~C", (scm_ichar_t)'\0')));
    UT_ASSERT_EQUAL_STRING("a",  STR(format("~C", (scm_ichar_t)'a')));
    UT_ASSERT_EQUAL_STRING("\"", STR(format("~C", (scm_ichar_t)'\"')));
    UT_ASSERT_EQUAL_STRING("\\", STR(format("~C", (scm_ichar_t)'\\')));
    UT_ASSERT_EQUAL_STRING("あ", STR(format("~C", (scm_ichar_t)0x3042)));
}

UT_DEF2(test_3, "~S")
{
    UT_ASSERT_EQUAL_STRING("",          STR(format("~S",   "")));
    UT_ASSERT_EQUAL_STRING("",          STR(format("~0S",  "")));
    UT_ASSERT_EQUAL_STRING(" ",         STR(format("~1S",  "")));
    UT_ASSERT_EQUAL_STRING("",          STR(format("~00S", "")));
    UT_ASSERT_EQUAL_STRING(" ",         STR(format("~01S", "")));
    UT_ASSERT_EQUAL_STRING("\"",        STR(format("~S",   "\"")));
    UT_ASSERT_EQUAL_STRING("\\",        STR(format("~S",   "\\")));
    UT_ASSERT_EQUAL_STRING("a",         STR(format("~S",   "a")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~S",   "aBc")));
    UT_ASSERT_EQUAL_STRING("あ",        STR(format("~S",   "あ")));
    UT_ASSERT_EQUAL_STRING("あい",      STR(format("~S",   "あい")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~S",   "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~0S",  "aBc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~1S",  "aBc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~2S",  "aBc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~3S",  "aBc")));
    UT_ASSERT_EQUAL_STRING(" aBc",      STR(format("~4S",  "aBc")));
    UT_ASSERT_EQUAL_STRING("  aBc",     STR(format("~5S",  "aBc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~00S", "aBc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~01S", "aBc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~02S", "aBc")));
    UT_ASSERT_EQUAL_STRING("aBc",       STR(format("~03S", "aBc")));
    UT_ASSERT_EQUAL_STRING(" aBc",      STR(format("~04S", "aBc")));
    UT_ASSERT_EQUAL_STRING("  aBc",     STR(format("~05S", "aBc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~0S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~1S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~2S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~3S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~4S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~5S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING(" aあBいc",  STR(format("~6S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING("  aあBいc", STR(format("~7S",  "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~00S", "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~01S", "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~02S", "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~03S", "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~04S", "aあBいc")));
    UT_ASSERT_EQUAL_STRING("aあBいc",   STR(format("~05S", "aあBいc")));
    UT_ASSERT_EQUAL_STRING(" aあBいc",  STR(format("~06S", "aあBいc")));
    UT_ASSERT_EQUAL_STRING("  aあBいc", STR(format("~07S", "aあBいc")));
}

UT_DEF2(test_4, "~P")
{
#if P32
    UT_ASSERT_EQUAL_STRING("0x00000000",
                           STR(format("~P", (void *)NULL)));
    UT_ASSERT_EQUAL_STRING("0xffffffff",
                           STR(format("~P", (void *)~0UL)));
    UT_ASSERT_EQUAL_STRING("0x00012abc",
                           STR(format("~P", (void *)0x12ABC)));
#elif P64
    UT_ASSERT_EQUAL_STRING("0x0000000000000000",
                           STR(format("~P", (void *)NULL)));
    UT_ASSERT_EQUAL_STRING("0xffffffffffffffff",
                           STR(format("~P", (void *)~0UL)));
    UT_ASSERT_EQUAL_STRING("0x0000000000012abc",
                           STR(format("~P", (void *)0x12ABC)));
#endif
}

UT_DEF2(test_5, "~D")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~D",   -100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~D",   -10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~D",   -1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~D",   0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~D",   1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~D",   10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~D",   100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0D",  -100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0D",  -10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0D",  -1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0D",  0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0D",  1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0D",  10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0D",  100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03D", -100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03D", -10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03D", -1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03D", 0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03D", 1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03D", 10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03D", 100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3D",  -100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3D",  -10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3D",  -1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3D",  0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3D",  1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3D",  10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3D",  100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127D", 123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127D", 123)));
}

UT_DEF2(test_6, "~U")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~U",   0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~U",   1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~U",   10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~U",   100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0U",  0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0U",  1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0U",  10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0U",  100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03U", 0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03U", 1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03U", 10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03U", 100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3U",  0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3U",  1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3U",  10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3U",  100)));

#if I32
    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~U",   -100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~U",   -10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~U",   -1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~0U",  -100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~0U",  -10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~0U",  -1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~03U", -100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~03U", -10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~03U", -1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~3U",  -100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~3U",  -10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~3U",  -1)));
#elif I64
    UT_ASSERT_EQUAL_STRING("18446744073709551516", STR(format("~U",   -100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606", STR(format("~U",   -10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615", STR(format("~U",   -1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516", STR(format("~0U",  -100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606", STR(format("~0U",  -10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615", STR(format("~0U",  -1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516", STR(format("~03U", -100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606", STR(format("~03U", -10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615", STR(format("~03U", -1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516", STR(format("~3U",  -100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606", STR(format("~3U",  -10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615", STR(format("~3U",  -1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127U", 123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127U", 123)));
}

UT_DEF2(test_7, "~X")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~X",   0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~X",   1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~X",   10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~X",   100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0X",  0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0X",  1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0X",  10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0X",  100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03X", 0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03X", 1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03X", 10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03X", 100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3X",  0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3X",  1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3X",  10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3X",  100)));

#if I32
    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~X",   -100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~X",   -10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~X",   -1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~0X",  -100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~0X",  -10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~0X",  -1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~03X", -100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~03X", -10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~03X", -1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~3X",  -100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~3X",  -10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~3X",  -1)));
#elif I64
    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c", STR(format("~X",   -100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6", STR(format("~X",   -10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff", STR(format("~X",   -1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c", STR(format("~0X",  -100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6", STR(format("~0X",  -10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff", STR(format("~0X",  -1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c", STR(format("~03X", -100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6", STR(format("~03X", -10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff", STR(format("~03X", -1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c", STR(format("~3X",  -100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6", STR(format("~3X",  -10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff", STR(format("~3X",  -1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127X", 0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127X", 0x1ac)));
}

UT_DEF2(test_8, "~O")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~O",   0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~O",   1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~O",   10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~O",   100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0O",  0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0O",  1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0O",  10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0O",  100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03O", 0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03O", 1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03O", 10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03O", 100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3O",  0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3O",  1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3O",  10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3O",  100)));

#if I32
    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~O",   -100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~O",   -10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~O",   -1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~0O",  -100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~0O",  -10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~0O",  -1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~03O", -100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~03O", -10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~03O", -1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~3O",  -100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~3O",  -10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~3O",  -1)));
#elif I64
    UT_ASSERT_EQUAL_STRING("1777777777777777777634", STR(format("~O",  -100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766", STR(format("~O",  -10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777", STR(format("~O",  -1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634", STR(format("~0O", -100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766", STR(format("~0O", -10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777", STR(format("~0O", -1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634", STR(format("~03O", -100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766", STR(format("~03O", -10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777", STR(format("~03O", -1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634", STR(format("~3O", -100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766", STR(format("~3O", -10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777", STR(format("~3O", -1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127O", 0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127O", 0123)));
}

UT_DEF2(test_9, "~B")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~B",   0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~B",   1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~B",   10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~B",   100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0B",  0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0B",  1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0B",  10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0B",  100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05B", 0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05B", 1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05B", 10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05B", 100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5B",  0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5B",  1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5B",  10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5B",  100)));

#if I32
    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~B", -100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~B", -10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~B", -1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~0B", -100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~0B", -10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~0B", -1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~05B", -100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~05B", -10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~05B", -1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~5B", -100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~5B", -10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~5B", -1)));
#elif I64
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~B", -100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~B", -10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~B", -1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~0B", -100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~0B", -10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~0B", -1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~05B", -100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~05B", -10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~05B", -1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~5B", -100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~5B", -10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~5B", -1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127B", 0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127B", 0x5)));
}

UT_DEF2(test_10, "~WD")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~WD",   (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~WD",   (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~WD",   (int32_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~WD",   (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~WD",   (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~WD",   (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~WD",   (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0WD",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0WD",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0WD",  (int32_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0WD",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0WD",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0WD",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0WD",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03WD", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03WD", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03WD", (int32_t)-1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03WD", (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03WD", (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03WD", (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03WD", (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3WD",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3WD",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3WD",  (int32_t)-1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3WD",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3WD",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3WD",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3WD",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127WD", (int32_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127WD", (int32_t)123)));
}

UT_DEF2(test_11, "~WU")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~WU",   (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~WU",   (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~WU",   (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~WU",   (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0WU",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0WU",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0WU",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0WU",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03WU", (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03WU", (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03WU", (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03WU", (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3WU",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3WU",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3WU",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3WU",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~WU",   (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~WU",   (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~WU",   (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~0WU",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~0WU",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~0WU",  (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~03WU", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~03WU", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~03WU", (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~3WU",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~3WU",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~3WU",  (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127WU", (int32_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127WU", (int32_t)123)));
}

UT_DEF2(test_12, "~WX")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~WX",   (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~WX",   (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~WX",   (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~WX",   (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0WX",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0WX",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0WX",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0WX",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03WX", (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03WX", (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03WX", (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03WX", (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3WX",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3WX",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3WX",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3WX",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~WX",   (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~WX",   (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~WX",   (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~0WX",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~0WX",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~0WX",  (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~03WX", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~03WX", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~03WX", (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~3WX",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~3WX",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~3WX",  (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127WX", (int32_t)0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127WX", (int32_t)0x1ac)));
}

UT_DEF2(test_13, "~WO")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~WO",   (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~WO",   (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~WO",   (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~WO",   (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0WO",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0WO",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0WO",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0WO",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03WO", (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03WO", (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03WO", (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03WO", (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3WO",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3WO",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3WO",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3WO",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~WO",   (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~WO",   (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~WO",   (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~0WO",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~0WO",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~0WO",  (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~03WO", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~03WO", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~03WO", (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~3WO",  (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~3WO",  (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~3WO",  (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127WO", (int32_t)0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127WO", (int32_t)0123)));
}

UT_DEF2(test_14, "~WB")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~WB",   (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~WB",   (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~WB",   (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~WB",   (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0WB",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0WB",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0WB",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0WB",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05WB", (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05WB", (int32_t)1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05WB", (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05WB", (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5WB",  (int32_t)0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5WB",  (int32_t)1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5WB",  (int32_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5WB",  (int32_t)100)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~WB", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~WB", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~WB", (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~0WB", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~0WB", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~0WB", (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~05WB", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~05WB", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~05WB", (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~5WB", (int32_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~5WB", (int32_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~5WB", (int32_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127WB", (int32_t)0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127WB", (int32_t)0x5)));
}

UT_DEF2(test_15, "~MD")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~MD",   (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~MD",   (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~MD",   (scm_int_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~MD",   (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~MD",   (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~MD",   (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~MD",   (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0MD",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0MD",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0MD",  (scm_int_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0MD",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0MD",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0MD",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0MD",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03MD", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03MD", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03MD", (scm_int_t)-1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03MD", (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03MD", (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03MD", (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03MD", (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3MD",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3MD",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3MD",  (scm_int_t)-1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3MD",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3MD",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3MD",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3MD",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127MD", (scm_int_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127MD", (scm_int_t)123)));
}

UT_DEF2(test_16, "~MU")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~MU",   (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~MU",   (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~MU",   (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~MU",   (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0MU",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0MU",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0MU",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0MU",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03MU", (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03MU", (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03MU", (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03MU", (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3MU",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3MU",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3MU",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3MU",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~MU",   (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~MU",   (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~MU",   (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~0MU",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~0MU",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~0MU",  (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~03MU", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~03MU", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~03MU", (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~3MU",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~3MU",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~3MU",  (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127MU", (scm_int_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127MU", (scm_int_t)123)));
}

UT_DEF2(test_17, "~MX")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~MX",   (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~MX",   (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~MX",   (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~MX",   (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0MX",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0MX",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0MX",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0MX",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03MX", (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03MX", (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03MX", (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03MX", (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3MX",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3MX",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3MX",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3MX",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~MX",   (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~MX",   (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~MX",   (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~0MX",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~0MX",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~0MX",  (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~03MX", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~03MX", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~03MX", (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~3MX",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~3MX",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~3MX",  (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127MX", (scm_int_t)0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127MX", (scm_int_t)0x1ac)));
}

UT_DEF2(test_18, "~MO")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~MO",   (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~MO",   (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~MO",   (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~MO",   (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0MO",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0MO",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0MO",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0MO",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03MO", (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03MO", (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03MO", (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03MO", (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3MO",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3MO",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3MO",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3MO",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~MO",   (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~MO",   (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~MO",   (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~0MO",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~0MO",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~0MO",  (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~03MO", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~03MO", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~03MO", (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~3MO",  (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~3MO",  (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~3MO",  (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127MO", (scm_int_t)0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127MO", (scm_int_t)0123)));
}

UT_DEF2(test_19, "~MB")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~MB",   (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~MB",   (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~MB",   (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~MB",   (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0MB",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0MB",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0MB",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0MB",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05MB", (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05MB", (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05MB", (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05MB", (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5MB",  (scm_int_t)0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5MB",  (scm_int_t)1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5MB",  (scm_int_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5MB",  (scm_int_t)100)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~MB", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~MB", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~MB", (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~0MB", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~0MB", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~0MB", (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~05MB", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~05MB", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~05MB", (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~5MB", (scm_int_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~5MB", (scm_int_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~5MB", (scm_int_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127MB", (scm_int_t)0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127MB", (scm_int_t)0x5)));
}

UT_DEF2(test_20, "~QD")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~QD",   (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~QD",   (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~QD",   (int64_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~QD",   (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~QD",   (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~QD",   (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~QD",   (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0QD",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0QD",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0QD",  (int64_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0QD",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0QD",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0QD",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0QD",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03QD", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03QD", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03QD", (int64_t)-1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03QD", (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03QD", (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03QD", (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03QD", (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3QD",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3QD",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3QD",  (int64_t)-1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3QD",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3QD",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3QD",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3QD",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127QD", (int64_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127QD", (int64_t)123)));
}

UT_DEF2(test_21, "~QU")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~QU",   (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~QU",   (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~QU",   (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~QU",   (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0QU",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0QU",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0QU",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0QU",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03QU", (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03QU", (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03QU", (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03QU", (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3QU",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3QU",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3QU",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3QU",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~QU",   (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~QU",   (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~QU",   (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~0QU",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~0QU",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~0QU",  (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~03QU", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~03QU", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~03QU", (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~3QU",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~3QU",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~3QU",  (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127QU", (int64_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127QU", (int64_t)123)));
}

UT_DEF2(test_22, "~QX")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~QX",   (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~QX",   (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~QX",   (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~QX",   (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0QX",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0QX",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0QX",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0QX",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03QX", (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03QX", (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03QX", (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03QX", (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3QX",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3QX",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3QX",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3QX",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~QX",   (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~QX",   (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~QX",   (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~0QX",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~0QX",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~0QX",  (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~03QX", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~03QX", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~03QX", (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~3QX",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~3QX",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~3QX",  (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127QX", (int64_t)0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127QX", (int64_t)0x1ac)));
}

UT_DEF2(test_23, "~QO")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~QO",   (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~QO",   (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~QO",   (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~QO",   (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0QO",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0QO",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0QO",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0QO",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03QO", (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03QO", (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03QO", (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03QO", (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3QO",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3QO",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3QO",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3QO",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~QO",   (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~QO",   (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~QO",   (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~0QO",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~0QO",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~0QO",  (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~03QO", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~03QO", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~03QO", (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~3QO",  (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~3QO",  (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~3QO",  (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127QO", (int64_t)0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127QO", (int64_t)0123)));
}

UT_DEF2(test_24, "~QB")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~QB",   (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~QB",   (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~QB",   (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~QB",   (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0QB",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0QB",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0QB",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0QB",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05QB", (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05QB", (int64_t)1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05QB", (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05QB", (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5QB",  (int64_t)0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5QB",  (int64_t)1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5QB",  (int64_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5QB",  (int64_t)100)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~QB", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~QB", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~QB", (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~0QB", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~0QB", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~0QB", (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~05QB", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~05QB", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~05QB", (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~5QB", (int64_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~5QB", (int64_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~5QB", (int64_t)-1)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127QB", (int64_t)0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127QB", (int64_t)0x5)));
}

UT_DEF2(test_25, "~LD")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~LD",   (long)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~LD",   (long)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~LD",   (long)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~LD",   (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~LD",   (long)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~LD",   (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~LD",   (long)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0LD",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0LD",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0LD",  (long)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0LD",  (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0LD",  (long)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0LD",  (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0LD",  (long)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03LD", (long)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03LD", (long)-10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03LD", (long)-1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03LD", (long)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03LD", (long)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03LD", (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03LD", (long)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3LD",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3LD",  (long)-10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3LD",  (long)-1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3LD",  (long)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3LD",  (long)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3LD",  (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3LD",  (long)100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127LD", (long)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127LD", (long)123)));
}

UT_DEF2(test_26, "~LU")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~LU",   (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~LU",   (long)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~LU",   (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~LU",   (long)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0LU",  (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0LU",  (long)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0LU",  (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0LU",  (long)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03LU", (long)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03LU", (long)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03LU", (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03LU", (long)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3LU",  (long)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3LU",  (long)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3LU",  (long)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3LU",  (long)100)));

#if L32
    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~LU",   (long)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~LU",   (long)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~LU",   (long)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~0LU",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~0LU",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~0LU",  (long)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~03LU", (long)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~03LU", (long)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~03LU", (long)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~3LU",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~3LU",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~3LU",  (long)-1)));
#elif L64
    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~LU",   (long)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~LU",   (long)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~LU",   (long)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~0LU",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~0LU",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~0LU",  (long)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~03LU", (long)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~03LU", (long)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~03LU", (long)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~3LU",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~3LU",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~3LU",  (long)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127LU", (long)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127LU", (long)123)));
}

UT_DEF2(test_27, "~LX")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~LX",   (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~LX",   (long)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~LX",   (long)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~LX",   (long)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0LX",  (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0LX",  (long)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0LX",  (long)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0LX",  (long)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03LX", (long)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03LX", (long)1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03LX", (long)10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03LX", (long)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3LX",  (long)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3LX",  (long)1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3LX",  (long)10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3LX",  (long)100)));

#if L32
    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~LX",   (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~LX",   (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~LX",   (long)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~0LX",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~0LX",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~0LX",  (long)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~03LX", (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~03LX", (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~03LX", (long)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~3LX",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~3LX",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~3LX",  (long)-1)));
#elif L64
    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~LX",   (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~LX",   (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~LX",   (long)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~0LX",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~0LX",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~0LX",  (long)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~03LX", (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~03LX", (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~03LX", (long)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~3LX",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~3LX",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~3LX",  (long)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127LX", (long)0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127LX", (long)0x1ac)));
}

UT_DEF2(test_28, "~LO")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~LO",   (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~LO",   (long)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~LO",   (long)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~LO",   (long)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0LO",  (long)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0LO",  (long)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0LO",  (long)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0LO",  (long)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03LO", (long)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03LO", (long)1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03LO", (long)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03LO", (long)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3LO",  (long)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3LO",  (long)1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3LO",  (long)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3LO",  (long)100)));

#if L32
    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~LO",   (long)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~LO",   (long)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~LO",   (long)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~0LO",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~0LO",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~0LO",  (long)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~03LO", (long)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~03LO", (long)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~03LO", (long)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~3LO",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~3LO",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~3LO",  (long)-1)));
#elif L64
    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~LO",   (long)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~LO",   (long)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~LO",   (long)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~0LO",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~0LO",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~0LO",  (long)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~03LO", (long)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~03LO", (long)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~03LO", (long)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~3LO",  (long)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~3LO",  (long)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~3LO",  (long)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127LO", (long)0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127LO", (long)0123)));
}

UT_DEF2(test_29, "~LB")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~LB",   (long)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~LB",   (long)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~LB",   (long)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~LB",   (long)100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0LB",  (long)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0LB",  (long)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0LB",  (long)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0LB",  (long)100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05LB", (long)0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05LB", (long)1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05LB", (long)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05LB", (long)100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5LB",  (long)0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5LB",  (long)1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5LB",  (long)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5LB",  (long)100)));

#if L32
    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~LB", (long)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~0LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~0LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~0LB", (long)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~05LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~05LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~05LB", (long)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~5LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~5LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~5LB", (long)-1)));
#elif L64
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~LB", (long)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~0LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~0LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~0LB", (long)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~05LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~05LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~05LB", (long)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~5LB", (long)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~5LB", (long)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~5LB", (long)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127LB", (long)0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127LB", (long)0x5)));
}

UT_DEF2(test_30, "~JD")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~JD",   (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~JD",   (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~JD",   (intmax_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~JD",   (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~JD",   (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~JD",   (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~JD",   (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0JD",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0JD",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0JD",  (intmax_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0JD",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0JD",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0JD",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0JD",  (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03JD", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03JD", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03JD", (intmax_t)-1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03JD", (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03JD", (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03JD", (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03JD", (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3JD",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3JD",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3JD",  (intmax_t)-1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3JD",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3JD",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3JD",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3JD",  (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127JD", (intmax_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127JD", (intmax_t)123)));
}

UT_DEF2(test_31, "~JU")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~JU",   (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~JU",   (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~JU",   (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~JU",   (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0JU",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0JU",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0JU",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0JU",  (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03JU", (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03JU", (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03JU", (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03JU", (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3JU",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3JU",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3JU",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3JU",  (intmax_t)100)));

#if IMAX32
    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~JU",   (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~JU",   (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~JU",   (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~0JU",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~0JU",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~0JU",  (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~03JU", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~03JU", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~03JU", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~3JU",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~3JU",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~3JU",  (intmax_t)-1)));
#elif IMAX64
    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~JU",   (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~JU",   (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~JU",   (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~0JU",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~0JU",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~0JU",  (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~03JU", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~03JU", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~03JU", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~3JU",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~3JU",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~3JU",  (intmax_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127JU", (intmax_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127JU", (intmax_t)123)));
}

UT_DEF2(test_32, "~JX")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~JX",   (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~JX",   (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~JX",   (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~JX",   (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0JX",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0JX",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0JX",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0JX",  (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03JX", (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03JX", (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03JX", (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03JX", (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3JX",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3JX",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3JX",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3JX",  (intmax_t)100)));

#if IMAX32
    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~JX",   (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~JX",   (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~JX",   (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~0JX",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~0JX",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~0JX",  (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~03JX", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~03JX", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~03JX", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~3JX",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~3JX",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~3JX",  (intmax_t)-1)));
#elif IMAX64
    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~JX",   (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~JX",   (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~JX",   (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~0JX",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~0JX",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~0JX",  (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~03JX", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~03JX", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~03JX", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~3JX",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~3JX",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~3JX",  (intmax_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127JX", (intmax_t)0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127JX", (intmax_t)0x1ac)));
}

UT_DEF2(test_33, "~JO")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~JO",   (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~JO",   (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~JO",   (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~JO",   (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0JO",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0JO",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0JO",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0JO",  (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03JO", (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03JO", (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03JO", (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03JO", (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3JO",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3JO",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3JO",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3JO",  (intmax_t)100)));

#if IMAX32
    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~JO",   (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~JO",   (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~JO",   (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~0JO",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~0JO",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~0JO",  (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~03JO", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~03JO", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~03JO", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~3JO",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~3JO",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~3JO",  (intmax_t)-1)));
#elif IMAX64
    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~JO",   (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~JO",   (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~JO",   (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~0JO",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~0JO",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~0JO",  (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~03JO", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~03JO", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~03JO", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~3JO",  (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~3JO",  (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~3JO",  (intmax_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127JO", (intmax_t)0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127JO", (intmax_t)0123)));
}

UT_DEF2(test_34, "~JB")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~JB",   (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~JB",   (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~JB",   (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~JB",   (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0JB",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0JB",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0JB",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0JB",  (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05JB", (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05JB", (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05JB", (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05JB", (intmax_t)100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5JB",  (intmax_t)0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5JB",  (intmax_t)1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5JB",  (intmax_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5JB",  (intmax_t)100)));

#if IMAX32
    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~JB", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~0JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~0JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~0JB", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~05JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~05JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~05JB", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~5JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~5JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~5JB", (intmax_t)-1)));
#elif IMAX64
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~JB", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~0JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~0JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~0JB", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~05JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~05JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~05JB", (intmax_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~5JB", (intmax_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~5JB", (intmax_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~5JB", (intmax_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127JB", (intmax_t)0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127JB", (intmax_t)0x5)));
}

UT_DEF2(test_35, "~TD")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~TD",   (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~TD",   (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~TD",   (ptrdiff_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~TD",   (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~TD",   (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~TD",   (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~TD",   (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0TD",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0TD",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0TD",  (ptrdiff_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0TD",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0TD",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0TD",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0TD",  (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03TD", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03TD", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03TD", (ptrdiff_t)-1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03TD", (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03TD", (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03TD", (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03TD", (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3TD",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3TD",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3TD",  (ptrdiff_t)-1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3TD",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3TD",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3TD",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3TD",  (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127TD", (ptrdiff_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127TD", (ptrdiff_t)123)));
}

UT_DEF2(test_36, "~TU")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~TU",   (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~TU",   (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~TU",   (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~TU",   (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0TU",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0TU",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0TU",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0TU",  (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03TU", (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03TU", (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03TU", (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03TU", (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3TU",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3TU",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3TU",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3TU",  (ptrdiff_t)100)));

#if PDIFF32
    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~TU",   (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~TU",   (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~TU",   (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~0TU",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~0TU",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~0TU",  (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~03TU", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~03TU", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~03TU", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~3TU",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~3TU",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~3TU",  (ptrdiff_t)-1)));
#elif PDIFF64
    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~TU",   (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~TU",   (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~TU",   (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~0TU",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~0TU",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~0TU",  (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~03TU", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~03TU", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~03TU", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~3TU",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~3TU",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~3TU",  (ptrdiff_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127TU", (ptrdiff_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127TU", (ptrdiff_t)123)));
}

UT_DEF2(test_37, "~TX")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~TX",   (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~TX",   (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~TX",   (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~TX",   (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0TX",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0TX",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0TX",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0TX",  (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03TX", (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03TX", (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03TX", (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03TX", (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3TX",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3TX",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3TX",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3TX",  (ptrdiff_t)100)));

#if PDIFF32
    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~TX",   (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~TX",   (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~TX",   (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~0TX",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~0TX",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~0TX",  (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~03TX", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~03TX", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~03TX", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~3TX",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~3TX",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~3TX",  (ptrdiff_t)-1)));
#elif PDIFF64
    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~TX",   (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~TX",   (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~TX",   (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~0TX",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~0TX",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~0TX",  (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~03TX", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~03TX", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~03TX", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~3TX",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~3TX",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~3TX",  (ptrdiff_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127TX", (ptrdiff_t)0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127TX", (ptrdiff_t)0x1ac)));
}

UT_DEF2(test_38, "~TO")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~TO",   (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~TO",   (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~TO",   (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~TO",   (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0TO",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0TO",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0TO",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0TO",  (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03TO", (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03TO", (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03TO", (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03TO", (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3TO",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3TO",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3TO",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3TO",  (ptrdiff_t)100)));

#if PDIFF32
    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~TO",   (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~TO",   (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~TO",   (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~0TO",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~0TO",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~0TO",  (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~03TO", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~03TO", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~03TO", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~3TO",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~3TO",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~3TO",  (ptrdiff_t)-1)));
#elif PDIFF64
    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~TO",   (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~TO",   (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~TO",   (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~0TO",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~0TO",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~0TO",  (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~03TO", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~03TO", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~03TO", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~3TO",  (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~3TO",  (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~3TO",  (ptrdiff_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127TO", (ptrdiff_t)0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127TO", (ptrdiff_t)0123)));
}

UT_DEF2(test_39, "~TB")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~TB",   (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~TB",   (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~TB",   (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~TB",   (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0TB",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0TB",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0TB",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0TB",  (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05TB", (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05TB", (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05TB", (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05TB", (ptrdiff_t)100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5TB",  (ptrdiff_t)0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5TB",  (ptrdiff_t)1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5TB",  (ptrdiff_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5TB",  (ptrdiff_t)100)));

#if PDIFF32
    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~TB", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~0TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~0TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~0TB", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~05TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~05TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~05TB", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~5TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~5TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~5TB", (ptrdiff_t)-1)));
#elif PDIFF64
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~TB", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~0TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~0TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~0TB", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~05TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~05TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~05TB", (ptrdiff_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~5TB", (ptrdiff_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~5TB", (ptrdiff_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~5TB", (ptrdiff_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127TB", (ptrdiff_t)0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127TB", (ptrdiff_t)0x5)));
}

UT_DEF2(test_40, "~ZD")
{
    UT_ASSERT_EQUAL_STRING("-100", STR(format("~ZD",   (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~ZD",   (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~ZD",   (size_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~ZD",   (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~ZD",   (size_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~ZD",   (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~ZD",   (size_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~0ZD",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~0ZD",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("-1",   STR(format("~0ZD",  (size_t)-1)));
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0ZD",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0ZD",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0ZD",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0ZD",  (size_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~03ZD", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~03ZD", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("-01",  STR(format("~03ZD", (size_t)-1)));
    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03ZD", (size_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03ZD", (size_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03ZD", (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03ZD", (size_t)100)));

    UT_ASSERT_EQUAL_STRING("-100", STR(format("~3ZD",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("-10",  STR(format("~3ZD",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING(" -1",  STR(format("~3ZD",  (size_t)-1)));
    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3ZD",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3ZD",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3ZD",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3ZD",  (size_t)100)));

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127ZD", (size_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127ZD", (size_t)123)));
}

UT_DEF2(test_41, "~ZU")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~ZU",   (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~ZU",   (size_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~ZU",   (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~ZU",   (size_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0ZU",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0ZU",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING("10",   STR(format("~0ZU",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~0ZU",  (size_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03ZU", (size_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03ZU", (size_t)1)));
    UT_ASSERT_EQUAL_STRING("010",  STR(format("~03ZU", (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~03ZU", (size_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3ZU",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3ZU",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING(" 10",  STR(format("~3ZU",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("100",  STR(format("~3ZU",  (size_t)100)));

#if P32
    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~ZU",   (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~ZU",   (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~ZU",   (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~0ZU",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~0ZU",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~0ZU",  (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~03ZU", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~03ZU", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~03ZU", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("4294967196", STR(format("~3ZU",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("4294967286", STR(format("~3ZU",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("4294967295", STR(format("~3ZU",  (size_t)-1)));
#elif P64
    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~ZU",   (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~ZU",   (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~ZU",   (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~0ZU",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~0ZU",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~0ZU",  (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~03ZU", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~03ZU", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~03ZU", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("18446744073709551516",
                           STR(format("~3ZU",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("18446744073709551606",
                           STR(format("~3ZU",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("18446744073709551615",
                           STR(format("~3ZU",  (size_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127ZU", (size_t)123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127ZU", (size_t)123)));
}

UT_DEF2(test_42, "~ZX")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~ZX",   (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~ZX",   (size_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~ZX",   (size_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~ZX",   (size_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0ZX",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0ZX",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING("a",    STR(format("~0ZX",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("64",   STR(format("~0ZX",  (size_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03ZX", (size_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03ZX", (size_t)1)));
    UT_ASSERT_EQUAL_STRING("00a",  STR(format("~03ZX", (size_t)10)));
    UT_ASSERT_EQUAL_STRING("064",  STR(format("~03ZX", (size_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3ZX",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3ZX",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING("  a",  STR(format("~3ZX",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING(" 64",  STR(format("~3ZX",  (size_t)100)));

#if P32
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~ZX",   ~(size_t)0)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~ZX",   (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~ZX",   (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~ZX",   (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~0ZX",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~0ZX",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~0ZX",  (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~03ZX", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~03ZX", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~03ZX", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffff9c", STR(format("~3ZX",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffff6", STR(format("~3ZX",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffff", STR(format("~3ZX",  (size_t)-1)));
#elif P64
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~ZX",   ~(size_t)0)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~ZX",   (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~ZX",   (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~ZX",   (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~0ZX",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~0ZX",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~0ZX",  (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~03ZX", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~03ZX", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~03ZX", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("ffffffffffffff9c",
                           STR(format("~3ZX",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("fffffffffffffff6",
                           STR(format("~3ZX",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("ffffffffffffffff",
                           STR(format("~3ZX",  (size_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            1ac",
                           STR(format("~127ZX", (size_t)0x1ac)));
    UT_ASSERT_EQUAL_STRING("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001ac",
                           STR(format("~0127ZX", (size_t)0x1ac)));
}

UT_DEF2(test_43, "~ZO")
{
    UT_ASSERT_EQUAL_STRING("0",    STR(format("~ZO",   (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~ZO",   (size_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~ZO",   (size_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~ZO",   (size_t)100)));

    UT_ASSERT_EQUAL_STRING("0",    STR(format("~0ZO",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",    STR(format("~0ZO",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING("12",   STR(format("~0ZO",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~0ZO",  (size_t)100)));

    UT_ASSERT_EQUAL_STRING("000",  STR(format("~03ZO", (size_t)0)));
    UT_ASSERT_EQUAL_STRING("001",  STR(format("~03ZO", (size_t)1)));
    UT_ASSERT_EQUAL_STRING("012",  STR(format("~03ZO", (size_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~03ZO", (size_t)100)));

    UT_ASSERT_EQUAL_STRING("  0",  STR(format("~3ZO",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("  1",  STR(format("~3ZO",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING(" 12",  STR(format("~3ZO",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("144",  STR(format("~3ZO",  (size_t)100)));

#if P32
    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~ZO",   (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~ZO",   (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~ZO",   (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~0ZO",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~0ZO",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~0ZO",  (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~03ZO", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~03ZO", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~03ZO", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("37777777634", STR(format("~3ZO",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("37777777766", STR(format("~3ZO",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("37777777777", STR(format("~3ZO",  (size_t)-1)));
#elif P64
    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~ZO",   (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~ZO",   (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~ZO",   (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~0ZO",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~0ZO",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~0ZO",  (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~03ZO", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~03ZO", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~03ZO", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("1777777777777777777634",
                           STR(format("~3ZO",  (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777766",
                           STR(format("~3ZO",  (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1777777777777777777777",
                           STR(format("~3ZO",  (size_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            123",
                           STR(format("~127ZO", (size_t)0123)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000123",
                           STR(format("~0127ZO", (size_t)0123)));
}

UT_DEF2(test_44, "~ZB")
{
    UT_ASSERT_EQUAL_STRING("0",       STR(format("~ZB",   (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~ZB",   (size_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~ZB",   (size_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~ZB",   (size_t)100)));

    UT_ASSERT_EQUAL_STRING("0",       STR(format("~0ZB",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("1",       STR(format("~0ZB",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING("1010",    STR(format("~0ZB",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~0ZB",  (size_t)100)));

    UT_ASSERT_EQUAL_STRING("00000",   STR(format("~05ZB", (size_t)0)));
    UT_ASSERT_EQUAL_STRING("00001",   STR(format("~05ZB", (size_t)1)));
    UT_ASSERT_EQUAL_STRING("01010",   STR(format("~05ZB", (size_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~05ZB", (size_t)100)));

    UT_ASSERT_EQUAL_STRING("    0",   STR(format("~5ZB",  (size_t)0)));
    UT_ASSERT_EQUAL_STRING("    1",   STR(format("~5ZB",  (size_t)1)));
    UT_ASSERT_EQUAL_STRING(" 1010",   STR(format("~5ZB",  (size_t)10)));
    UT_ASSERT_EQUAL_STRING("1100100", STR(format("~5ZB",  (size_t)100)));

#if P32
    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~ZB", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~0ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~0ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~0ZB", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~05ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~05ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~05ZB", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("11111111111111111111111110011100",
                           STR(format("~5ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111110110",
                           STR(format("~5ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("11111111111111111111111111111111",
                           STR(format("~5ZB", (size_t)-1)));
#elif P64
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~ZB", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~0ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~0ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~0ZB", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~05ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~05ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~05ZB", (size_t)-1)));

    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111110011100",
                           STR(format("~5ZB", (size_t)-100)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111110110",
                           STR(format("~5ZB", (size_t)-10)));
    UT_ASSERT_EQUAL_STRING("1111111111111111111111111111111111111111111111111111111111111111",
                           STR(format("~5ZB", (size_t)-1)));
#endif

    UT_ASSERT_EQUAL_STRING("                                                                                                                            101",
                           STR(format("~127ZB", (size_t)0x5)));
    UT_ASSERT_EQUAL_STRING("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101",
                           STR(format("~0127ZB", (size_t)0x5)));
}

UT_REGISTER_BEGIN("format")
UT_REGISTER(test_1, "no directives")
UT_REGISTER(test_2, "~C")
UT_REGISTER(test_3, "~S")
UT_REGISTER(test_4, "~P")
UT_REGISTER(test_5, "~D")
UT_REGISTER(test_6, "~U")
UT_REGISTER(test_7, "~X")
UT_REGISTER(test_8, "~O")
UT_REGISTER(test_9, "~B")
UT_REGISTER(test_10, "~WD")
UT_REGISTER(test_11, "~WU")
UT_REGISTER(test_12, "~WX")
UT_REGISTER(test_13, "~WO")
UT_REGISTER(test_14, "~WB")
UT_REGISTER(test_15, "~MD")
UT_REGISTER(test_16, "~MU")
UT_REGISTER(test_17, "~MX")
UT_REGISTER(test_18, "~MO")
UT_REGISTER(test_19, "~MB")
UT_REGISTER(test_20, "~QD")
UT_REGISTER(test_21, "~QU")
UT_REGISTER(test_22, "~QX")
UT_REGISTER(test_23, "~QO")
UT_REGISTER(test_24, "~QB")
UT_REGISTER(test_25, "~LD")
UT_REGISTER(test_26, "~LU")
UT_REGISTER(test_27, "~LX")
UT_REGISTER(test_28, "~LO")
UT_REGISTER(test_29, "~LB")
UT_REGISTER(test_30, "~JD")
UT_REGISTER(test_31, "~JU")
UT_REGISTER(test_32, "~JX")
UT_REGISTER(test_33, "~JO")
UT_REGISTER(test_34, "~JB")
UT_REGISTER(test_35, "~TD")
UT_REGISTER(test_36, "~TU")
UT_REGISTER(test_37, "~TX")
UT_REGISTER(test_38, "~TO")
UT_REGISTER(test_39, "~TB")
UT_REGISTER(test_40, "~ZD")
UT_REGISTER(test_41, "~ZU")
UT_REGISTER(test_42, "~ZX")
UT_REGISTER(test_43, "~ZO")
UT_REGISTER(test_44, "~ZB")
UT_REGISTER_END