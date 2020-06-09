#ifndef ONYXWASM_H
#define ONYXWASM_H

#define BH_NO_STRING
#include "bh.h"

#include "onyxparser.h"

typedef u8 WasmType;

extern const WasmType WASM_TYPE_INT32;
extern const WasmType WASM_TYPE_INT64;
extern const WasmType WASM_TYPE_FLOAT32;
extern const WasmType WASM_TYPE_FLOAT64;

typedef struct WasmFuncType {
	// NOTE: For now, WASM only allows for 1 return value.
	// This may be lifted in the future.
	i32 param_count;
	WasmType return_type;
	WasmType param_types[];
} WasmFuncType;


typedef struct WasmFunc {
	i32 type_idx;
} WasmFunc;

typedef enum WasmExportKind {
	WASM_EXPORT_FUNCTION,
	WASM_EXPORT_TABLE,
	WASM_EXPORT_MEMORY,
	WASM_EXPORT_GLOBAL,
} WasmExportKind;

typedef struct WasmExport {
	WasmExportKind kind;
	i32 idx;
} WasmExport;

typedef struct OnyxWasmModule {
	bh_allocator allocator;

	// NOTE: Used internally as a map from strings that represent function types,
	// 0x7f 0x7f : 0x7f ( (i32, i32) -> i32 )
	// to the function type index if it has been created.
	bh_hash(i32) type_map;
	i32 next_type_idx;
	// NOTE: This have to be pointers because the type is variadic in size
	bh_arr(WasmFuncType*) functypes;

	bh_arr(WasmFunc) funcs;
	i32 next_func_idx;

	bh_hash(WasmExport) exports;
} OnyxWasmModule;

OnyxWasmModule onyx_wasm_generate_module(bh_allocator alloc, OnyxAstNode* program);
void onyx_wasm_module_free(OnyxWasmModule* module);

#endif
