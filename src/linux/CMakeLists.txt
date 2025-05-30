#https://cmake.org/cmake/help/latest/policy/CMP0073.html#policy:CMP0076

cmake_minimum_required(VERSION 3.13)
target_sources(${PROJECT_NAME} 
                PRIVATE
                src/linux.c
                PUBLIC FILE_SET HEADERS
                BASE_DIRS
                include)
