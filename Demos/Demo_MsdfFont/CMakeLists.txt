mygui_demo(Demo_MsdfFont)

target_compile_definitions(Demo_MsdfFont
    PRIVATE
        $<$<BOOL:${MYGUI_MSDF_FONTS}>:MYGUI_MSDF_FONTS>
)