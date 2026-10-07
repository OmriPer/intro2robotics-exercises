set(SRC
    loop_functions.cpp
    loop_functions.hpp
)

set(LIBS
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck
)

# The drawing needs the ARGoS Qt-OpenGL visualization.
# Without it the loop functions still build, nothing is drawn.
if(ARGOS_QTOPENGL_FOUND)
  include_directories(${ARGOS_QTOPENGL_INCLUDE_DIRS})
  list(APPEND SRC qt_user_functions.cpp qt_user_functions.hpp)
  list(APPEND LIBS argos3plugin_${ARGOS_BUILD_FOR}_qtopengl ${ARGOS_QTOPENGL_LIBRARIES})
endif()

add_library(my_loop_functions MODULE ${SRC})

target_link_libraries(my_loop_functions PUBLIC ${LIBS})

target_include_directories(my_loop_functions
  PUBLIC
    ${ARGOS_INCLUDE_DIRS}
)

set_target_properties(my_loop_functions PROPERTIES
  OUTPUT_NAME hw2_loop_functions
)
