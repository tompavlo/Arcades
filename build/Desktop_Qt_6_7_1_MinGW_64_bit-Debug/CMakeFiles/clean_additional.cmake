# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Arcades_autogen"
  "CMakeFiles\\Arcades_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Arcades_autogen.dir\\ParseCache.txt"
  )
endif()
