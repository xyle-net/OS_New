cmake_minimum_required(VERSION 3.20)
project(lab1)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Поиск GTest
find_package(GTest REQUIRED)

# Добавление исходных файлов
set(SOURCES
    src/main.cpp
    src/parent.cpp
    src/utils.cpp
    src/child1.cpp
    src/child2.cpp
    lab1_test.cpp
)

# Добавление исполняемых файлов
add_executable(
    lab1
    src/main.cpp
    src/parent.cpp
    src/utils.cpp
)

add_executable(
    child1
    src/child1.cpp
)

add_executable(
    child2
    src/child2.cpp
)

# Добавление исполняемого файла для тестов
add_executable(
    lab1_test
    lab1_test.cpp
    src/parent.cpp
    src/utils.cpp
)

# Установка директорий для включения
target_include_directories(lab1 PRIVATE include)
target_include_directories(lab1_test PRIVATE include)

# Связывание библиотек
target_link_libraries(lab1_test PRIVATE GTest::GTest GTest::Main)

# Установка опций компиляции
target_compile_options(lab1 PRIVATE -Wall -Wextra -Wpedantic)