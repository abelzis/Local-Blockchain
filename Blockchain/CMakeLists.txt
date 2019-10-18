cmake_minimum_required(VERSION 3.12.2)

project(Blockchain)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release"
    CACHE STRING "Configuration types" FORCE)

add_executable(Blockchain
  blockchain.cpp
  block.h
  hash.h
  RandomEngine.h
  TimerClass.h
  transaction.h
  user.h
)
# Preprocessor definitions
target_compile_definitions(Blockchain PRIVATE
  $<$<CONFIG:Debug>:_UNICODE;_DEBUG;_CONSOLE>
  $<$<CONFIG:Release>:_UNICODE;NDEBUG;_CONSOLE>
)

# SDL check
target_compile_options(Blockchain PRIVATE
  "$<$<CONFIG:Debug>:/sdl>"
  "$<$<CONFIG:Release>:/sdl>"
)

# Minimal rebuild
if (MSVC)
  target_compile_options(Blockchain PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
    "$<$<CONFIG:Release>:/Gm->"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(Blockchain PRIVATE
    ""
  )
endif ()
