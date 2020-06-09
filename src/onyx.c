#define BH_NO_STRING
// #define BH_DEBUG
#define BH_DEFINE
#include "bh.h"

#include "onyxlex.h"
#include "onyxmsgs.h"
#include "onyxparser.h"
#include "onyxutils.h"
#include "onyxwasm.h"

int main(int argc, char *argv[]) {
	bh_file source_file;
	bh_file_error err = bh_file_open(&source_file, argv[1]);
	if (err != BH_FILE_ERROR_NONE) {
		bh_printf_err("Failed to open file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	bh_allocator alloc = bh_heap_allocator();

	bh_file_contents fc = bh_file_read_contents(alloc, &source_file);
	bh_file_close(&source_file);

	OnyxTokenizer tokenizer = onyx_tokenizer_create(alloc, &fc);
	onyx_lex_tokens(&tokenizer);
	bh_arr(OnyxToken) token_arr = tokenizer.tokens;

#if 0
	bh_printf("There are %d tokens (Allocated space for %d tokens)\n", bh_arr_length(token_arr), bh_arr_capacity(token_arr));

	for (OnyxToken* it = token_arr; !bh_arr_end(token_arr, it); it++) {
		onyx_token_null_toggle(*it);
		bh_printf("%s (%s:%l:%l)\n", onyx_get_token_type_name(it->type), it->pos.filename, it->pos.line, it->pos.column);
		onyx_token_null_toggle(*it);
	}
#endif

	bh_arena msg_arena;
	bh_arena_init(&msg_arena, alloc, 4096);
	bh_allocator msg_alloc = bh_arena_allocator(&msg_arena);

	OnyxMessages msgs;
	onyx_message_create(msg_alloc, &msgs);

	bh_arena ast_arena;
	bh_arena_init(&ast_arena, alloc, 16 * 1024 * 1024); // 16MB
	bh_allocator ast_alloc = bh_arena_allocator(&ast_arena);

	OnyxParser parser = onyx_parser_create(ast_alloc, &tokenizer, &msgs);
	OnyxAstNode* program = onyx_parse(&parser);

	// NOTE: if there are errors, assume the parse tree was generated wrong,
	// even if it may have still been generated correctly.
	if (onyx_message_has_errors(&msgs)) {
		onyx_message_print(&msgs);
		goto main_exit;
	} else {
		onyx_ast_print(program, 0);
		bh_printf("\nNo errors.\n");
	}

	OnyxWasmModule wasm_mod = onyx_wasm_generate_module(alloc, program);

#if 1
	// NOTE: Ensure type table made correctly

	bh_printf("Type map:\n");
	bh_hash_iterator type_map_it = bh_hash_iter_setup(i32, wasm_mod.type_map);
	while (bh_hash_iter_next(&type_map_it)) {
		const char* key = bh_hash_iter_key(type_map_it);
		i32 value = bh_hash_iter_value(i32, type_map_it);

		bh_printf("%s -> %d\n", key, value);
	}

	bh_printf("Type list:\n");
	WasmFuncType** func_type = wasm_mod.functypes;
	while (!bh_arr_end(wasm_mod.functypes, func_type)) {
		for (int p = 0; p < (*func_type)->param_count; p++) {
			bh_printf("%c ", (*func_type)->param_types[p]);
		}
		bh_printf("-> ");
		bh_printf("%c\n", (*func_type)->return_type);

		func_type++;
	}
#endif

#if 1
	// NOTE: Ensure the export table was built correctly

	bh_printf("Function types:\n");
	for (WasmFunc* func_it = wasm_mod.funcs; !bh_arr_end(wasm_mod.funcs, func_it); func_it++) {
		bh_printf("%d\n", func_it->type_idx);
	}

	bh_printf("Exports:\n");
	bh_hash_iterator export_it = bh_hash_iter_setup(WasmExport, wasm_mod.exports);
	while (bh_hash_iter_next(&export_it)) {
		const char* key = bh_hash_iter_key(export_it);
		WasmExport value = bh_hash_iter_value(WasmExport, export_it);

		bh_printf("%s: %d %d\n", key, value.kind, value.idx);
	}
#endif


	onyx_wasm_module_free(&wasm_mod);
main_exit: // NOTE: Cleanup, since C doesn't have defer
	bh_arena_free(&msg_arena);
	bh_arena_free(&ast_arena);
	onyx_parser_free(&parser);
	onyx_tokenizer_free(&tokenizer);
	bh_file_contents_free(&fc);

	return 0;
}
