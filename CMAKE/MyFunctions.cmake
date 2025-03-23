
#显示目录中的文件
function(show_files  file_list  description)
        list(LENGTH file_list len)
        message("show_files:" ${description}  " files:${len}")
        FOREACH(file_path ${file_list})
           message(STATUS ${file_path})
        ENDFOREACH()
endfunction()
#show_files("${DES_INCLUDE}" "DES_INCLUDE")

#提取文件名（无扩展名）
function(get_file_name file_path out_file_name)
   message("get_file_name input:"  ${file_path} )
   STRING(REGEX REPLACE  ".+/(.+)\\..*"  "\\1"  file_name ${file_path})
   message("get_file_name output:" ${file_name})   
   set(${out_file_name} ${file_name} PARENT_SCOPE) 
endfunction()

#提取完整文件名（含扩展名）
function(get_file_name_full file_path out_file_name)
   message("get_file_name_full input:"  ${file_path} )
   STRING(REGEX REPLACE  ".+/(.+\\..*)"  "\\1"  file_name_full ${file_path})
   message("get_file_name_full output:" ${file_name_full})   
   set(${out_file_name} ${file_name_full} PARENT_SCOPE) 
endfunction()

#提取目录路径
function(get_file_dir file_path out_file_name)
   message("get_file_dir input:"  ${file_path} )
   STRING(REGEX REPLACE  "(.+)/.+\\..*"  "\\1"  file_dir ${file_path})
   message("get_file_dir output:" ${file_dir})   
   set(${out_file_name} ${file_dir} PARENT_SCOPE) 
endfunction()


function(FindLibraries DIRECTORY PATTERN OUTPUT_VARIABLE)
    # 使用 GLOB_RECURSE 查找符合 PATTERN 的文件
    file(GLOB_RECURSE FOUND_FILES "${DIRECTORY}/${PATTERN}")
    
    # 将结果设置为输出变量
    set(${OUTPUT_VARIABLE} ${FOUND_FILES} PARENT_SCOPE)

    # 打印找到的文件（可选）
    if(FOUND_FILES)
        #message(STATUS "找到以下符合条件的文件：")
        #foreach(FILE ${FOUND_FILES})
        #    message("${FILE}")
        #endforeach()
    else()
        message(WARNING "未找到符合条件的文件。")
    endif()
endfunction()
#FindLibraries(${BOOST_LIBRARY} "libboost_*-vc[0-9]*-mt-gd-x64-*.lib" BOOST_MATCHED_DEBUG_SO)\
#FindLibraries(${BOOST_LIBRARY} "libboost_*-vc[0-9]*-mt-x64-*.lib" BOOST_MATCHED_RELEASE_SO)

#于打印变量中所有文件以及文件总个数
function(PrintFiles VARIABLE_NAME DESCRIPTION)
    #message(STATUS "Debug: VARIABLE_NAME=${VARIABLE_NAME}")
    #message(STATUS "Debug: DESCRIPTION=${DESCRIPTION}")
    # 获取变量的值
    set(FILES ${${VARIABLE_NAME}})
    # 获取文件总个数
    list(LENGTH FILES FILE_COUNT)
    # 打印描述信息和文件总个数
    message(STATUS "PrintFiles  ${DESCRIPTION} 文件总个数: ${FILE_COUNT}")
    # 遍历并打印每个文件
    foreach(FILE ${FILES})
        message(STATUS "文件: ${FILE}")
    endforeach()
endfunction()
#PrintFiles(DES_INCLUDE "描述")

function(GET_LIB_NAMES INPUT_LIBS OUTPUT_VAR)
    set(_func_temp_names "")  # 临时存储结果
    foreach(LIB IN LISTS INPUT_LIBS)
        get_filename_component(NAME "${LIB}" NAME)  # 提取文件名
        list(APPEND _func_temp_names "${NAME}")
    endforeach()
    set(${OUTPUT_VAR} "${_func_temp_names}" PARENT_SCOPE)  # 传递结果到父作用域
endfunction()


# 定义宏：根据文件路径自动生成组名
macro(auto_source_group SOURCE_FILES)
    foreach(FILE ${SOURCE_FILES})
        # 获取文件相对路径
        file(RELATIVE_PATH REL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${FILE})
        # 替换路径分隔符为 CMake 组名分隔符（Windows用\\，Unix用/）
        string(REPLACE "/" "\\\\" GROUP_NAME "${REL_PATH}")
        # 提取目录部分（去掉文件名）
        get_filename_component(GROUP_NAME "${GROUP_NAME}" PATH)
        # 创建组
        source_group("${GROUP_NAME}" FILES ${FILE})
    endforeach()
endmacro()

# 使用示例
#file(GLOB_RECURSE ALL_SOURCES "src/*.cpp" "include/*.h")
#auto_source_group("${ALL_SOURCES}")


# 将多配置检测逻辑封装为函数
# 用法: detect_multi_config([OUTPUT_VAR])
# 可选参数 OUTPUT_VAR 指定结果变量名（默认 IS_MULTI_CONFIG）
function(detect_multi_config)
    # 处理可选参数
    set(output_var "IS_MULTI_CONFIG")  # 默认输出变量名
    if(ARGC GREATER 0)
        set(output_var "${ARGV0}")      # 允许自定义变量名
    endif()

    # 判断逻辑
    if(NOT DEFINED CMAKE_CONFIGURATION_TYPES AND DEFINED CMAKE_BUILD_TYPE)
        message(STATUS "当前为单配置生成器，构建类型: ${CMAKE_BUILD_TYPE}")
        set(is_multi FALSE)
    elseif(DEFINED CMAKE_CONFIGURATION_TYPES)
        message(STATUS "当前为多配置生成器，支持配置: ${CMAKE_CONFIGURATION_TYPES}")
        set(is_multi TRUE)
    elseif(CMAKE_GENERATOR MATCHES "Visual Studio|Xcode|Ninja Multi-Config")
        message(STATUS "当前为多配置生成器（通过生成器名称匹配）")
        set(is_multi TRUE)
    else()
        message(STATUS "当前为单配置生成器，构建类型: ${CMAKE_BUILD_TYPE}")
        set(is_multi FALSE)
    endif()

    # 将结果传递到父作用域
    set(${output_var} ${is_multi} PARENT_SCOPE)
endfunction()

# 调用函数（使用默认变量名 IS_MULTI_CONFIG）
#detect_multi_config()
# 或自定义输出变量名
#detect_multi_config("MY_MULTI_CONFIG_FLAG")
