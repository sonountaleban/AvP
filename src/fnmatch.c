#include <string.h>
#include <ctype.h>
#include "fnmatch.h"

static char fnm_lower(char c, int flags) {
    if (flags & FNM_CASEFOLD)
        return (char)tolower((unsigned char)c);
    return c;
}

/* Helper to check if string ends at a leading directory match */
static int check_leading_dir(const char *p, const char *s, int flags) {
    if ((flags & FNM_LEADING_DIR) && *s == '/') {
        // If pattern is fully consumed and string hits a '/', it's a match under FNM_LEADING_DIR
        if (*p == '\0')
            return 0;
    }
    return -1;
}

int fnmatch(const char *__pattern, const char *__name, int __flags) {
    const char *p = __pattern;
    const char *s = __name;

    // Check FNM_PERIOD rule for the initial character
    if ((__flags & FNM_PERIOD) && *s == '.' && *p != '.') {
        if (!((__flags & FNM_NOESCAPE) == 0 && p[0] == '\\' && p[1] == '.') && p[0] != '.')
            return FNM_NOMATCH;
    }

    while (*p != '\0') {
        switch (*p) {
            case '?':
                if (*s == '\0') {
                    if (check_leading_dir(p, s, __flags) == 0) return 0;
                    return FNM_NOMATCH;
                }
                if ((__flags & FNM_PATHNAME) && *s == '/')
                    return FNM_NOMATCH;
                if ((__flags & FNM_PERIOD) && *s == '.' && (s == __name || ((__flags & FNM_PATHNAME) && s[-1] == '/')))
                    return FNM_NOMATCH;
                p++;
                s++;
                break;

            case '*':
                // Collapse multiple adjacent '*' into one
                while (*p == '*')
                    p++;

                // If pattern ends with '*', it matches the remainder (subject to pathname/period rules)
                if (*p == '\0') {
                    if ((__flags & FNM_PATHNAME) && strchr(s, '/'))
                        return FNM_NOMATCH;
                    if ((__flags & FNM_PERIOD) && *s == '.' && (s == __name || ((__flags & FNM_PATHNAME) && s[-1] == '/')))
                        return FNM_NOMATCH;
                    return 0;
                }

                // Recursively match the rest of the pattern against suffixes of the string
                while (*s != '\0') {
                    if ((__flags & FNM_PATHNAME) && *s == '/')
                        break;
                    if ((__flags & FNM_PERIOD) && *s == '.' && (s == __name || ((__flags & FNM_PATHNAME) && s[-1] == '/'))) {
                        s++;
                        continue;
                    }
                    if (fnmatch(p, s, __flags) == 0)
                        return 0;
                    s++;
                }

                // Check FNM_LEADING_DIR fallback at the end of string
                if (*s == '\0' && check_leading_dir(p, s, __flags) == 0)
                    return 0;

                return FNM_NOMATCH;

            case '[': {
                if (*s == '\0') {
                    if (check_leading_dir(p, s, __flags) == 0) return 0;
                    return FNM_NOMATCH;
                }
                if ((__flags & FNM_PATHNAME) && *s == '/')
                    return FNM_NOMATCH;
                if ((__flags & FNM_PERIOD) && *s == '.' && (s == __name || ((__flags & FNM_PATHNAME) && s[-1] == '/')))
                    return FNM_NOMATCH;

                p++;
                int negate = 0;
                if (*p == '!' || *p == '^') {
                    negate = 1;
                    p++;
                }

                int matched = 0;
                char c = fnm_lower(*s, __flags);

                while (*p != '\0' && *p != ']') {
                    if (!(__flags & FNM_NOESCAPE) && *p == '\\') {
                        p++;
                    }
                    if (*p == '\0')
                        break;

                    char start = fnm_lower(*p, __flags);
                    p++;

                    // Check for ranges (e.g., [a-z])
                    if (*p == '-' && p[1] != ']' && p[1] != '\0') {
                        p++; // skip '-'
                        if (!(__flags & FNM_NOESCAPE) && *p == '\\') {
                            p++;
                        }
                        char end = fnm_lower(*p, __flags);
                        p++;
                        if (c >= start && c <= end)
                            matched = 1;
                    } else {
                        if (c == start)
                            matched = 1;
                    }
                }

                if (*p == ']')
                    p++;
                else
                    return FNM_NOMATCH; // Unterminated bracket

                if (negate)
                    matched = !matched;

                if (!matched)
                    return FNM_NOMATCH;
                s++;
                break;
            }

            case '\\':
                if (!(__flags & FNM_NOESCAPE)) {
                    p++;
                    if (*p == '\0')
                        return FNM_NOMATCH;
                }
                /* fallthrough */

            default:
                if (*s == '\0') {
                    if (check_leading_dir(p, s, __flags) == 0) return 0;
                    return FNM_NOMATCH;
                }
                if (fnm_lower(*p, __flags) != fnm_lower(*s, __flags))
                    return FNM_NOMATCH;
                p++;
                s++;
                break;
        }
    }

    if (*s == '\0')
        return 0;

    return check_leading_dir(p, s, __flags);
}
