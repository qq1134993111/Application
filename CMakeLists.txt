cmake_minimum_required(VERSION 3.10.0)
#CMAKE_BUILD_TYPE    Debug  Release  RelWithDebInfo  MinSizeRel
#cmake ..  -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release  -DCMAKE_INSTALL_PREFIX=. 
##cmake ..  -G"Visual Studio 15 2017 Win64" -A x64  -DCMAKE_BUILD_TYPE=Release  -DCMAKE_INSTALL_PREFIX=D:/tmp  -DCMAKE_CONFIGURATION_TYPES="Release;Debug;RelWithDebInfo;MinSizeRel"  -DCMAKE_SYSTEM_VERSION=10.0
##cmake --build . --config Release --target Install  编译并且安装，后面可以指定--prefix /custom/install/path
##cmake --build . -- VERBOSE=1

#多生成器
#cmake --build . -j 4 --config Release  # 指定 Release 编译
#cmake --build . -j 4 --config Debug    # 指定 Debug 编译
#单生成器
#cmake --build . -j 4

#安装
#cmake --install . 
#cmake --install . [--prefix <安装路径>] --config <Debug/Release>

#清理编译
#cmake --build . --target clean  # 通用命令

# 引入函数文件
include("${CMAKE_SOURCE_DIR}/CMAKE/MyFunctions.cmake")

project(MyApp)

