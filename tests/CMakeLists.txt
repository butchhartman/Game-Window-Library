link_libraries(${PROJECT_NAME})

add_executable(linkingTestDriver linking.c)

add_executable(windowInitializationTestDriver initialization.c)

add_executable(callbackTestDriver callback.c)

add_executable(queueTestDriver queueTests.c)

add_executable(lockMouseTestDriver lockMouseTest.c)

add_executable(openglLoadingTestDriver openGLLoading.c)

add_test(openglLoadingTest openglLoadingTestDriver)
add_test(lockMouseTest lockMouseTestDriver)
add_test(queueTest queueTestDriver)
add_test(callbackTest callbackTestDriver)
add_test(linkingTest linkingTestDriver)
add_test(windowInitializationTest windowInitializationTestDriver)