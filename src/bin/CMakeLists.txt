file(GLOB EXES "*.c")
foreach(EXE_FILE ${EXES})
  get_filename_component(EXE_NAME ${EXE_FILE} NAME_WE)
  add_executable(${EXE_NAME} ${EXE_FILE})
  target_link_libraries(${EXE_NAME} ${PROJECT_LIBS} ${THIRDPARTY_LIBRARIES})
endforeach(EXE_FILE ${EXES})
