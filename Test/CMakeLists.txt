add_executable(XorStringTest "Source/Main.cpp")

target_link_libraries(XorStringTest PUBLIC XorString)

target_compile_options(XorStringTest PUBLIC "-O3")

add_test(NAME XorStringTest COMMAND $<TARGET_FILE:XorStringTest>)