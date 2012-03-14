cmake_minimum_required (VERSION 2.8)
project (opencvee)

set (PROJECT_CPP addin.cpp export.def)
set (PROJECT_HPP resource.h)
set (PROJECT_RES addin.rc)

source_group ("Resource Files" FILES ${PROJECT_RES})

add_library (${PROJECT_NAME} SHARED ${PROJECT_CPP} ${PROJECT_HPP}
  ${PROJECT_RES})
