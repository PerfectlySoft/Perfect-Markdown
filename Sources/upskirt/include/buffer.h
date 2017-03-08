/*
 * Copyright (c) 2008, Natacha Porté
 * Copyright (c) 2011, Vicent Martí
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef UPSKIRT_BUFFER_H__
#define UPSKIRT_BUFFER_H__

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#include "upskirt_dll_exports.h"
#if defined(_WIN32)
  #include "upskirt_win32.h"
#endif


#ifdef __cplusplus
namespace upskirt { extern "C" {
#endif

typedef enum {
    SD_BUF_OK = 0,
    SD_BUF_ENOMEM = -1,
} sd_buferror_t;

typedef void *(*sd_malloc_cb)(size_t);
typedef void *(*sd_realloc_cb)(void *, size_t);
typedef void (*sd_free_cb)(void *);

/* struct sd_buf: character array buffer */
struct sd_buf {
    uint8_t *data;      /* actual character data */
    size_t size;    /* size of the string */
    size_t asize;   /* allocated size (0 = volatile buffer) */
    size_t unit;    /* reallocation unit size (0 = read-only buffer) */
    sd_realloc_cb realloc;
    sd_free_cb free;
};

/* SD_BUF_STATIC: global buffer from a string litteral */
#define SD_BUF_STATIC(string) \
    { (uint8_t *)(string), sizeof(string) - 1, sizeof(string), 0, 0 }

/* SD_BUF_VOLATILE: macro for creating a volatile buffer on the stack */
#define SD_BUF_VOLATILE(strname) \
    { (uint8_t *)(strname), strlen(strname), 0, 0, 0 }

/* SD_BUFPUTSL: optimized sd_bufputs of a string litteral */
#define SD_BUFPUTSL(output, literal) \
    sd_bufput(output, literal, sizeof(literal) - 1)

/* sd_bufgrow: increasing the allocated size to the given value */
SDPUBFUN int sd_bufgrow(struct sd_buf *, size_t);

/* sd_bufnew: allocation of a new buffer; use the system default heap allocation functions */
SDPUBFUN struct sd_buf *sd_bufnew(size_t) __attribute__ ((malloc));

/* sd_bufnewcb: allocation of a new buffer; use user-specified heap allocation functions to manage the buffer */
SDPUBFUN struct sd_buf *sd_bufnewcb(size_t, sd_malloc_cb, sd_realloc_cb, sd_free_cb);

/* sd_bufcstr: NUL-termination of the string array (making a C-string) */
SDPUBFUN const char *sd_bufcstr(struct sd_buf *);

/* sd_bufprefix: compare the beginning of a buffer with a string */
SDPUBFUN int sd_bufprefix(const struct sd_buf *buf, const char *prefix);

/* sd_bufput: appends raw data to a buffer */
SDPUBFUN void sd_bufput(struct sd_buf *, const void *, size_t);

/* sd_bufputs: appends a NUL-terminated string to a buffer */
SDPUBFUN void sd_bufputs(struct sd_buf *, const char *);

/* sd_bufputc: appends a single char to a buffer */
SDPUBFUN void sd_bufputc(struct sd_buf *, uint8_t);

/* sd_bufrelease: decrease the reference count and free the buffer if needed */
SDPUBFUN void sd_bufrelease(struct sd_buf *);

/* sd_bufreset: frees internal data of the buffer */
SDPUBFUN void sd_bufreset(struct sd_buf *);

/* sd_bufslurp: removes a given number of bytes from the head of the array */
SDPUBFUN void sd_bufslurp(struct sd_buf *, size_t);

/* sd_bufprintf: formatted printing to a buffer */
SDPUBFUN void sd_bufprintf(struct sd_buf *, const char *, ...) __attribute__ ((format (printf, 2, 3)));

#ifdef __cplusplus
} }
#endif

#endif
