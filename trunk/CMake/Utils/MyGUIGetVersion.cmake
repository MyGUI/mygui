include(PreprocessorUtils)

macro(mygui_get_version HEADER)
  file(READ ${HEADER} TEMP_VAR_CONTENTS)
  get_preprocessor_entry(TEMP_VAR_CONTENTS MYGUI_VERSION_MAJOR MYGUI_VERSION_MAJOR)
  get_preprocessor_entry(TEMP_VAR_CONTENTS MYGUI_VERSION_MINOR MYGUI_VERSION_MINOR)
  get_preprocessor_entry(TEMP_VAR_CONTENTS MYGUI_VERSION_PATCH MYGUI_VERSION_PATCH)
  get_preprocessor_entry(TEMP_VAR_CONTENTS MYGUI_VERSION_NAME MYGUI_VERSION_NAME)
  get_preprocessor_entry(TEMP_VAR_CONTENTS MYGUI_VERSION_SUFFIX MYGUI_VERSION_SUFFIX)
  set(MYGUI_VERSION "${MYGUI_VERSION_MAJOR}.${MYGUI_VERSION_MINOR}.${MYGUI_VERSION_PATCH}")
endmacro()