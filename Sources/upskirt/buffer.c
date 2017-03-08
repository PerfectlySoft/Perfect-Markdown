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

#define BUFFER_MAX_ALLOC_SIZE (1024 * 1024 * 16) //16mb

#include "upskirt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int
sd_bufprefix(const struct sd_buf *buf, const char *prefix)
{
    size_t i;
    assert(buf && buf->unit);

    for (i = 0; i < buf->size; ++i) {
        if (prefix[i] == 0)
            return 0;

        if (buf->data[i] != prefix[i])
            return buf->data[i] - prefix[i];
    }

    return 0;
}

/* sd_bufgrow: increasing the allocated size to the given value */
int
sd_bufgrow(struct sd_buf *buf, size_t neosz)
{
    size_t neoasz;
    void *neodata;

    assert(buf && buf->unit);

    if (neosz > BUFFER_MAX_ALLOC_SIZE)
        return SD_BUF_ENOMEM;

    if (buf->asize >= neosz)
        return SD_BUF_OK;

    neoasz = buf->asize + buf->unit;
    while (neoasz < neosz)
        neoasz += buf->unit;

    neodata = buf->realloc(buf->data, neoasz);
    if (!neodata)
        return SD_BUF_ENOMEM;

    buf->data = neodata;
    buf->asize = neoasz;
    return SD_BUF_OK;
}


/* sd_bufnew: allocation of a new buffer; use the system default heap allocation functions */
struct sd_buf *
sd_bufnew(size_t unit)
{
    return sd_bufnewcb(unit, malloc, realloc, free);
}

/* sd_bufnewcb: allocation of a new buffer; use user-specified heap allocation functions to manage the buffer */
struct sd_buf *
sd_bufnewcb(size_t unit, sd_malloc_cb malloc_cb, sd_realloc_cb realloc_cb, sd_free_cb free_cb)
{
    struct sd_buf *ret;
    ret = malloc_cb(sizeof(*ret));

    if (ret) {
        ret->data = 0;
        ret->size = ret->asize = 0;
        ret->unit = unit;

        ret->realloc = realloc_cb;
        ret->free = free_cb;
    }
    return ret;
}

/* bufnullterm: NULL-termination of the string array */
const char *
sd_bufcstr(struct sd_buf *buf)
{
    assert(buf && buf->unit);

    if (buf->size < buf->asize && buf->data[buf->size] == 0)
        return (char *)buf->data;

    if (buf->size + 1 <= buf->asize || sd_bufgrow(buf, buf->size + 1) == 0) {
        buf->data[buf->size] = 0;
        return (char *)buf->data;
    }

    return NULL;
}

/* sd_bufprintf: formatted printing to a buffer */
void
sd_bufprintf(struct sd_buf *buf, const char *fmt, ...)
{
    va_list ap;
    int n;

    assert(buf && buf->unit);

    if (buf->size >= buf->asize && sd_bufgrow(buf, buf->size + 1) < 0)
        return;

    va_start(ap, fmt);
    n = vsnprintf((char *)buf->data + buf->size, buf->asize - buf->size, fmt, ap);
    va_end(ap);

    if (n < 0) {
#ifdef _MSC_VER
        va_start(ap, fmt);
        n = _vscprintf(fmt, ap);
        va_end(ap);
#else
        return;
#endif
    }

    if ((size_t)n >= buf->asize - buf->size) {
        if (sd_bufgrow(buf, buf->size + n + 1) < 0)
            return;

        va_start(ap, fmt);
        n = vsnprintf((char *)buf->data + buf->size, buf->asize - buf->size, fmt, ap);
        va_end(ap);
    }

    if (n < 0)
        return;

    buf->size += n;
}

/* sd_bufput: appends raw data to a buffer */
void
sd_bufput(struct sd_buf *buf, const void *data, size_t len)
{
    assert(buf && buf->unit);

    if (buf->size + len > buf->asize && sd_bufgrow(buf, buf->size + len) < 0)
        return;

    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
}

/* sd_bufputs: appends a NUL-terminated string to a buffer */
void
sd_bufputs(struct sd_buf *buf, const char *str)
{
    if (!str)
        return;

    sd_bufput(buf, str, strlen(str));
}


/* sd_bufputc: appends a single uint8_t to a buffer */
void
sd_bufputc(struct sd_buf *buf, uint8_t c)
{
    assert(buf && buf->unit);

    if (buf->size + 1 > buf->asize && sd_bufgrow(buf, buf->size + 1) < 0)
        return;

    buf->data[buf->size] = c;
    buf->size += 1;
}

/* sd_bufrelease: decrease the reference count and free the buffer if needed */
void
sd_bufrelease(struct sd_buf *buf)
{
    if (!buf)
        return;

    buf->free(buf->data);
    buf->free(buf);
}


/* sd_bufreset: frees internal data of the buffer */
void
sd_bufreset(struct sd_buf *buf)
{
    if (!buf)
        return;

    buf->free(buf->data);
    buf->data = NULL;
    buf->size = buf->asize = 0;
}

/* sd_bufslurp: removes a given number of bytes from the head of the array */
void
sd_bufslurp(struct sd_buf *buf, size_t len)
{
    assert(buf && buf->unit);

    if (len >= buf->size) {
        buf->size = 0;
        return;
    }

    buf->size -= len;
    memmove(buf->data, buf->data + len, buf->size);
}
