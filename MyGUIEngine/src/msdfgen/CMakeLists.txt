file(GLOB_RECURSE msdfgen_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	"core/*.h"
	"core/*.hpp"
)

file(GLOB_RECURSE msdfgen_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	"core/*.cpp"
)

file(GLOB_RECURSE msdfgen-ext_PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	"ext/*.h"
)

file(GLOB_RECURSE msdfgen-ext_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	"ext/*.cpp"
)

add_library(msdfgen
	STATIC
	${msdfgen_HEADERS}
	${msdfgen_SOURCES}
	${msdfgen-ext_PUBLIC_HEADERS}
	${msdfgen-ext_SOURCES}
)

if (NOT MSVC)
	target_compile_options(msdfgen PRIVATE -w)
else ()
	target_compile_options(msdfgen PRIVATE /W0)
endif ()

target_compile_definitions(msdfgen PUBLIC MSDFGEN_USE_CPP11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(SYSTEM
	${FREETYPE_INCLUDE_DIRS}
)
