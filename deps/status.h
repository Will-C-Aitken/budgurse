/* Copyright (c) 2004-2023 calcurse Development Team <misc@calcurse.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the
 *        following disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the
 *        following disclaimer in the documentation and/or other
 *        materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// the following three macros are adapted from the src code for calcurse,
// specifically `calcurse/src/calcurse.h`
#define ERROR_MSG(...) do {                                                  \
    char msg[BUFSIZ];                                                        \
    int len;                                                                 \
    len = snprintf(msg, BUFSIZ, "%s: %d: ", __FILE__, __LINE__);             \
    snprintf(msg + len, BUFSIZ - len, __VA_ARGS__);                          \
    fprintf(stderr, "%s\n", msg);                                            \
} while (0)

#define EXIT(...) do {							     \
    ERROR_MSG(__VA_ARGS__);                                                  \
    if (curses_mode)							     \
	end_budgurse(EXIT_FAILURE);                                          \
    else								     \
	exit(EXIT_FAILURE);						     \
} while (0)

#define EXIT_IF(cond, ...) do {                                              \
    if ((cond))                                                              \
	EXIT(__VA_ARGS__);                                                   \
} while (0)

