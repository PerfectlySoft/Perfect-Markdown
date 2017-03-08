/*
 * Copyright (c) 2009, Natacha Porté
 * Copyright (c) 2011, Vicent Marti
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

#include "upskirt.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "houdini.h"


#define USE_XHTML(opt) (opt->flags & HTML_USE_XHTML)


int
sdhtml_is_tag(const uint8_t *tag_data, size_t tag_size, const char *tagname)
{
    size_t i;
    int closed = 0;

    if (tag_size < 3 || tag_data[0] != '<')
        return HTML_TAG_NONE;

    i = 1;

    if (tag_data[i] == '/') {
        closed = 1;
        i++;
    }

    for (; i < tag_size; ++i, ++tagname) {
        if (*tagname == 0)
            break;

        if (tag_data[i] != *tagname)
            return HTML_TAG_NONE;
    }

    if (i == tag_size)
        return HTML_TAG_NONE;

    if (isspace(tag_data[i]) || tag_data[i] == '>')
        return closed ? HTML_TAG_CLOSE : HTML_TAG_OPEN;

    return HTML_TAG_NONE;
}

static inline void escape_html(struct sd_buf *ob, const uint8_t *source, size_t length)
{
    houdini_escape_html0(ob, source, length, 0);
}

static inline void escape_href(struct sd_buf *ob, const uint8_t *source, size_t length)
{
    houdini_escape_href(ob, source, length);
}

/********************
 * GENERIC RENDERER *
 ********************/
static int
rndr_autolink(struct sd_buf *ob, const struct sd_buf *link, enum mkd_autolink type, void *opaque)
{
    struct html_renderopt *options = opaque;

    if (!link || !link->size)
        return 0;

    if ((options->flags & HTML_SAFELINK) != 0 &&
        !sd_autolink_issafe(link->data, link->size) &&
        type != MKDA_EMAIL)
        return 0;

    SD_BUFPUTSL(ob, "<a href=\"");
    if (type == MKDA_EMAIL)
        SD_BUFPUTSL(ob, "mailto:");
    escape_href(ob, link->data, link->size);

    if (options->link_attributes) {
        sd_bufputc(ob, '\"');
        options->link_attributes(ob, link, opaque);
        sd_bufputc(ob, '>');
    } else {
        SD_BUFPUTSL(ob, "\">");
    }

    /*
     * Pretty printing: if we get an email address as
     * an actual URI, e.g. `mailto:foo@bar.com`, we don't
     * want to print the `mailto:` prefix
     */
    if (sd_bufprefix(link, "mailto:") == 0) {
        escape_html(ob, link->data + 7, link->size - 7);
    } else {
        escape_html(ob, link->data, link->size);
    }

    SD_BUFPUTSL(ob, "</a>");

    return 1;
}

static void
rndr_blockcode(struct sd_buf *ob, const struct sd_buf *text, const struct sd_buf *lang, void *opaque)
{
    if (ob->size) sd_bufputc(ob, '\n');

    if (lang && lang->size) {
        size_t i, cls;
        SD_BUFPUTSL(ob, "<pre><code class=\"lang-");

        for (i = 0, cls = 0; i < lang->size; ++i, ++cls) {
            while (i < lang->size && isspace(lang->data[i]))
                i++;

            if (i < lang->size) {
                size_t org = i;
                while (i < lang->size && !isspace(lang->data[i]))
                    i++;

                if (lang->data[org] == '.')
                    org++;

                if (cls) sd_bufputc(ob, ' ');
                escape_html(ob, lang->data + org, i - org);
            }
        }

        SD_BUFPUTSL(ob, "\">");
    } else
        SD_BUFPUTSL(ob, "<pre><code>");

    if (text)
        escape_html(ob, text->data, text->size);

    SD_BUFPUTSL(ob, "</code></pre>\n");
}

static void
rndr_blockquote(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (ob->size) sd_bufputc(ob, '\n');
    SD_BUFPUTSL(ob, "<blockquote>\n");
    if (text) sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</blockquote>\n");
}

