#ifndef __STATIC_STR_V2_H__
#define __STATIC_STR_V2_H__

#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

/* concatenate a character. */
void str_concat_char(char* buf, int* len, int limit, char c);
void wstr_concat_char(wchar_t* buf, int* len, int limit, wchar_t c);

/*
    concatenate a string at tail.
*/
void str_concat_n(char* buf, int* len, int limit, const char* other);
void wstr_concat_n(wchar_t* buf, int* len, int limit, const wchar_t* other);

/*
    concatenate a string to the front.
*/
void str_front_concat_n(char* buf, int* len, int limit, const char* other);
void wstr_front_concat_n(wchar_t* buf, int* len, int limit, const wchar_t* other);

/*
    convert a integer to a string.
*/
void str_from_integer(char* buf, int* len, int limit, int i);

/*
    convert a binary integer to a string.
*/
void str_from_integer_bin(char* buf, int* len, int limit, unsigned int i);

/*
    convert a hexadecimal integer to a string.
*/
void str_from_integer_hex(char* buf, int* len, int limit, unsigned int i);

/*
    convert a octal integer to a string.
*/
void str_from_integer_oct(char* buf, int* len, int limit, unsigned int i);

/*
    convert a float number with the given precision to a string.
*/
void str_from_float(char* buf, int* len, int limit, float n, int precision);

/*
    just like snprintf. not thread safe.

    %d     integer.
    %s     string.
    %c     character.
    %.xf   float number, x precision.
    %x     hex.
    %o     oct.
    %b     binary.
    %D     current year-month-day.
    %T     current hour:minute:second.
*/
void str_format_n(char* buf, int* len, int limit, const char* fmt, ...);

/* reverse a string. */
void str_reverse_segment(char* str, int start, int end);   /* index begins from start to the end, includes end. */
void str_reverse(char* str, int len);
void wstr_reverse(wchar_t* wstr, int len);

/*
    get line from the f.
*/
void str_read_line(char* buf, int* len, int limit, FILE* f);

/*
    string split.
*/
typedef void (*str_split_callback)(const char* segment, int len, void* param);
void str_split(const char* str, int len, const char* pattern, str_split_callback callback, void* param);

/*
    string find, but ignore case.
*/
const char* str_istr(const char* str, const char* pattern);

/*
    wide string find, but ignore case.
*/
const wchar_t* wstr_istr(const wchar_t* wstr, const wchar_t* pattern);

void str_visible(char* buf, int* len, int limit, const char* str);

#endif