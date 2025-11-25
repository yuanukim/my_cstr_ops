#include "static_str_v2.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <wctype.h>

static unsigned int pow_integer(unsigned int x, unsigned int y) {
    unsigned int result = 1;
    unsigned int i;

    for (i = 0; i < y; ++i) {
        result *= x;
    }

    return result;
}

static void put_current_date(char* buf, int* len, int limit) {
    char date[32];
    const char* cursor = date;
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);

    if (time_info == NULL) {
        return;
    }
    
    strftime(date, sizeof(date) / sizeof(date[0]), "%Y-%m-%d", time_info);
    while (*cursor) {
        str_concat_char(buf, len, limit, *cursor);
        ++cursor;
    }
}

static void put_current_time(char* buf, int* len, int limit) {
    char now[32];
    const char* cursor = now;
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);

    if (time_info == NULL) {
        return;
    }
    
    strftime(now, sizeof(now) / sizeof(now[0]), "%H:%M:%S", time_info);
    while (*cursor) {
        str_concat_char(buf, len, limit, *cursor);
        ++cursor;
    }
}

void str_concat_char(char* buf, int* len, int limit, char c) {
    if (*len + 1 < limit) {
        buf[*len] = c;
        *len += 1;
        buf[*len] = '\0';
    }
}

void wstr_concat_char(wchar_t* buf, int* len, int limit, wchar_t c) {
    if (*len + 1 < limit) {
        buf[*len] = c;
        *len += 1;
        buf[*len] = L'\0';
    }
}