static int
rndr_codespan(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    SD_BUFPUTSL(ob, "<code>");
    if (text) escape_html(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</code>");
    return 1;
}

static int
rndr_ins(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (!text || !text->size)
        return 0;

    SD_BUFPUTSL(ob, "<ins>");
    sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</ins>");
    return 1;
}

static int
rndr_strikethrough(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (!text || !text->size)
        return 0;

    SD_BUFPUTSL(ob, "<del>");
    sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</del>");
    return 1;
}

static int
rndr_double_emphasis(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (!text || !text->size)
        return 0;

    SD_BUFPUTSL(ob, "<strong>");
    sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</strong>");

    return 1;
}

static int
rndr_emphasis(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (!text || !text->size) return 0;
    SD_BUFPUTSL(ob, "<em>");
    if (text) sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</em>");
    return 1;
}

static int
rndr_linebreak(struct sd_buf *ob, void *opaque)
{
    struct html_renderopt *options = opaque;
    sd_bufputs(ob, USE_XHTML(options) ? "<br/>\n" : "<br>\n");
    return 1;
}

static void
rndr_header(struct sd_buf *ob, const struct sd_buf *text, int level, void *opaque)
{
    struct html_renderopt *options = opaque;

    if (ob->size)
        sd_bufputc(ob, '\n');

    if (options->flags & HTML_OUTLINE)
    {
        if(options->outline_data.current_level >= level)
        {
            SD_BUFPUTSL(ob, "</section>");
            options->outline_data.open_section_count--;
        }
        sd_bufprintf(ob, "<section class=\"section%d\">\n", level);
        options->outline_data.open_section_count++;
        options->outline_data.current_level = level;
    }

    if (options->flags & HTML_TOC)
        sd_bufprintf(ob, "<h%d id=\"toc_%d\">", level, options->toc_data.header_count++);
    else
        sd_bufprintf(ob, "<h%d>", level);

    if (text) sd_bufput(ob, text->data, text->size);
    sd_bufprintf(ob, "</h%d>\n", level);
}

static int
rndr_link(struct sd_buf *ob, const struct sd_buf *link, const struct sd_buf *title, const struct sd_buf *content, void *opaque)
{
    struct html_renderopt *options = opaque;

    if (link != NULL && (options->flags & HTML_SAFELINK) != 0 && !sd_autolink_issafe(link->data, link->size))
        return 0;

    SD_BUFPUTSL(ob, "<a href=\"");

    if (link && link->size)
        escape_href(ob, link->data, link->size);

    if (title && title->size) {
        SD_BUFPUTSL(ob, "\" title=\"");
        escape_html(ob, title->data, title->size);
    }

    if (options->link_attributes) {
        sd_bufputc(ob, '\"');
        options->link_attributes(ob, link, opaque);
        sd_bufputc(ob, '>');
    } else {
        SD_BUFPUTSL(ob, "\">");
    }

    if (content && content->size) sd_bufput(ob, content->data, content->size);
    SD_BUFPUTSL(ob, "</a>");
    return 1;
}

static void
rndr_list(struct sd_buf *ob, const struct sd_buf *text, int flags, void *opaque)
{
    if (ob->size) sd_bufputc(ob, '\n');
	/* TODO: alpha/roman lists */
    sd_bufput(ob, flags & MKD_LIST_ORDERED ? "<ol>\n" : "<ul>\n", 5);
    if (text) sd_bufput(ob, text->data, text->size);
    sd_bufput(ob, flags & MKD_LIST_ORDERED ? "</ol>\n" : "</ul>\n", 6);
}

static void
rndr_listitem(struct sd_buf *ob, const struct sd_buf *text, size_t number, int flags, void *opaque)
{
    if(flags & MKD_LIST_ORDERED && flags & MKD_LIST_FIXED) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "<li value=%lu>", number);
        sd_bufput(ob, buffer, strlen(buffer));
    } else {
        SD_BUFPUTSL(ob, "<li>");
    }

    if (text) {
        size_t size = text->size;
        while (size && text->data[size - 1] == '\n')
            size--;

        sd_bufput(ob, text->data, size);
    }
    SD_BUFPUTSL(ob, "</li>\n");
}

static void
rndr_paragraph(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    struct html_renderopt *options = opaque;
    size_t i = 0;

    if (ob->size) sd_bufputc(ob, '\n');

    if (!text || !text->size)
        return;

    while (i < text->size && isspace(text->data[i])) i++;

    if (i == text->size)
        return;

    SD_BUFPUTSL(ob, "<p>");
    if (options->flags & HTML_HARD_WRAP) {
        size_t org;
        while (i < text->size) {
            org = i;
            while (i < text->size && text->data[i] != '\n')
                i++;

            if (i > org)
                sd_bufput(ob, text->data + org, i - org);

            /*
             * do not insert a line break if this newline
             * is the last character on the paragraph
             */
            if (i >= text->size - 1)
                break;

            rndr_linebreak(ob, opaque);
            i++;
        }
    } else {
        sd_bufput(ob, &text->data[i], text->size - i);
    }
    SD_BUFPUTSL(ob, "</p>\n");
}

