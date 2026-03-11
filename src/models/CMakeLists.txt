add_library(models_lib STATIC
    src/entry.cpp
    src/user.cpp
)

target_include_directories(models_lib PUBLIC include)
