#######################################################################
# Sanitizer options (Clang only)
#######################################################################

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# usage:
	# UBSAN_OPTIONS=print_stacktrace=1,suppressions=sanitizers/UBSanitizer.supp <command>
	option(MYGUI_UB_SANITIZER "Compile with undefined behaviour sanitizer" FALSE)
	if(MYGUI_UB_SANITIZER)
		add_compile_options("-fno-omit-frame-pointer" "-fno-optimize-sibling-calls")
		add_compile_options("-fsanitize=undefined" "-fno-sanitize-recover=undefined")
		add_link_options("-fsanitize=undefined")
	endif()

	# usage:
	# ASAN_OPTIONS=alloc_dealloc_mismatch=0,check_initialization_order=1,detect_stack_use_after_return=1 LSAN_OPTIONS=suppressions=sanitizers/LeaksSanitizer.supp <command>
	# alloc_dealloc_mismatch is here due to issues with Ogre and no clear way to suppress the issue
	option(MYGUI_ADDRESS_SANITIZER "Compile with address sanitizer" FALSE)
	if(MYGUI_ADDRESS_SANITIZER)
		add_compile_options("-fno-omit-frame-pointer" "-fno-optimize-sibling-calls")
		add_compile_options("-fsanitize=address" "-fsanitize-address-use-after-scope")
		add_link_options("-fsanitize=address")
	endif()
endif()