static void
rndr_raw_block(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    size_t org, sz;
    if (!text) return;
    sz = text->size;
    while (sz > 0 && text->data[sz - 1] == '\n') sz--;
    org = 0;
    while (org < sz && text->data[org] == '\n') org++;
    if (org >= sz) return;
    if (ob->size) sd_bufputc(ob, '\n');
    sd_bufput(ob, text->data + org, sz - org);
    sd_bufputc(ob, '\n');
}

static int
rndr_triple_emphasis(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (!text || !text->size) return 0;
    SD_BUFPUTSL(ob, "<strong><em>");
    sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</em></strong>");
    return 1;
}

static void
rndr_hrule(struct sd_buf *ob, void *opaque)
{
    struct html_renderopt *options = opaque;
    if (ob->size) sd_bufputc(ob, '\n');
    sd_bufputs(ob, USE_XHTML(options) ? "<hr/>\n" : "<hr>\n");
}

static int
rndr_image(struct sd_buf *ob, const struct sd_buf *link, const struct sd_buf *title, const struct sd_buf *alt, void *opaque)
{
    struct html_renderopt *options = opaque;
    if (!link || !link->size) return 0;

    SD_BUFPUTSL(ob, "<img src=\"");
    escape_href(ob, link->data, link->size);
    SD_BUFPUTSL(ob, "\" alt=\"");

    if (alt && alt->size)
        escape_html(ob, alt->data, alt->size);

    if (title && title->size) {
        SD_BUFPUTSL(ob, "\" title=\"");
        escape_html(ob, title->data, title->size); }

    sd_bufputs(ob, USE_XHTML(options) ? "\"/>" : "\">");
    return 1;
}

static int
rndr_raw_html(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    struct html_renderopt *options = opaque;

    /* HTML_ESCAPE overrides SKIP_HTML, SKIP_STYLE, SKIP_LINKS and SKIP_IMAGES
    * It doens't see if there are any valid tags, just escape all of them. */
    if((options->flags & HTML_ESCAPE) != 0) {
        escape_html(ob, text->data, text->size);
        return 1;
    }

    if ((options->flags & HTML_SKIP_HTML) != 0)
        return 1;

    if ((options->flags & HTML_SKIP_STYLE) != 0 &&
        sdhtml_is_tag(text->data, text->size, "style"))
        return 1;

    if ((options->flags & HTML_SKIP_LINKS) != 0 &&
        sdhtml_is_tag(text->data, text->size, "a"))
        return 1;

    if ((options->flags & HTML_SKIP_IMAGES) != 0 &&
        sdhtml_is_tag(text->data, text->size, "img"))
        return 1;

    sd_bufput(ob, text->data, text->size);
    return 1;
}

static void
rndr_table(struct sd_buf *ob, const struct sd_buf *header, const struct sd_buf *body, void *opaque)
{
    if (ob->size) sd_bufputc(ob, '\n');
    SD_BUFPUTSL(ob, "<table><thead>\n");
    if (header)
        sd_bufput(ob, header->data, header->size);
    SD_BUFPUTSL(ob, "</thead><tbody>\n");
    if (body)
        sd_bufput(ob, body->data, body->size);
    SD_BUFPUTSL(ob, "</tbody></table>\n");
}

static void
rndr_tablerow(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    SD_BUFPUTSL(ob, "<tr>\n");
    if (text)
        sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</tr>\n");
}

static void
rndr_tablecell(struct sd_buf *ob, const struct sd_buf *text, int flags, void *opaque)
{
    if (flags & MKD_TABLE_HEADER) {
        SD_BUFPUTSL(ob, "<th");
    } else {
        SD_BUFPUTSL(ob, "<td");
    }

    switch (flags & MKD_TABLE_ALIGNMASK) {
    case MKD_TABLE_ALIGN_CENTER:
		SD_BUFPUTSL(ob, " style=\"text-align: center\">");
        break;

    case MKD_TABLE_ALIGN_L:
		SD_BUFPUTSL(ob, " style=\"text-align: left\">");
        break;

    case MKD_TABLE_ALIGN_R:
		SD_BUFPUTSL(ob, " style=\"text-align: right\">");
        break;

    default:
        SD_BUFPUTSL(ob, ">");
    }

    if (text)
        sd_bufput(ob, text->data, text->size);

    if (flags & MKD_TABLE_HEADER) {
        SD_BUFPUTSL(ob, "</th>\n");
    } else {
        SD_BUFPUTSL(ob, "</td>\n");
    }
}

