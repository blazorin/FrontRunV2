cmake_minimum_required(VERSION 3.16)
project(FrontRunV2)

set(CMAKE_CXX_STANDARD 17)

add_executable(FrontRunV2 main.cpp Controllers/Http/Http.h Controllers/Http/Http.cpp Structs/Structs.h Controllers/Cache/Config.h Controllers/Cache/Config.cpp
        Controllers/WebSocket/WebSocket.h Controllers/WebSocket/WebSocket.cpp Utils/Common/PolyHex.h Utils/Common/PolyHex.cpp
        lib/rest-cpp/src/ChunkedReaderImpl.cpp lib/rest-cpp/src/ChunkedWriterImpl.cpp lib/rest-cpp/src/ConnectionImpl.h lib/rest-cpp/src/ConnectionPoolImpl.cpp lib/rest-cpp/src/DataReaderStream.cpp lib/rest-cpp/src/IoReaderImpl.cpp lib/rest-cpp/src/IoWriterImpl.cpp lib/rest-cpp/src/NoBodyReaderImpl.cpp lib/rest-cpp/src/PlainReaderImpl.cpp lib/rest-cpp/src/PlainWriterImpl.cpp lib/rest-cpp/src/ReplyImpl.cpp lib/rest-cpp/src/ReplyImpl.h lib/rest-cpp/src/RequestBodyFileImpl.cpp lib/rest-cpp/src/RequestBodyStringImpl.cpp lib/rest-cpp/src/RequestImpl.cpp lib/rest-cpp/src/RestClientImpl.cpp lib/rest-cpp/src/SocketImpl.h lib/rest-cpp/src/TlsSocketImpl.h lib/rest-cpp/src/TlsSocketImpl.h lib/rest-cpp/src/Url.cpp lib/rest-cpp/src/url_encode.cpp
        lib/baseconverter/baseconverter.h lib/baseconverter/baseconverter.cpp
        lib/BigInt/BigInt.h lib/BigInt/BigInt.cpp
        lib/BigDecimal/BigDecimal.cpp lib/BigDecimal/BigDecimal.h

        Utils/Common/PolyJson.cpp Utils/Common/PolyJson.h Utils/Common/StringConverter.h Utils/Common/StringConverter.cpp)

## Struct Mapping
INCLUDE_DIRECTORIES(./mapper/include)

## Rest-CPP
INCLUDE_DIRECTORIES(./lib/rest-cpp/include)

## LRU Config
INCLUDE_DIRECTORIES(./lib/lru/in)


#Windows
if(CMAKE_HOST_WIN32)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME ON)
    find_package(Boost COMPONENTS random filesystem regex coroutine system thread)

    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        #add_executable(PolygonFrontRun main.cpp flow.cpp WebSocket.cpp)
        #target_link_libraries(PolygonFrontRun )
    endif()

    find_package(jsoncpp CONFIG REQUIRED)
    find_package(websocketpp CONFIG REQUIRED)
    find_package(OpenSSL REQUIRED)


    target_link_libraries(FrontRunV2 PRIVATE jsoncpp_lib jsoncpp_object JsonCpp::JsonCpp websocketpp::websocketpp ${Boost_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)

    #Otros
    #Añade OpenSSL
elseif(CMAKE_HOST_APPLE)
    ## JsonCPP
    INCLUDE_DIRECTORIES(/usr/local/Cellar/jsoncpp/1.9.5/include)
    LINK_DIRECTORIES(/usr/local/Cellar/jsoncpp/1.9.5/lib)

    file(GLOB LIBRARIES "/usr/local/Cellar/jsoncpp/1.9.5/lib/*.dylib")
    message("LIBRARIES = ${LIBRARIES}")


    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})

    ## Boost
    INCLUDE_DIRECTORIES(/usr/local/Cellar/boost/1.78.0_1/include)
    LINK_DIRECTORIES(/usr/local/Cellar/boost/1.78.0_1/lib)

    file(GLOB LIBRARIES "/usr/local/Cellar/boost/1.78.0_1/lib/*.dylib")
    message("LIBRARIES = ${LIBRARIES}")

    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})

    ## WebSocket++
    INCLUDE_DIRECTORIES(/usr/local/Cellar/websocketpp/0.8.2/include)
    LINK_DIRECTORIES(/usr/local/Cellar/websocketpp/0.8.2/lib)

    file(GLOB LIBRARIES "/usr/local/Cellar/websocketpp/0.8.2/lib/*.dylib")
    message("LIBRARIES = ${LIBRARIES}")

    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})

    ## OpenSSL
    INCLUDE_DIRECTORIES(/usr/local/opt/openssl@3/include)
    LINK_DIRECTORIES(/usr/local/opt/openssl@3/lib)

    file(GLOB LIBRARIES "/usr/local/opt/openssl@3/lib/*.dylib")
    message("LIBRARIES = ${LIBRARIES}")

    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})
elseif(CMAKE_HOST_UNIX)

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    ## JsonCPP
    INCLUDE_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/jsoncpp/1.9.5/include)
    LINK_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/jsoncpp/1.9.5/lib)

    file(GLOB LIBRARIES "/home/linuxbrew/.linuxbrew/Cellar/jsoncpp/1.9.5/lib/*.so")
    message("LIBRARIES = ${LIBRARIES}")


    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})

    ## Boost
    INCLUDE_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/boost/1.78.0_1/include)
    LINK_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/boost/1.78.0_1/lib)

    file(GLOB LIBRARIES "/home/linuxbrew/.linuxbrew/Cellar/boost/1.78.0_1/lib/*.so")
    message("LIBRARIES = ${LIBRARIES}")

    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})

    ## WebSocket++
    INCLUDE_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/websocketpp/0.8.2/include)
    LINK_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/websocketpp/0.8.2/lib)

    file(GLOB LIBRARIES "/home/linuxbrew/.linuxbrew/Cellar/websocketpp/0.8.2/lib/*.so")
    message("LIBRARIES = ${LIBRARIES}")

    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})


    ## OpenSSL
    INCLUDE_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/openssl@3/3.0.3/include)
    LINK_DIRECTORIES(/home/linuxbrew/.linuxbrew/Cellar/openssl@3/3.0.3/lib)

    file(GLOB LIBRARIES "/home/linuxbrew/.linuxbrew/Cellar/openssl@3/3.0.3/lib/*.so")
    message("LIBRARIES = ${LIBRARIES}")

    TARGET_LINK_LIBRARIES(FrontRunV2 ${LIBRARIES})
endif()