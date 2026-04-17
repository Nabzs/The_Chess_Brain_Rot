# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-src")
  file(MAKE_DIRECTORY "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-build"
  "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-subbuild/tgui-populate-prefix"
  "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-subbuild/tgui-populate-prefix/tmp"
  "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp"
  "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-subbuild/tgui-populate-prefix/src"
  "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Nabil/Desktop/chess/Chess-projet-main/Chess-projet-main/proba/bld/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