static int
rndr_superscript(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (!text || !text->size) return 0;
    SD_BUFPUTSL(ob, "<sup>");
    sd_bufput(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</sup>");
    return 1;
}

static void
rndr_normal_text(struct sd_buf *ob, const struct sd_buf *text, void *opaque)
{
    if (text)
        escape_html(ob, text->data, text->size);
}

static void
rndr_finalize(struct sd_buf *ob, void *opaque)
{
    struct html_renderopt *options = opaque;
    int i;

    if (options->flags & HTML_OUTLINE) {
        for(i = 0; i < options->outline_data.open_section_count; i++)
        {
            SD_BUFPUTSL(ob, "\n</section>\n");
        }
    }
}


static void
toc_header(struct sd_buf *ob, const struct sd_buf *text, int level, void *opaque)
{
    struct html_renderopt *options = opaque;

    /* set the level offset if this is the first header
     * we're parsing for the document */
    if (options->toc_data.current_level == 0) {
        options->toc_data.level_offset = level - 1;
    }
    level -= options->toc_data.level_offset;

    if (level > options->toc_data.current_level) {
        while (level > options->toc_data.current_level) {
            SD_BUFPUTSL(ob, "<ul>\n<li>\n");
            options->toc_data.current_level++;
        }
    } else if (level < options->toc_data.current_level) {
        SD_BUFPUTSL(ob, "</li>\n");
        while (level < options->toc_data.current_level) {
            SD_BUFPUTSL(ob, "</ul>\n</li>\n");
            options->toc_data.current_level--;
        }
        SD_BUFPUTSL(ob,"<li>\n");
    } else {
        SD_BUFPUTSL(ob,"</li>\n<li>\n");
    }

    sd_bufprintf(ob, "<a href=\"#toc_%d\">", options->toc_data.header_count++);
    if (text)
        escape_html(ob, text->data, text->size);
    SD_BUFPUTSL(ob, "</a>\n");
}

static int
toc_link(struct sd_buf *ob, const struct sd_buf *link, const struct sd_buf *title, const struct sd_buf *content, void *opaque)
{
    if (content && content->size)
        sd_bufput(ob, content->data, content->size);
    return 1;
}

static void
toc_finalize(struct sd_buf *ob, void *opaque)
{
    struct html_renderopt *options = opaque;

    while (options->toc_data.current_level > 0) {
        SD_BUFPUTSL(ob, "</li>\n</ul>\n");
        options->toc_data.current_level--;
    }
}

void
sdhtml_toc_renderer(struct sd_callbacks *callbacks, struct html_renderopt *options)
{
    static const struct sd_callbacks cb_default = {
        NULL,
        NULL,
        NULL,
        toc_header,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,

        NULL,
        rndr_codespan,
        rndr_double_emphasis,
        rndr_emphasis,
        NULL,
        NULL,
        toc_link,
        NULL,
        rndr_triple_emphasis,
        rndr_ins,
        rndr_strikethrough,
        rndr_superscript,

        NULL,
        NULL,

        NULL,
        toc_finalize,

        NULL,
    };

    memset(options, 0x0, sizeof(struct html_renderopt));
    options->flags = HTML_TOC;

    memcpy(callbacks, &cb_default, sizeof(struct sd_callbacks));
}

void
sdhtml_renderer(struct sd_callbacks *callbacks, struct html_renderopt *options, unsigned int render_flags)
{
    static const struct sd_callbacks cb_default = {
        rndr_blockcode,
        rndr_blockquote,
        rndr_raw_block,
        rndr_header,
        rndr_hrule,
        rndr_list,
        rndr_listitem,
        rndr_paragraph,
        rndr_table,
        rndr_tablerow,
        rndr_tablecell,

        rndr_autolink,
        rndr_codespan,
        rndr_double_emphasis,
        rndr_emphasis,
        rndr_image,
        rndr_linebreak,
        rndr_link,
        rndr_raw_html,
        rndr_triple_emphasis,
        rndr_ins,
        rndr_strikethrough,
        rndr_superscript,

        NULL,
        rndr_normal_text,

        NULL,
        NULL,

        NULL, //rndr_finalize,
    };

    /* Prepare the options pointer */
    memset(options, 0x0, sizeof(struct html_renderopt));
    options->flags = render_flags;

    /* Prepare the callbacks */
    memcpy(callbacks, &cb_default, sizeof(struct sd_callbacks));

    if (render_flags & HTML_OUTLINE)
    {
        callbacks->outline = rndr_finalize;

        options->outline_data.open_section_count = 0;
        options->outline_data.current_level = 0;
    }

    if (render_flags & HTML_SKIP_IMAGES)
        callbacks->image = NULL;

    if (render_flags & HTML_SKIP_LINKS) {
        callbacks->link = NULL;
        callbacks->autolink = NULL;
    }

    if (render_flags & HTML_SKIP_HTML || render_flags & HTML_ESCAPE)
        callbacks->blockhtml = NULL;
}
