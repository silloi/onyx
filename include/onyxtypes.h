#ifndef ONYXTYPES_H
#define ONYXTYPES_H

#include "bh.h"

enum BasicKind {
    Basic_Kind_Void,

    Basic_Kind_Bool,

    Basic_Kind_I8,
    Basic_Kind_U8,
    Basic_Kind_I16,
    Basic_Kind_U16,
    Basic_Kind_I32,
    Basic_Kind_U32,
    Basic_Kind_I64,
    Basic_Kind_U64,

    Basic_Kind_F32,
    Basic_Kind_F64,

    Basic_Kind_Rawptr,
};

enum BasicFlag {
    Basic_Flag_Boolean          = BH_BIT(0),
    Basic_Flag_Integer          = BH_BIT(1),
    Basic_Flag_Unsigned         = BH_BIT(2),
    Basic_Flag_Float            = BH_BIT(3),
    Basic_Flag_Pointer          = BH_BIT(4),

    Basic_Flag_Numeric          = Basic_Flag_Integer | Basic_Flag_Float,
    Basic_Flag_Ordered          = Basic_Flag_Integer | Basic_Flag_Float | Basic_Flag_Pointer,
    Basic_Flag_Constant_Type    = Basic_Flag_Boolean | Basic_Flag_Numeric | Basic_Flag_Pointer,
    Basic_Flag_Numeric_Ordered  = Basic_Flag_Numeric | Basic_Flag_Ordered,
};

typedef struct TypeBasic {
    enum BasicKind kind;
    u32 flags;
    i64 size; // NOTE: In bytes
    const char* name;
} TypeBasic;

// NOTE: Forward declaration for some of the types below
typedef struct Type Type;

#define TYPE_KINDS \
    TYPE_KIND(Basic, TypeBasic)                       \
    TYPE_KIND(Pointer, struct { Type *elem; })

typedef enum TypeKind {
    Type_Kind_Invalid,

#define TYPE_KIND(k, ...) Type_Kind_##k,
    TYPE_KINDS
#undef TYPE_KIND

    Type_Kind_Count,
} TypeKind;

#define TYPE_KIND(k, ...) typedef __VA_ARGS__ Type ## k;
    TYPE_KINDS
#undef TYPE_KIND

enum TypeFlag {
    Type_Flag_Default
};

struct Type {
    TypeKind kind;

    union {
#define TYPE_KIND(k, ...) Type##k k;
        TYPE_KINDS
#undef TYPE_KIND
    };

    u32 flags;
};

extern Type basic_types[];


#endif // #ifndef ONYX_TYPES
