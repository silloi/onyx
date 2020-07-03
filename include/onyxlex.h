#ifndef ONYXLEX_H
#define ONYXLEX_H

#include "bh.h"

typedef enum TokenType {
    TOKEN_TYPE_UNKNOWN,
    TOKEN_TYPE_END_STREAM,

    TOKEN_TYPE_COMMENT,

    TOKEN_TYPE_KEYWORD_STRUCT,
    TOKEN_TYPE_KEYWORD_USE,
    TOKEN_TYPE_KEYWORD_EXPORT,
    TOKEN_TYPE_KEYWORD_IF,
    TOKEN_TYPE_KEYWORD_ELSE,
    TOKEN_TYPE_KEYWORD_ELSEIF,
    TOKEN_TYPE_KEYWORD_RETURN,
    TOKEN_TYPE_KEYWORD_FOREIGN,
    TOKEN_TYPE_KEYWORD_PROC,
    TOKEN_TYPE_KEYWORD_CAST,
    TOKEN_TYPE_KEYWORD_WHILE,
    TOKEN_TYPE_KEYWORD_BREAK,
    TOKEN_TYPE_KEYWORD_CONTINUE,

    TOKEN_TYPE_RIGHT_ARROW,
    TOKEN_TYPE_LEFT_ARROW,
    TOKEN_TYPE_OPEN_PAREN,
    TOKEN_TYPE_CLOSE_PAREN,
    TOKEN_TYPE_OPEN_BRACE,
    TOKEN_TYPE_CLOSE_BRACE,
    TOKEN_TYPE_OPEN_BRACKET,
    TOKEN_TYPE_CLOSE_BRACKET,

    TOKEN_TYPE_SYM_PLUS,
    TOKEN_TYPE_SYM_MINUS,
    TOKEN_TYPE_SYM_STAR,
    TOKEN_TYPE_SYM_PERCENT,
    TOKEN_TYPE_SYM_DOT,
    TOKEN_TYPE_SYM_FSLASH,
    TOKEN_TYPE_SYM_BSLASH,
    TOKEN_TYPE_SYM_COLON,
    TOKEN_TYPE_SYM_SEMICOLON,
    TOKEN_TYPE_SYM_COMMA,
    TOKEN_TYPE_SYM_GREATER,
    TOKEN_TYPE_SYM_GREATER_EQUAL,
    TOKEN_TYPE_SYM_LESS,
    TOKEN_TYPE_SYM_LESS_EQUAL,
    TOKEN_TYPE_SYM_EQUAL_EQUAL,
    TOKEN_TYPE_SYM_NOT_EQUAL,
    TOKEN_TYPE_SYM_EQUALS,
    TOKEN_TYPE_SYM_TILDE,
    TOKEN_TYPE_SYM_BANG,
    TOKEN_TYPE_SYM_CARET,
    TOKEN_TYPE_SYM_AMPERSAND,
    TOKEN_TYPE_SYM_PLUS_EQUAL,
    TOKEN_TYPE_SYM_MINUS_EQUAL,
    TOKEN_TYPE_SYM_STAR_EQUAL,
    TOKEN_TYPE_SYM_FSLASH_EQUAL,
    TOKEN_TYPE_SYM_PERCENT_EQUAL,

    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_LITERAL_STRING,
    TOKEN_TYPE_LITERAL_NUMERIC,
    TOKEN_TYPE_LITERAL_BOOL_TRUE,
    TOKEN_TYPE_LITERAL_BOOL_FALSE,

    TOKEN_TYPE_COUNT
} TokenType;

typedef struct OnyxFilePos {
    const char* filename;
    u64 line, column;
} OnyxFilePos;

typedef struct OnyxToken {
    TokenType type;
    i32 length;
    char* token;
    OnyxFilePos pos;
} OnyxToken;

typedef struct OnyxTokenizer {
    char *start, *curr, *end;

    const char* filename;

    char* line_start;
    u64 line_number;

    bh_arr(OnyxToken) tokens;
} OnyxTokenizer;

const char* onyx_get_token_type_name(TokenType tkn_type);
void onyx_token_null_toggle(OnyxToken* tkn);
OnyxToken* onyx_get_token(OnyxTokenizer* tokenizer);
OnyxTokenizer onyx_tokenizer_create(bh_allocator allocator, bh_file_contents *fc);
void onyx_tokenizer_free(OnyxTokenizer* tokenizer);
void onyx_lex_tokens(OnyxTokenizer* tokenizer);

#endif
