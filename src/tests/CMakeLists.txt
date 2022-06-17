file(GLOB TESTS "test_*.c")
foreach(T_FILE ${TESTS})
  get_filename_component(T_NAME ${T_FILE} NAME_WE)
  add_executable(${T_NAME} ${T_FILE})
  target_link_libraries(${T_NAME} ${PROJECT_LIBS} ${THIRDPARTY_LIBRARIES})
  add_test(NAME ${T_NAME} COMMAND ${T_NAME}
	  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

endforeach(T_FILE ${TESTS})
