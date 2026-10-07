foreach(CTRL controller2)
  add_library(pipuck_${CTRL} MODULE ${CTRL}.hpp ${CTRL}.cpp)

  target_link_libraries(pipuck_${CTRL}
    PUBLIC
      argos3core_${ARGOS_BUILD_FOR}
      argos3plugin_${ARGOS_BUILD_FOR}_pipuck
  )

  target_include_directories(pipuck_${CTRL}
    PUBLIC
      ${ARGOS_INCLUDE_DIRS}
  )
endforeach()
