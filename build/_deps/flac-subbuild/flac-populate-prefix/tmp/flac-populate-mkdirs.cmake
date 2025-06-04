# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "E:/Project/MarioG/build/_deps/flac-src")
  file(MAKE_DIRECTORY "E:/Project/MarioG/build/_deps/flac-src")
endif()
file(MAKE_DIRECTORY
  "E:/Project/MarioG/build/_deps/flac-build"
  "E:/Project/MarioG/build/_deps/flac-subbuild/flac-populate-prefix"
  "E:/Project/MarioG/build/_deps/flac-subbuild/flac-populate-prefix/tmp"
  "E:/Project/MarioG/build/_deps/flac-subbuild/flac-populate-prefix/src/flac-populate-stamp"
  "E:/Project/MarioG/build/_deps/flac-subbuild/flac-populate-prefix/src"
  "E:/Project/MarioG/build/_deps/flac-subbuild/flac-populate-prefix/src/flac-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/Project/MarioG/build/_deps/flac-subbuild/flac-populate-prefix/src/flac-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/Project/MarioG/build/_deps/flac-subbuild/flac-populate-prefix/src/flac-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
