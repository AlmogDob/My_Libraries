/**
 * @file
 * @brief Lightweight string and line manipulation helpers.
 *
 * @details
 * This single-header module provides small utilities for working with
 * C strings:
 *  - Reading a single line from a FILE stream
 *  - Measuring string length
 *  - Extracting the next "word" (token) from a line using a separator
 *  - Cutting the extracted word from the source buffer
 *  - Copying a substring by indices
 *  - Counting occurrences of a substring
 *  - A boolean-style strncmp (returns 1 on equality, 0 otherwise)
 *
 * Usage
 *  - In exactly one translation unit, define
 *    ALMOG_STRING_MANIPULATION_IMPLEMENTATION before including this header
 *    to compile the implementation.
 *  - In all other files, include the header without the macro to get
 *    declarations only.
 *
 * Notes and limitations
 *  - All destination buffers must be large enough; functions do not grow or
 *    allocate buffers.
 *  - asm_get_line enforces MAX_LEN_LINE characters (not counting the
 *    terminating '\0'). Longer lines cause a fatal error via exit(1).
 *  - asm_strncmp differs from the standard C strncmp: this version returns
 *    1 if equal and 0 otherwise.
 *  - These functions are not locale-aware unless otherwise noted (isspace is
 *    used for whitespace handling).
 */

#ifndef ALMOG_STRING_MANIPULATION_H_
#define ALMOG_STRING_MANIPULATION_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @def ASM_MAXDIR
 * @brief Generic maximum directory length constant (not used by the functions
 * in this header but available to callers).
 */
#define ASM_MAXDIR 100

/**
 * @def ASM_MAX_LEN_LINE
 * @brief Maximum number of characters read by asm_get_line (excluding the
 * terminating null).
 *
 * If an input line exceeds this value before encountering '\n' or EOF,
 * asm_get_line prints an error to stderr and terminates the process with
 * exit(1).
 */
#define ASM_MAX_LEN_LINE (int)1e3

/**
 * @def asm_dprintSTRING(expr)
 * @brief Debug print a C string expression as "expr = value\n".
 * @param expr An expression that yields a pointer to char (const or non-const).
 */
#define asm_dprintSTRING(expr) printf(#expr " = %s\n", expr)

/**
 * @def asm_dprintCHAR(expr)
 * @brief Debug print a character expression as "expr = c\n".
 * @param expr An expression that yields a character promoted to int.
 */
#define asm_dprintCHAR(expr) printf(#expr " = %c\n", expr)

/**
 * @def asm_dprintINT(expr)
 * @brief Debug print an integer expression as "expr = n\n".
 * @param expr An expression that yields an int.
 */
#define asm_dprintINT(expr) printf(#expr " = %d\n", expr)

/**
 * @def asm_dprintSIZE_T(expr)
 * @brief Debug print a size_t expression as "expr = n\n".
 * @param expr An expression that yields a size_t.
 */
#define asm_dprintSIZE_T(expr) printf(#expr " = %zu\n", expr)

int asm_get_line(FILE *fp, char *dst);
int asm_length(char *str);
int asm_get_next_word_from_line(char *dst, char *src, char seperator);
void asm_copy_array_by_indesies(char *target, int start, int end, char *src);
int asm_get_word_and_cut(char *dst, char *src, char seperator);
int asm_str_in_str(char *src, char *word2search);
int asm_strncmp(const char *s1, const char *s2, const int N);

#endif /*ALMOG_STRING_MANIPULATION_H_*/

#ifdef ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#undef ALMOG_STRING_MANIPULATION_IMPLEMENTATION


/**
 * @brief Read a single line from a stream into a buffer.
 *
 * Reads characters from the FILE stream until a newline ('\n') or EOF is
 * encountered. The newline, if present, is not copied. The result is
 * always null-terminated.
 *
 * @param fp Input stream (must be non-NULL).
 * @param dst Destination buffer. Must have capacity of at least
 *            MAX_LEN_LINE + 1 bytes.
 * @return Number of characters stored in dst (excluding the terminating null).
 * @retval -1 EOF was encountered before any character was read.
 *
 * @note If the line exceeds MAX_LEN_LINE characters before a newline or EOF,
 *       the function prints an error and calls exit(1).
 * @note An empty line returns 0 (not -1).
 */
int asm_get_line(FILE *fp, char *dst)
{
    int i = 0;
    char c;

    while ((c = fgetc(fp)) != '\n' && c != EOF) {
        dst[i] = c;
        i++;
        if (i >= ASM_MAX_LEN_LINE) {
            fprintf(stderr, "ERROR: line too long\n");
            exit(1);
        }
    }
    dst[i] = '\0';
    if (c == EOF && i == 0) {
        return -1;
    }
    return i;
}

/**
 * @brief Compute the length of a null-terminated C string.
 *
 * @param str Null-terminated string (must be non-NULL).
 * @return The number of characters before the terminating null byte.
 *
 */
int asm_length(char *str)
{
    char c;
    int i = 0;

    while ((c = str[i]) != '\0') {
        i++;
    }
    return i++;
}

