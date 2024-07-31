#ifndef SV_H_
#define SV_H_

#include <string.h>
#include <stdbool.h>

typedef struct {
    const char *data;
    size_t count;
} StringView;

#define SV_NULL sv_from_parts(NULL, 0)

// printf macros for String_View
#define SV_Fmt "%.*s"
#define SV_Arg(sv) (int) (sv).count, (sv).data
#define SV_Debug_Fmt "[count:%i] -> '%.*s'"
#define SV_Debug_Arg(sv) (int) (sv).count, (int) (sv).count, (sv).data
// USAGE:
//   StringView name = ...;
//   printf("Name: " SV_Fmt "\n", SV_Arg(name));
//   printf("Name: " SV_Debug_Fmt "\n", SV_Debug_Arg(name));

// Init functions
// -----------------------------------------------------------------------------
StringView sv_from_parts(const char *data, size_t count);
StringView sv_from_cstr(const char *cstr);

// Whitespace Trimming functions
// -----------------------------------------------------------------------------
StringView sv_trim_left(StringView sv);
StringView sv_trim_right(StringView sv);
StringView sv_trim(StringView sv);

// Chopping functions
// -----------------------------------------------------------------------------

/*******************************************************
 Chops the given StringView until the delimiter, assigning the chopped off
 chunk to out_chopped_chunk.

 If the delimiter does not exist then the function returns false and the given
 StringView will be unchanged, and out_chopped_chunk will be empty.
*******************************************************/
bool sv_chop_until_delim(StringView *sv, char delim, StringView *out_chopped_chunk);

// Comparison function
// -----------------------------------------------------------------------------
bool sv_eq(StringView a, StringView b);

#endif // SV_H_


#ifdef SV_IMPLEMENTATION

StringView sv_from_parts(const char *data, size_t count)
{
    return (StringView) {
        .data = data,
        .count = count
    };
}

StringView sv_from_cstr(const char *cstr)
{
    return sv_from_parts(cstr, strlen(cstr));
}

StringView sv_trim_left(StringView sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[i])) {
        i++;
    }
    return sv_from_parts(sv.data + i, sv.count - i);
}

StringView sv_trim_right(StringView sv)
{
    size_t i = sv.count - 1;
    while (i > 0 && isspace(sv.data[i])) {
        i--;
    }
    return sv_from_parts(sv.data, i+1);
}

StringView sv_trim(StringView sv)
{
    return sv_trim_left(sv_trim_right(sv));
}

bool sv_chop_until_delim(StringView *sv, char delim, StringView *out_chopped_chunk)
{
    size_t i = 0;
    while (i < sv->count &&
           sv->data[i] != delim) {
        i++;
    }

    // No delim, change nothing
    if (i == sv->count) {
        *out_chopped_chunk = SV_NULL;
        return false;
    }

    // Found delim
    if (out_chopped_chunk) {
        StringView chopped_chunk = sv_from_parts(sv->data, i);
        *out_chopped_chunk = chopped_chunk;
    }
    sv->data  += i + 1;
    sv->count -= i + 1;

    return true;
}

bool sv_eq(StringView a, StringView b)
{
    if (a.count != b.count) {
        return false;
    } else {
        return memcmp(a.data, b.data, a.count) == 0;
    }
}

#endif // SV_IMPLEMENTATION
