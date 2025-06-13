add_subdirectory(datastructures)

if (WIN32)
    add_subdirectory(win32)
elseif (APPLE) 
    add_subdirectory(mac)
elseif (LINUX)
    add_subdirectory(linux)
endif()
