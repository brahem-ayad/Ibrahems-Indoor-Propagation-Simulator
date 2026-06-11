# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-src")
  file(MAKE_DIRECTORY "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-src")
endif()
file(MAKE_DIRECTORY
  "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-build"
  "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-subbuild/cdt-populate-prefix"
  "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-subbuild/cdt-populate-prefix/tmp"
  "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-subbuild/cdt-populate-prefix/src/cdt-populate-stamp"
  "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-subbuild/cdt-populate-prefix/src"
  "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-subbuild/cdt-populate-prefix/src/cdt-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-subbuild/cdt-populate-prefix/src/cdt-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/br4hem/Programming/Indoor_Propagation_Simulator/build/_deps/cdt-subbuild/cdt-populate-prefix/src/cdt-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
