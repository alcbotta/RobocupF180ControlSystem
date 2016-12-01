cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(libs)

set(QT_USE_QTNETWORK TRUE)
find_package(Qt REQUIRED)
include(${QT_USE_FILE})
list(APPEND libs ${QT_LIBRARIES})


find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
list(APPEND libs ${PROTOBUF_LIBRARIES})

protobuf_generate_cpp(PROTO_CPP PROTO_H
    ../grSim-master/src/proto/grSim_Replacement.proto
    ../grSim-master/src/proto/grSim_Commands.proto
    ../grSim-master/src/proto/grSim_Packet.proto
    ../grSim-master/src/proto/messages_robocup_ssl_detection.proto
    ../grSim-master/src/proto/messages_robocup_ssl_geometry.proto
    ../grSim-master/src/proto/messages_robocup_ssl_wrapper.proto
    ../grSim-master/src/proto/messages_robocup_ssl_refbox_log.proto
    ../grSim-master/src/proto/grSim_Replacement.proto
    ../grSim-master/src/proto/grSim_Commands.proto
    ../grSim-master/src/proto/grSim_Packet.proto
    ../ssl-refbox-master/referee.proto
    ../ssl-refbox-master/savestate.proto
)




set(HEADERS
        ./src/fuzzy/FIE.h
	./src/fuzzy/flie.h
	./src/fuzzy/fuzzfier.h
	./src/fuzzy/fuzzy_control.h
	./src/fuzzy/fuzzy_obstacle.h
	./src/fuzzy/lingvar.h
	./src/fuzzy/motor.h
        ./src/fuzzy/rule.h

        ./src/old/MVector.h
        ./src/old/UtilGame.h
        ./src/old/Point.h
        ./src/old/Element.h
        ./src/old/CommunicationSystem.h
        ./src/old/ComSysSimulator.h
        ./src/old/Ball.h
        ./src/old/Game.h
        ./src/old/Robot.h
        ./src/old/RobotBehavior.h
        ./src/old/RobotBehaviorGoalkeaper.h
        ./src/old/Strategy.h

        ./src/GameMainWindow.h
)

set(SOURCES
        ./src/fuzzy/FIE.cc
	./src/fuzzy/flie.cc
	./src/fuzzy/fuzzfier.cc
	./src/fuzzy/fuzzy_control.cc
	./src/fuzzy/fuzzy_obstacle.cc
	./src/fuzzy/lingvar.cc
	./src/fuzzy/motor.cc
        ./src/fuzzy/rule.cc

        ./src/old/MVector.cpp
        ./src/old/UtilGame.cpp
        ./src/old/Point.cpp
        ./src/old/Element.cpp
        ./src/old/CommunicationSystem.cpp
        ./src/old/ComSysSimulator.cpp
        ./src/old/Ball.cpp
        ./src/old/Game.cpp
        ./src/old/GameFunctions.cpp
        ./src/old/Robot.cpp
        ./src/old/RobotBehavior.cpp
        ./src/old/RobotBehaviorGoalkeaper.cpp
        ./src/old/Strategy.cpp

        ./src/GameMainWindow.cpp

	Main.cpp
)

#set(app ControlSystem)

set(srcs
    ${CONFIG_FILES}
    ${PROTO_CPP}
    ${PROTO_H}
    ${RESOURCES}
    ${HEADERS}
    ${SOURCES}
)

set(app ControlSystem)
add_executable(${app} ${srcs})
target_link_libraries(${app} ${libs})
#install(TARGETS ${app} DESTINATION bin)
#install(DIRECTORY config DESTINATION share/${app})
#install(FILES resources/grsim.desktop DESTINATION share/applications)
#install(FILES resources/icons/grsim.svg DESTINATION share/icons/hicolor/scalable/apps)

