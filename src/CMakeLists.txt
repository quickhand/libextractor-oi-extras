#this is just a basic CMakeLists.txt, for more information see the cmake manpage

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)

#build a shared library
ADD_LIBRARY(fb2plug3 SHARED fb2plug3.cpp)

#for testing the shared library you probably need some test app too
#ADD_EXECUTABLE()

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES(fb2plug3 expat)

#add an install target here
#INSTALL_FILES(...)
#INSTALL_PROGRAMS(...)
#INSTALL_TARGET(...)

