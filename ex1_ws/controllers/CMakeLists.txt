set(SRC
  controller1.hpp
  controller1.cpp
  # Add other controller source files here
)

add_library(pipuck_controller1 MODULE ${SRC})

target_link_libraries(pipuck_controller1
  PUBLIC
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck

)

target_include_directories(pipuck_controller1
  PUBLIC
    ${ARGOS_INCLUDE_DIRS}
)

set_target_properties(pipuck_controller1 PROPERTIES
  OUTPUT_NAME pipuck_controller1
)