/**
 * @brief Extract the next word from a line without modifying the source.
 *
 * Skips leading whitespace in src (as determined by isspace), then copies
 * characters into dst until one of the following is seen: the separator,
 * a newline ('\n'), or the string terminator ('\0'). The copied word in dst
 * is null-terminated and is never empty on success.
 *
 * Special case:
 *  - If the very first character in src (at index 0, without leading
 *    whitespace) is the separator, '\n', or '\0', that single character is
 *    returned as a one-character "word".
 *
 * @param dst Destination buffer for the extracted word. Must be large enough
 *            to hold the token plus the null terminator.
 * @param src Source C string to parse (not modified by this function).
 * @param seperator Separator character to stop at (spelling as in the API).
 * @return The number of characters consumed from src (i.e., the index of the
 *         first unconsumed character).
 * @retval -1 No word was found (e.g., only whitespace before a delimiter or
 *         end-of-string).
 *
 * @note The source buffer is not altered. To both extract and advance/cut the
 *       source, see asm_get_word_and_cut.
 */
int asm_get_next_word_from_line(char *dst, char *src, char seperator)
{
    int i = 0, j = 0;
    char c;

    while (isspace((c = src[i]))) {
        i++;
    }

    while ((c = src[i]) != seperator &&
                      c != '\n'&&
                      c != '\0') {
                        dst[j] = src[i];
                        i++;
                        j++;
    }

    if ((c == seperator || 
         c == '\n'||
         c == '\0') && i == 0) {
            dst[j++] = c;
            i++;
    }

    dst[j] = '\0';

    if (j == 0) {
        return -1;
    }
    return i;

}

/**
 * @brief Copy a substring [start, end) from src into target and null-terminate.
 *
 * Copies characters with indices i = start, start+1, ..., end-1 from src into
 * target, then writes a terminating '\0'.
 *
 * @param target Destination buffer. Must be large enough to hold (end - start)
 *              characters plus the null terminator.
 * @param start Inclusive start index within src (0-based).
 * @param end Exclusive end index within src (must satisfy end >= start).
 * @param src Source string buffer.
 *
 * @warning No bounds checking is performed. The caller must ensure valid
 *          indices and sufficient target capacity.
 * @note This routine supports in-place "left-shift" usage where target == src
 *       and start > 0 (used by asm_get_word_and_cut).
 */
void asm_copy_array_by_indesies(char *target, int start, int end, char *src)
{
    int j = 0;
    for (int i = start; i < end; i++) {
        target[j] = src[i];
        j++;
    }
    target[j] = '\0';
}

/**
 * @brief Get the next word and cut the source string at that point.
 *
 * Extracts the next word from src (per asm_get_next_word_from_line semantics)
 * into dst. On success, src is modified in-place to remove the consumed
 * prefix. The new src begins at the stopping character (the separator,
 * newline, or terminator).
 *
 * Example: For src = "abc,def", separator = ','
 *  - dst becomes "abc"
 *  - src becomes ",def" (note the leading separator remains)
 *
 * @param dst Destination buffer for the extracted word (large enough for the
 *            token and terminating null).
 * @param src Source buffer. Modified in-place if a word is found.
 * @param seperator Separator character to stop at (spelling as in the API).
 * @return 1 if a word was extracted and src adjusted, 0 otherwise.
 */
int asm_get_word_and_cut(char *dst, char *src, char seperator)
{
    int last_pos;

    if (src[0] == '\0') {
        return 0;
    }
    last_pos = asm_get_next_word_from_line(dst, src, seperator);
    if (last_pos == -1) {
        return 0;
    }
    asm_copy_array_by_indesies(src, last_pos, asm_length(src), src);
    return 1;
}

/**
 * @brief Count occurrences of a substring within a string.
 *
 * Counts how many times word2search appears in src. Occurrences may overlap.
 *
 * @param src The string to search in (must be null-terminated).
 * @param word2search The substring to find (must be null-terminated).
 * @return The number of (possibly overlapping) occurrences found.
 *
 */
int asm_str_in_str(char *src, char *word2search)
{
    int i = 0, num_of_accur = 0;
    while (src[i] != '\0') {
        if (asm_strncmp(src+i, word2search, asm_length(word2search))) {
            num_of_accur++;
        }
        i++;
    }
    return num_of_accur;
}

/**
 * @brief Compare up to N characters for equality (boolean result).
 *
 * Returns 1 if the first N characters of s1 and s2 are all equal; otherwise
 * returns 0. Unlike the standard C strncmp, which returns 0 on equality and
 * a non-zero value on inequality/order, this function returns a boolean-like
 * result (1 == equal, 0 == different).
 *
 * @param s1 First string (may be shorter than N).
 * @param s2 Second string (may be shorter than N).
 * @param N  Number of characters to compare.
 * @return 1 if equal for the first N characters, 0 otherwise.
 */
int asm_strncmp(const char *s1, const char *s2, const int N)
{
    int i = 0;
    while (i < N) {
        if (s1[i] == '\0' && s2[i] == '\0') {
            break;
        }
        if (s1[i] != s2[i] || (s1[i] == '\0') || (s2[i] == '\0')) {
            return 0;
        }
        i++;
    }
    return 1;
}


#endif /*ALMOG_STRING_MANIPULATION_IMPLEMENTATION*/

