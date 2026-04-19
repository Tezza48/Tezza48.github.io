#pragma once
#include "sb.h"

#define TAG(name, attribs)                                                    \
    for (                                                                     \
        int _once = (sb_appendf(STRINGBUILDER, "<%s %s>", name, attribs), 1); \
        _once;                                                                \
        (sb_appendf(STRINGBUILDER, "</%s>", name), _once = 0))

#define TEXT(name, innerText, attribs) sb_appendf(STRINGBUILDER, "<%s %s>%s</%s>", name, attribs, innerText, name)
#define ESC(name, attribs) sb_appendf(STRINGBUILDER, "<%s %s />", name, attribs)

#define html() TAG("html", "")
#define head() TAG("head", "")
#define body() TAG("body", "")

#define header(attribs) TAG("header", attribs)
#define main_el(attribs) TAG("main", attribs)
#define footer(attribs) TAG("footer", attribs)
#define section(attribs) TAG("section", attribs)

#define h1(innerText, attribs) TEXT("h1", innerText, attribs)
#define h2(innerText, attribs) TEXT("h2", innerText, attribs)
#define h3(innerText, attribs) TEXT("h3", innerText, attribs)
#define h4(innerText, attribs) TEXT("h4", innerText, attribs)
#define h5(innerText, attribs) TEXT("h5", innerText, attribs)
#define p(innerText, attribs) TEXT("p", innerText, attribs)