void str_reverse_segment(char* str, int start, int end) {
    char temp;
    int i, j;

    for (i = start, j = end; i < j; ++i, --j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void str_concat_n(char* buf, int* len, int limit, const char* other) {
    while (*other) {
        str_concat_char(buf, len, limit, *other);
        ++other;
    }
}

void str_front_concat_n(char* buf, int* len, int limit, const char* other) {
    int otherLen = strlen(other);

    if (*len + otherLen + 1 <= limit) {
        memcpy(buf + otherLen, buf, *len * sizeof(char));
        memcpy(buf, other, otherLen * sizeof(char));
        *len += otherLen;
    }
    else {
        int temp = limit - 1 - otherLen;

        if (temp > 0) {
            memcpy(buf + otherLen, buf, temp * sizeof(char));
            memcpy(buf, other, otherLen * sizeof(char));
        }
        else {
            memcpy(buf, other, (limit - 1) * sizeof(char));
        }

        *len = limit - 1;
    }

    buf[*len] = '\0';
}

void wstr_concat_n(wchar_t* buf, int* len, int limit, const wchar_t* other) {
    while (*other) {
        wstr_concat_char(buf, len, limit, *other);
        ++other;
    }
}

void wstr_front_concat_n(wchar_t* buf, int* len, int limit, const wchar_t* other) {
    int otherLen = wcslen(other);

    if (*len + otherLen + 1 <= limit) {
        memcpy(buf + otherLen, buf, *len * sizeof(wchar_t));
        memcpy(buf, other, otherLen * sizeof(wchar_t));
        *len += otherLen;
    }
    else {
        int temp = limit - 1 - otherLen;

        if (temp > 0) {
            memcpy(buf + otherLen, buf, temp * sizeof(wchar_t));
            memcpy(buf, other, otherLen * sizeof(wchar_t));
        }
        else {
            memcpy(buf, other, (limit - 1) * sizeof(wchar_t));
        }

        *len = limit - 1;
    }

    buf[*len] = L'\0';
}

void str_from_integer(char* buf, int* len, int limit, int i) {
    int start = *len;
    int sign = i;

    if (sign < 0) {
        i = -i;
    }

    do {
        str_concat_char(buf, len, limit, i % 10 + '0');
        i /= 10;
    } while(i > 0);

    if (sign < 0) {
        str_concat_char(buf, len, limit, '-');
    }

    str_reverse_segment(buf, start, *len - 1);
}

void str_from_integer_bin(char* buf, int* len, int limit, unsigned int i) {
    int start = *len;

    do {
        str_concat_char(buf, len, limit, i % 2 + '0');
        i /= 2;
    } while(i > 0);

    str_reverse_segment(buf, start, *len - 1);
}

void str_from_integer_hex(char* buf, int* len, int limit, unsigned int i) {
    char c;
    int start = *len;

    do {
        c = i % 16;
        str_concat_char(buf, len, limit, (c <= 9 ? c + '0' : c - 10 + 'a'));
        i /= 16;
    } while(i > 0);

    str_reverse_segment(buf, start, *len - 1);
}

void str_from_integer_oct(char* buf, int* len, int limit, unsigned int i) {
    int start = *len;

    do {
        str_concat_char(buf, len, limit, i % 8 + '0');
        i /= 8;
    } while(i > 0);

    str_reverse_segment(buf, start, *len - 1);
}

void str_from_float(char* buf, int* len, int limit, float n, int precision) {
    unsigned int intPart1, intPart2, factor;
    float temp;

    if (n < 0) {
        str_concat_char(buf, len, limit, '-');
        n = -n;
    }

    intPart1 = (unsigned int)n;
    factor = pow_integer(10, precision);

    temp = (n - intPart1) * factor;
    intPart2 = (unsigned int)temp;
    temp -= intPart2;

    if (temp >= 0.499999f) {
        intPart2 += 1;
    }

    if (intPart2 >= factor) {
        intPart1 += 1;
        intPart2 -= factor;
    }

    str_from_integer(buf, len, limit, intPart1);

    if (precision > 0) {
        str_concat_char(buf, len, limit, '.');
        str_from_integer(buf, len, limit, intPart2);
    }
}

void str_from_float_v2(char* buf, int* len, int limit, float n, int precision) {
    #define FLOAT_ZERO_LIMITATION  0.000001f

    int i = 0;
    float added;

    if (n < FLOAT_ZERO_LIMITATION && n > -FLOAT_ZERO_LIMITATION) {
        str_concat_n(buf, len, limit, "0");
        return;
    }

    if (isinf(n)) {
        if (n > 0) {
            str_concat_n(buf, len, limit, "inf");
            return;
        }
        else {
            str_concat_n(buf, len, limit, "-inf");
            return;
        }
    }

    if (isnan(n)) {
        str_concat_n(buf, len, limit, "nan");
        return;
    }

    added = 0.5f;
    for (i = 0; i < precision; ++i) {
        added /= 10.0;
    }

    if (n < 0) {
        n = -n;
        str_concat_char(buf, len, limit, '-');
    }

    n += added;
    str_from_integer(buf, len, limit, (int)n);

    if (precision > 0) {
        str_concat_char(buf, len, limit, '.');
        n -= (int)n;
        
        for (i = 0; i < precision; ++i) {
            n *= 10;
        }

        str_from_integer(buf, len, limit, (int)n);
    }
}

static void vstr_format_n(char* buf, int* len, int limit, const char* fmt, va_list args) {
    const char* cursor = fmt;
    const char* cstr;
    int tempInt;
    unsigned int tempUInt;
    float tempFloat;
    int precision = 0;
    int hasDigit = 0;

    while (*cursor) {
        while (*cursor != '%' && *cursor != '\0') {
            str_concat_char(buf, len, limit, *cursor);
            ++cursor;
        }

        if (*cursor == '\0') {
            break;
        }
        else {
            ++cursor;

            if (*cursor == '%' || *cursor == 'c') {
                str_concat_char(buf, len, limit, *cursor);
            }
            else if (*cursor == 's') {
                cstr = va_arg(args, const char*);

                while (*cstr) {
                    str_concat_char(buf, len, limit, *cstr);
                    ++cstr;
                }
            }
            else if (*cursor == 'd') {
                tempInt = va_arg(args, int);
                str_from_integer(buf, len, limit, tempInt);
            }
            else if (*cursor == 'x') {
                tempUInt = va_arg(args, unsigned int);
                str_from_integer_hex(buf, len, limit, tempUInt);
            }
            else if (*cursor == 'o') {
                tempUInt = va_arg(args, unsigned int);
                str_from_integer_oct(buf, len, limit, tempUInt);
            }
            else if (*cursor == 'b') {
                tempUInt = va_arg(args, unsigned int);
                str_from_integer_bin(buf, len, limit, tempUInt);
            }
            else if (*cursor == '.') {
                ++cursor;

                precision = 0;
                hasDigit = 0;

                while (isdigit(*cursor)) {
                    hasDigit = 1;
                    precision = precision * 10 + (*cursor - '0');
                    ++cursor;
                }

                if (!hasDigit && *cursor != 'f') {
                    break;
                }

                tempFloat = va_arg(args, double);
                str_from_float_v2(buf, len, limit, tempFloat, precision);
            }
            else if (*cursor == 'D') {
                put_current_date(buf, len, limit);
            }
            else if (*cursor == 'T') {
                put_current_time(buf, len, limit);
            }
            else {
                break;
            }
        }

        ++cursor;
    }
}

void str_format_n(char* buf, int* len, int limit, const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vstr_format_n(buf, len, limit, fmt, args);
    va_end(args);
}

void str_reverse(char* str, int len) {
    char temp;
    int i, j;

    for (i = 0, j = len - 1; i < j; ++i, --j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void wstr_reverse(wchar_t* wstr, int len) {
    wchar_t temp;
    int i, j;

    for (i = 0, j = len - 1; i < j; ++i, --j) {
        temp = wstr[i];
        wstr[i] = wstr[j];
        wstr[j] = temp;
    }
}

void str_read_line(char* buf, int* len, int limit, FILE* f) {
    char c;

    while (*len + 1 < limit) {
        c = fgetc(f);

        if (c == EOF || c == '\n') {
            break;
        }

        buf[*len] = c;
        *len += 1;
    }

    /* ignore the remaining. */
    while (c != EOF && c != '\n') {
        c = fgetc(f);
    }

    buf[*len] = '\0';
}

void str_split(const char* str, int len, const char* pattern, str_split_callback callback, void* param) {
    const char* start = str;
    const char* current = start;
    const char* end = start + len;
    int patternLen = strlen(pattern);
    int segmentLen;

    while (current < end) {
        if (current + patternLen <= end && memcmp(current, pattern, patternLen * sizeof(char)) == 0) {
            segmentLen = current - start;

            if (segmentLen > 0) {
                callback(start, segmentLen, param);
            }

            current += patternLen;
            start = current;
        }
        else {
            ++current;
        }
    }

    if (start < end) {
        callback(start, end - start, param);
    }
}

const char* str_istr(const char* str, const char* pattern) {
    int strLen = strlen(str);
    int patternLen = strlen(pattern);
    int i, j;

    if (strLen < patternLen) {
        return NULL;
    }

    i = 0;
    j = 0;
    while (i < strLen && j < patternLen) {
        if (tolower(str[i]) == tolower(pattern[j])) {
            ++i;
            ++j;
        }
        else {
            i = i - j + 1;
            j = 0;
        }
    }

    if (j == patternLen) {
        return str + (i - j);
    }
    else {
        return NULL;
    }
}

const wchar_t* wstr_istr(const wchar_t* wstr, const wchar_t* pattern) {
    int strLen = wcslen(wstr);
    int patternLen = wcslen(pattern);
    int i, j;

    if (strLen < patternLen) {
        return NULL;
    }

    i = 0;
    j = 0;
    while (i < strLen && j < patternLen) {
        if (towlower(wstr[i]) == towlower(pattern[j])) {
            ++i;
            ++j;
        }
        else {
            i = i - j + 1;
            j = 0;
        }
    }

    if (j == patternLen) {
        return wstr + (i - j);
    }
    else {
        return NULL;
    }
}

void str_visible(char* buf, int* len, int limit, const char* str) {
    while (*str) {
        if (*str == '\r') {
            str_concat_char(buf, len, limit, '\\');
            str_concat_char(buf, len, limit, 'r');
        }
        else if (*str == '\n') {
            str_concat_char(buf, len, limit, '\\');
            str_concat_char(buf, len, limit, 'n');
        }
        else if (*str == '\t') {
            str_concat_char(buf, len, limit, '\\');
            str_concat_char(buf, len, limit, 't');
        }
        else {
            str_concat_char(buf, len, limit, *str);
        }

        ++str;
    }
}

