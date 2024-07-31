#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define SV_IMPLEMENTATION
#include "sv.h"

#define SV(cstr_lit) sv_from_parts(cstr_lit, sizeof(cstr_lit) - 1)
#define SV_STATIC(cstr_lit)    \
    {                          \
        (cstr_lit),            \
        sizeof(cstr_lit) - 1   \
    }

void sv_assert_eq_StringView(const char *file, size_t line,
                             const char *expected_expr, StringView expected,
                             const char *actual_expr, StringView actual)
{
    if (!sv_eq(expected, actual)) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: " SV_Fmt "\n", SV_Arg(expected));
        fprintf(stderr, "  ACTUAL:   " SV_Fmt "\n", SV_Arg(actual));
        exit(1);
    }
}

void sv_assert_eq_size_t(const char *file, size_t line,
                         const char *expected_expr, size_t expected,
                         const char *actual_expr, size_t actual)
{
    if (expected != actual) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: %zu\n", expected);
        fprintf(stderr, "  ACTUAL:   %zu\n", actual);
        exit(1);
    }
}

void sv_assert_eq_uint64_t(const char *file, size_t line,
                           const char *expected_expr, uint64_t expected,
                           const char *actual_expr, uint64_t actual)
{
    if (expected != actual) {
        fprintf(stderr, "%s:%zu: failed: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  expected: %" PRIu64 "\n", expected);
        fprintf(stderr, "  actual:   %" PRIu64 "\n", actual);
        exit(1);
    }
}

void sv_assert_true(const char *file, size_t line,
                    const char *expression_cstr,
                    bool expression)
{
    if (!expression) {
        fprintf(stderr, "%s:%zu: failed: %s\n", file, line, expression_cstr);
        exit(1);
    }
}

#define ASSERT_EQ(type, expected, actual) sv_assert_eq_##type(__FILE__, __LINE__, #expected, expected, #actual, actual)
#define ASSERT_TRUE(expression) sv_assert_true(__FILE__, __LINE__, #expression, expression)

int main(void)
{
    { // Init functions
        ASSERT_EQ(StringView, SV("Foo"), sv_from_cstr("Foo"));
        ASSERT_EQ(StringView, SV("Foo"), sv_from_parts("Foo", 3));

        StringView null_init = {0};
        ASSERT_EQ(StringView, SV_NULL, null_init);
    }

    { // Whitespace Trimming functions
        ASSERT_EQ(StringView, SV("Foo"), sv_trim_left(SV("     Foo")));
        ASSERT_EQ(StringView, SV("Foo"), sv_trim_right(SV("Foo    ")));
        ASSERT_EQ(StringView, SV("Foo"), sv_trim(SV("    Foo      ")));

        ASSERT_EQ(StringView, SV("Foo"), sv_trim_left(SV("Foo")));
        ASSERT_EQ(StringView, SV("Foo"), sv_trim_right(SV("Foo")));
        ASSERT_EQ(StringView, SV("Foo"), sv_trim(SV("Foo")));
    }

    { // Chopping functions
        StringView sv = sv_from_cstr("Foo\nBar\nBaz");
        StringView chunk = {0};

        sv_chop_until_delim(&sv, '\n', &chunk);
        ASSERT_EQ(StringView, SV("Bar\nBaz"), sv);
        ASSERT_EQ(StringView, SV("Foo"), chunk);

        sv_chop_until_delim(&sv, '\n', &chunk);
        ASSERT_EQ(StringView, SV("Baz"), sv);
        ASSERT_EQ(StringView, SV("Bar"), chunk);

        sv_chop_until_delim(&sv, '\n', &chunk);
        ASSERT_EQ(StringView, SV("Baz"), sv);
    }

    return 0;
}