message(STATUS "PROJECT_NAME "  ${PROJECT_NAME})
message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message(STATUS "源码目录: ${CMAKE_SOURCE_DIR}")
message(STATUS "生成目录: ${CMAKE_BINARY_DIR}")
message(STATUS "当前源码目录: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "当前生成目录: ${CMAKE_CURRENT_BINARY_DIR}")

message(STATUS "CMAKE_INCLUDE_PATH " ${CMAKE_INCLUDE_PATH})
message(STATUS "CMAKE_LIBRARY_PATH " ${CMAKE_LIBRARY_PATH})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "CMAKE_CURRENT_BINARY_DIR " ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "CMAKE_MODULE_PATH  " ${CMAKE_MODULE_PATH})
message(STATUS "EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})
message(STATUS "LIBRARY_OUTPUT_PATH  " ${LIBRARY_OUTPUT_PATH})
message(STATUS "CMAKE_INCLUDE_PATH " ${CMAKE_INCLUDE_PATH})
message(STATUS "CMAKE_SYSTEM  " ${CMAKE_SYSTEM})
message(STATUS "CAMKE_SYSTEM_NAME " ${CAMKE_SYSTEM_NAME})
message(STATUS "CMAKE_SYSTEM_VERSION " ${CMAKE_SYSTEM_VERSION})
message(STATUS "CMAKE_SYSTEM_PROCESSOR  " ${CMAKE_SYSTEM_PROCESSOR})


if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Building for Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("Building for Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("Building for macOS")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Using GCC compiler")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message("Using Clang compiler")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message("Using MSVC compiler")
endif()

if(MSVC)
    message(STATUS "VC 编译器版本: ${MSVC_VERSION}")
endif()
message(STATUS "C++ 编译器版本: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ 编译器路径: ${CMAKE_CXX_COMPILER}")
message(STATUS "C   编译器路径: ${CMAKE_C_COMPILER}")


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
    message(STATUS "CMAKE_BUILD_TYPE is not defined, defaulting to Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Current build type is Debug")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Current build type is Release")
else()
    message(STATUS "Current build type is ${CMAKE_BUILD_TYPE}")
endif()




# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # 禁用编译器扩展，确保标准符合性

file(GLOB_RECURSE SOURCES "*.cpp" "*.cxx" "*.c" "*.h" "*.hpp" "*.hxx" "*.h++")
# 过滤掉包含下面路径的文件
list(FILTER SOURCES EXCLUDE REGEX ".*pugixml/docs/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*spdlog/example/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*spdlog/src/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*whereami/example/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*cppregex/example.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*pystring/test.cpp|.*pystring/unittest.h")
# 将 CMAKE_BINARY_DIR 转换为正则表达式格式
string(REPLACE "\\" "/" BIN_DIR_REGEX "${CMAKE_BINARY_DIR}")
# 过滤掉 ${CMAKE_BINARY_DIR} 目录中的文件
list(FILTER SOURCES EXCLUDE REGEX ".*${BIN_DIR_REGEX}/.*")
list(FILTER SOURCES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/tests/*.*")


#分组
# 过滤出所有头源文件（*.cpp" "*.cxx" "*.c"）
set(GROUP_SOURCE  ${SOURCES})
list(FILTER GROUP_SOURCE INCLUDE REGEX ".*\\.(cpp|cxx|c)$") 
source_group(TREE ${CMAKE_SOURCE_DIR}    # 指定根目录
    PREFIX "Source Files"        # 统一前缀（可选）
    FILES ${GROUP_SOURCE}         # 需要分组的文件列表
)

# 过滤出所有头文件（.h/.hpp/.hxx）
set(GROUP_INCLUDE  ${SOURCES})
list(FILTER GROUP_INCLUDE INCLUDE REGEX ".*\\.(h|hpp|hxx)$") 
source_group(TREE ${CMAKE_SOURCE_DIR}
    PREFIX "Header Files"
    FILES ${GROUP_INCLUDE}
)


#include(GNUInstallDirs)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#if(CMAKE_CONFIGURATION_TYPES)
#  foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
#    string(TOUPPER ${config} config_upper)
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${config})
#    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${config})
#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${config})
#  endforeach()
#endif()


set(THIRD_PARTY_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/3rd)
set(SPDLOG_INCLUDE ${THIRD_PARTY_LIBRARY_DIR}/spdlog/include)
set(WHEREAMI_INCLUDE  ${THIRD_PARTY_LIBRARY_DIR}/whereami)

# 获取环境变量 BOOST_ROOT
if(DEFINED ENV{BOOST_ROOT})
    if("$ENV{BOOST_ROOT}" STREQUAL "")
        message(WARNING "环境变量 BOOST_ROOT 已定义但为空")
    else()
        message("环境变量 BOOST_ROOT 已定义 " $ENV{BOOST_ROOT})
        set(BOOST_ROOT $ENV{BOOST_ROOT})
    endif()
else()
    message(WARNING "环境变量 BOOST_ROOT 未定义")
endif()

# 获取环境变量 BOOST_INCLUDE
if(DEFINED ENV{BOOST_INCLUDE})
    if("$ENV{BOOST_INCLUDE}" STREQUAL "")
        message(WARNING "环境变量 BOOST_INCLUDE 已定义但为空")
    else()
        message("环境变量 BOOST_INCLUDE 已定义 " $ENV{BOOST_INCLUDE})
        set(BOOST_INCLUDE $ENV{BOOST_INCLUDE})
    endif()
else()
    message(WARNING "环境变量 BOOST_INCLUDE 未定义")
endif()

# 获取环境变量 BOOST_LIBRARY
if(DEFINED ENV{BOOST_LIBRARY})
    if("$ENV{BOOST_LIBRARY}" STREQUAL "")
        message(WARNING "环境变量 BOOST_LIBRARY 已定义但为空")
    else()
        message("环境变量 BOOST_LIBRARY 已定义 " $ENV{BOOST_LIBRARY})
        set(BOOST_LIBRARY $ENV{BOOST_LIBRARY})
    endif()
else()
    message(WARNING "环境变量 BOOST_LIBRARY 未定义")
endif()

# 检查是否定义了 BOOST_ROOT 或 BOOST_INCLUDE
if(NOT DEFINED BOOST_ROOT AND NOT DEFINED BOOST_INCLUDE)
    message(FATAL_ERROR "Neither BOOST_ROOT nor BOOST_INCLUDE is defined. Please define one of them.")
endif()

# 如果定义了 BOOST_ROOT 但没有定义 BOOST_INCLUDE
if(DEFINED BOOST_ROOT AND NOT DEFINED BOOST_INCLUDE)
    set(BOOST_INCLUDE ${BOOST_ROOT}) # 使用 BOOST_ROOT 的值设置 BOOST_INCLUDE
    message(STATUS "BOOST_INCLUDE is not defined. Using BOOST_ROOT (${BOOST_ROOT}) as BOOST_INCLUDE.")
endif()

# 检查是否定义了 BOOST_ROOT 和 BOOST_LIBRARY
if(NOT DEFINED BOOST_ROOT AND NOT DEFINED BOOST_LIBRARY)
    message(FATAL_ERROR "Neither BOOST_ROOT nor BOOST_LIBRARY is defined. Please define at least one of them.")
endif()

# 如果定义了 BOOST_ROOT 但没有定义 BOOST_LIBRARY
if(DEFINED BOOST_ROOT AND NOT DEFINED BOOST_LIBRARY)
      # 检测目标架构
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 检测 64 位
        set(BOOST_LIBRARY "${BOOST_ROOT}/stage/lib64")
        message(STATUS "Detected 64-bit architecture. Setting BOOST_LIBRARY to '${BOOST_ROOT}/stage/lib64'.")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 检测 32 位
        set(BOOST_LIBRARY "${BOOST_ROOT}/stage/lib32")
        message(STATUS "Detected 32-bit architecture. Setting BOOST_LIBRARY to '${BOOST_ROOT}/stage/lib32'.")
    else()
        message(FATAL_ERROR "Unknown architecture. Unable to set BOOST_LIBRARY.")
    endif()
endif()

# 输出设置的值（可选，用于调试）
message(STATUS "BOOST_ROOT: ${BOOST_ROOT}")
message(STATUS "BOOST_INCLUDE: ${BOOST_INCLUDE}")
message(STATUS "BOOST_LIBRARY: ${BOOST_LIBRARY}")



if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # 调用函数，传入搜索目录、匹配规则和输出变量名
    FindLibraries(${BOOST_LIBRARY} "libboost_*.a" BOOST_MATCHED_DEBUG_SO)
    PrintFiles(BOOST_MATCHED_DEBUG_SO "boost debug libs path")
    GET_LIB_NAMES("${BOOST_MATCHED_DEBUG_SO}" BOOST_MATCHED_DEBUG_SO)
    set(BOOST_UNIT_TEST_DEBUG_SO ${BOOST_MATCHED_DEBUG_SO})
    list(FILTER BOOST_UNIT_TEST_DEBUG_SO INCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")
    #PrintFiles(BOOST_UNIT_TEST_DEBUG_SO "boost unit test libs name")
    list(FILTER BOOST_MATCHED_DEBUG_SO EXCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")


    FindLibraries(${BOOST_LIBRARY} "libboost_*.a" BOOST_MATCHED_RELEASE_SO)
    PrintFiles(BOOST_MATCHED_RELEASE_SO "boost release libs path")
    GET_LIB_NAMES("${BOOST_MATCHED_RELEASE_SO}" BOOST_MATCHED_RELEASE_SO)
    #PrintFiles(BOOST_MATCHED_RELEASE_SO "boost release libs")
    set(BOOST_UNIT_TEST_RELEASE_SO ${BOOST_MATCHED_RELEASE_SO})
    list(FILTER BOOST_UNIT_TEST_RELEASE_SO INCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")
    #PrintFiles(BOOST_UNIT_TEST_RELEASE_SO "boost unit test libs name")
    list(FILTER BOOST_MATCHED_RELEASE_SO EXCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")    
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # 调用函数，传入搜索目录、匹配规则和输出变量名
    FindLibraries(${BOOST_LIBRARY} "libboost_*-vc[0-9]*-mt-gd-x64-*.lib" BOOST_MATCHED_DEBUG_SO)
    PrintFiles(BOOST_MATCHED_DEBUG_SO "boost debug libs path")
    GET_LIB_NAMES("${BOOST_MATCHED_DEBUG_SO}" BOOST_MATCHED_DEBUG_SO)
    set(BOOST_UNIT_TEST_DEBUG_SO ${BOOST_MATCHED_DEBUG_SO})
    list(FILTER BOOST_UNIT_TEST_DEBUG_SO INCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")
    #PrintFiles(BOOST_UNIT_TEST_DEBUG_SO "boost unit test libs name")
    list(FILTER BOOST_MATCHED_DEBUG_SO EXCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")



    FindLibraries(${BOOST_LIBRARY} "libboost_*-vc[0-9]*-mt-x64-*.lib" BOOST_MATCHED_RELEASE_SO)
    PrintFiles(BOOST_MATCHED_RELEASE_SO "boost release libs path")
    GET_LIB_NAMES("${BOOST_MATCHED_RELEASE_SO}" BOOST_MATCHED_RELEASE_SO)
    #PrintFiles(BOOST_MATCHED_RELEASE_SO "boost release libs")
    set(BOOST_UNIT_TEST_RELEASE_SO ${BOOST_MATCHED_RELEASE_SO})
    list(FILTER BOOST_UNIT_TEST_RELEASE_SO INCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")
    #PrintFiles(BOOST_UNIT_TEST_RELEASE_SO "boost unit test libs name")
    list(FILTER BOOST_MATCHED_RELEASE_SO EXCLUDE REGEX ".*unit_test_framework*|.*prg_exec_monitor*|.*test_exec_monitor*")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()


set(Application_INCLUDE ${PROJECT_SOURCE_DIR}/src)


add_executable(${PROJECT_NAME} ${SOURCES})

# 针对特定文件覆盖设置（确保在 add_executable 之后）
set_source_files_properties(
    ${THIRD_PARTY_LIBRARY_DIR}/whereami/whereami.c
    PROPERTIES
    #手动添加标志
    COMPILE_FLAGS "-std=c99"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_LIBRARY_DIR}  ${SPDLOG_INCLUDE} ${WHEREAMI_INCLUDE}
${BOOST_INCLUDE}
${Application_INCLUDE} ${Application_INCLUDE}/base)


#target_link_directories(${PROJECT_NAME}
#  PRIVATE
#    $<$<CONFIG:Debug>:${BOOST_LIBRARY}>
#    $<$<CONFIG:Release>:${BOOST_LIBRARY}>
#)

target_link_directories(${PROJECT_NAME}
  PRIVATE  ${BOOST_LIBRARY}
)

#target_link_libraries(${PROJECT_NAME} PRIVATE
#debug ${BOOST_MATCHED_DEBUG_SO}
#optimized  ${BOOST_MATCHED_RELEASE_SO}
#)

#target_link_libraries(${PROJECT_NAME}
#    PRIVATE
#      $<$<CONFIG:Debug>:${BOOST_MATCHED_DEBUG_SO}>
#      $<$<CONFIG:Release>:${BOOST_MATCHED_RELEASE_SO}>
#)

#target_link_libraries(${PROJECT_NAME}
#  PRIVATE
#    $<$<CONFIG:Debug>:${BOOST_MATCHED_DEBUG_SO}>
#    $<$<OR:
#        $<CONFIG:Release>,
#        $<CONFIG:RelWithDebInfo>,
#        $<CONFIG:MinSizeRel>
#      >:${BOOST_MATCHED_RELEASE_SO}>
#)


target_link_libraries(${PROJECT_NAME}
  PRIVATE
    $<$<CONFIG:Debug>:${BOOST_MATCHED_DEBUG_SO}>         # Debug 使用 DEBUG 库
    $<$<NOT:$<CONFIG:Debug>>:${BOOST_MATCHED_RELEASE_SO}> # 其他配置均使用 RELEASE 库
)

message(STATUS "当前生成器: ${CMAKE_GENERATOR}")

# 检测是否用户未指定安装前缀
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "默认安装路径" FORCE)
endif()


detect_multi_config()

if(IS_MULTI_CONFIG)
  message(STATUS "当前为多配置环境，需指定 --config 参数")
  include(GNUInstallDirs)
  foreach(config Debug Release RelWithDebInfo  MinSizeRel)
  string(TOLOWER ${config} config_lower)
  set(INSTALL_BIN_DIR_${config} ${CMAKE_INSTALL_PREFIX}/${config_lower}/${CMAKE_INSTALL_BINDIR})
  set(INSTALL_LIB_DIR_${config} ${CMAKE_INSTALL_PREFIX}/${config_lower}/${CMAKE_INSTALL_LIBDIR})
  
  install(TARGETS ${PROJECT_NAME}
    CONFIGURATIONS ${config}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR_${config}}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR_${config}}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR_${config}}
  )
  endforeach()
else()
  message(STATUS "当前为单配置环境，使用 CMAKE_BUILD_TYPE")
  # 根据构建类型（Debug/Release）动态生成子目录
  if(CMAKE_BUILD_TYPE)
    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type_dir)
    set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/${build_type_dir}/bin)
    set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/${build_type_dir}/lib)
  else()
    set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
    set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  endif()

  # 安装规则示例
  install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  )

endif()


## 安装头文件到 include 目录
#install(FILES
#    "include/header1.h"
#    "include/header2.h"
#    DESTINATION include
#)
#
#install(FILES
#    include/my_header.h
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/myproject  # 目标路径：默认 <prefix>/include/myproject
#)
#
#
##找到文件安装
#file(GLOB API_INSTALL ${ApiPath}/include/api/*.h)
#install(FILES ${API_INSTALL}  DESTINATION include/api)   
#
## 安装配置文件到 etc 目录
#install(FILES
#    "config/project.conf"
#    "config/settings.conf"
#    DESTINATION etc
#)
#
##‌静态配置文件‌
#install(FILES
#    config/settings.conf
#    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/myproject  # 目标路径：默认 <prefix>/etc/myproject
#)
#
##‌生成后配置文件（使用 configure_file）
#configure_file(
#    ${CMAKE_CURRENT_SOURCE_DIR}/config/settings.conf.in
#    ${CMAKE_CURRENT_BINARY_DIR}/generated/settings.conf
#    @ONLY
#)
#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/generated/settings.conf
#    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/myproject
#)
#
#install(PROGRAMS  # 注意用 PROGRAMS 而非 FILES，以保留可执行权限
#    scripts/myscript.sh
#    DESTINATION ${CMAKE_INSTALL_BINDIR}  # 目标路径：默认 <prefix>/bin
#)
#
## 安装脚本到 bin 目录，带自定义权限的脚本
#install(FILES
#    "scripts/script1.sh"
#    "scripts/script2.py"
#    DESTINATION ${CMAKE_INSTALL_BINDIR}
#    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ  # 设置权限：用户可读、写、执行
#)
#
#
## 安装资源目录. PATTERN 参数：可以用来过滤需要安装的文件类型
#install(DIRECTORY "resources/"
#    DESTINATION resources
#    FILES_MATCHING PATTERN "*.txt" PATTERN "*.json"
#)
#
##安装整个目录（保留目录结构）
#install(DIRECTORY
#    include/
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/myproject
#    PATTERN "*.tmp" EXCLUDE  # 排除临时文件
#)

##编译静态库和动态库
#add_library(my_lib_static STATIC src/lib1.cpp src/lib2.cpp)
#add_library(my_lib_shared SHARED src/lib1.cpp src/lib2.cpp)
## 可选：设置不同的输出名称，避免冲突
#set_target_properties(my_lib_static PROPERTIES OUTPUT_NAME "my_lib")
#set_target_properties(my_lib_shared PROPERTIES OUTPUT_NAME "my_lib")
##为动态库添加版本号 输出可能是libmy_lib.so.1.0.0和符号链接libmy_lib.so.1
#set_target_properties(my_lib PROPERTIES
#    VERSION 1.0.0
#    SOVERSION 1
#)
## 为库添加别名，格式为 Namespace::Target   这样使用 target_link_libraries(my_app PRIVATE MyNamespace::my_lib)
#add_library(MyNamespace::my_lib ALIAS my_lib)
#set_target_properties(my_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)  # 确保动态库的 PIC 标志
#target_compile_options(mylib PRIVATE -fvisibility=hidden)  #通过 -fvisibility=hidden 控制符号导出（仅 Linux/macOS）
#target_compile_definitions(mylib PRIVATE MYLIB_EXPORTS) #‌Windows 动态库导出宏 定义 __declspec(dllexport) 以导出符号

###############单元测试
option(BUILD_TESTS "Enable test compilation" OFF)  # 默认关闭测试
message(STATUS "BUILD_TESTS = ${BUILD_TESTS}") 
if (BUILD_TESTS)
    message(STATUS "Test mode activated. Test cases will be compiled.")  
    #这一行通常放在项目的顶层 CMakeLists.txt 文件中，表明该项目支持测试功能。
    enable_testing()
    
    ##############################################################################################################################################################
    add_executable(my_test tests/my_test.cpp)
    #target_link_libraries(my_test PRIVATE MyLibrary)  # 链接被测库
    # 设置覆盖率编译标志
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(my_test PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_libraries(my_test PRIVATE --coverage)
    endif()
    
    
    # 注册基础测试
    add_test(NAME BasicTest COMMAND my_test --arg1 value1 --arg2 value2
     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}  # 运行目录（可选）
    )
    
    # 在测试中添加库路径  #export LD_LIBRARY_PATH=/usr/local/boost/lib:$LD_LIBRARY_PATH
    set_tests_properties(BasicTest PROPERTIES
        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/lib"
    )
    
    set_tests_properties(BasicTest PROPERTIES TIMEOUT 10)  # 10秒后超时失败
    
    #ctest -R BasicTest  # -R 按名称筛选测试
    #ctest -C Release --output-on-failure   --verbose 多配置生成器（如 Visual Studio）‌ 指定构建类型运行测试
    
    ####################################################################################################################################################################
    # 链接boost 单元测试 
    add_executable(test_case1 tests/test_case1.cpp)
    target_include_directories(test_case1 PRIVATE ${THIRD_PARTY_LIBRARY_DIR}  ${SPDLOG_INCLUDE} ${WHEREAMI_INCLUDE}
    ${BOOST_INCLUDE}
    ${Application_INCLUDE} ${Application_INCLUDE}/base)
    
    target_link_directories(test_case1
      PRIVATE  ${BOOST_LIBRARY}
    )
    
    target_link_libraries(test_case1
      PRIVATE
        $<$<CONFIG:Debug>:${BOOST_MATCHED_DEBUG_SO} ${BOOST_UNIT_TEST_DEBUG_SO} >         # Debug 使用 DEBUG 库
        $<$<NOT:$<CONFIG:Debug>>:${BOOST_MATCHED_RELEASE_SO} ${BOOST_UNIT_TEST_RELEASE_SO} > # 其他配置均使用 RELEASE 库
    )
    
    # 设置覆盖率编译标志
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(test_case1 PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_libraries(test_case1 PRIVATE --coverage)
    endif()
    
    
    # 注册基础测试
    add_test(NAME TestCase1 COMMAND test_case1 --run_test=test_case1,test_case2,*
     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}  # 运行目录（可选）
    )
    
    # 在测试中添加库路径  #export LD_LIBRARY_PATH=/usr/local/boost/lib:$LD_LIBRARY_PATH
    set_tests_properties(TestCase1 PROPERTIES
        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/lib"
    )
    
    set_tests_properties(TestCase1 PROPERTIES TIMEOUT 10)  # 10秒后超时失败
    
    #ctest -R TestCase1  # -R 按名称筛选测试
    #ctest -C Release --output-on-failure   --verbose 多配置生成器（如 Visual Studio）‌ 指定构建类型运行测试
    
    ########################################################################################################################################################################
    # 不链接boost 单元测试 
    add_executable(test_case2 tests/test_case2.cpp)
    target_include_directories(test_case2 PRIVATE ${THIRD_PARTY_LIBRARY_DIR}  ${SPDLOG_INCLUDE} ${WHEREAMI_INCLUDE}
    ${BOOST_INCLUDE}
    ${Application_INCLUDE} ${Application_INCLUDE}/base)
    
    target_link_directories(test_case2
      PRIVATE  ${BOOST_LIBRARY}
    )
    
    target_link_libraries(test_case2
      PRIVATE
        $<$<CONFIG:Debug>:${BOOST_MATCHED_DEBUG_SO} >         # Debug 使用 DEBUG 库
        $<$<NOT:$<CONFIG:Debug>>:${BOOST_MATCHED_RELEASE_SO} > # 其他配置均使用 RELEASE 库
    )
    
    # 设置覆盖率编译标志
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(test_case2 PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_libraries(test_case2 PRIVATE --coverage)
    endif()
    
    
    # 注册基础测试
    add_test(NAME TestCase2 COMMAND test_case2
     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}  # 运行目录（可选）
    )
    
    # 在测试中添加库路径  #export LD_LIBRARY_PATH=/usr/local/boost/lib:$LD_LIBRARY_PATH
    set_tests_properties(TestCase2 PROPERTIES
        ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/lib"
    )
    
    set_tests_properties(TestCase2 PROPERTIES TIMEOUT 10)  # 10秒后超时失败
    
    #ctest -R TestCase2  # -R 按名称筛选测试
    #ctest -C Release --output-on-failure   --verbose 多配置生成器（如 Visual Studio）‌ 指定构建类型运行测试
endif()
