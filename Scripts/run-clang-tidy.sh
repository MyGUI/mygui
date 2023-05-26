# Runs clang tidy on a cmake build directory. This directory must be prepared with the
# -DMYGUI_CLANG_TIDY_BUILD=True option to prevent issues with clang-tidy not handling PCH.
./run-clang-tidy.py -p ../cmake-build-debug -fix -format \
  -config-file ../.clang-tidy \
  -ignore ../.clang-tidy-ignore \
  2> /tmp/run-clang-tidy-output.txt
