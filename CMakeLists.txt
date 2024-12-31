cmake_minimum_required(VERSION 3.10)
project(IRViewer)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

find_package(LLVM REQUIRED CONFIG HINTS ${LLVM_DIR} ${LLVM_DIR}/lib/cmake/llvm
	                NO_DEFAULT_PATH)
include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS} SYSTEM)
link_directories(${LLVM_LIBRARY_DIRS})
set(LLVM_LINK_COMPONENTS
  LLVMCore
  LLVMIRReader
  LLVMPasses
  )

set(irviewer_SRC
  src/main.cpp
  src/irviewer.cpp
  src/irviewer.h
  src/irparser.cpp
  src/irparser.h
)

# Tell CMake to create the helloworld executable
add_executable(irviewer ${irviewer_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(irviewer Qt5::Widgets ${LLVM_LINK_COMPONENTS})

# Install the executable
install(TARGETS irviewer DESTINATION bin)
