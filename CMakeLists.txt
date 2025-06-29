﻿
# Cmake的命令不区分打下写，例如message，set等命令；但Cmake的变量区分大小写
# 为统一风格，本项目的Cmake命令全部采用小写，变量全部采用大写加下划线组合。
# DA WorkBench 顶层Cmake文件
# 在运行次CMakeLists.txt之前，请确保3rdparty已经运行并进行了install，3rdparty位于/src/3rdparty/CMakeLists.txt
# 请先单独编译第三方库后，安装，并把安装目录设置到和此安装目录一致

#3.16是qt6要求的最低版本
cmake_minimum_required(VERSION 3.12)

########################################################
# 版本定义
########################################################
set(DA_VERSION_MAJOR 0)
set(DA_VERSION_MINOR 0)
set(DA_VERSION_PATCH 2)
set(DA_VERSION "${DA_VERSION_MAJOR}.${DA_VERSION_MINOR}.${DA_VERSION_PATCH}")
string(TIMESTAMP DA_COMPILE_DATETIME %y%m%d)
string(TIMESTAMP DA_COMPILE_DATETIME_YEAR %y)
string(TIMESTAMP DA_COMPILE_DATETIME_MONTH %m)
string(TIMESTAMP DA_COMPILE_DATETIME_DAY %d)
message(STATUS "DA Version is ${DA_VERSION}")
########################################################
# 定义工程
########################################################
set(DA_PROJECT_NAME "DAWorkbench")

project(${DA_PROJECT_NAME}
        VERSION ${DA_VERSION}
        LANGUAGES CXX
        DESCRIPTION "DataWorkBench : A Workflow Integration Workbench"
        )
########################################################
# include
########################################################

include(${CMAKE_CURRENT_LIST_DIR}/cmake/daworkbench_utils.cmake)
########################################################
# 可选的变量
########################################################
# 此选项将设置是否开启python
option(DA_ENABLE_PYTHON
    "enable python|if ON will add DAPythonBind DAPyScript DAPyCommonWidgets COMPONENTS"
    ON)
option(DA_ENABLE_AUTO_INSTALL_PYTHON_ENV
    "This parameter allows cmake to automatically search for the Python environment and copy the necessary DLLs from the Python environment to the bin directory without manual deployment. It is recommended to enable this option for Windows users who have not set the Python directory as a system environment variable"
    ON)
# 此选项将自动调用Linguist工具对翻译文件进行翻译
option(DA_ENABLE_AUTO_TRANSLATE
    "This option will automatically call the Linguist tool to translate the ts file"
    ON)
# 此选项将自动调用Linguist工具对翻译文件进行update,此参数建议仅对翻译时开启
option(DA_ENABLE_UPDATE_TRANSLATE
    "This option will automatically call the Linguist tool to update ts file"
    OFF)
# 此选项将让da自动把结果安装到本地目录下
option(DA_AUTO_INSTALL_PREFIX
    "This option will allow DA to automatically install the results to the local directory"
    ON)
# 此选项将让da自动生成DAConfig.h文件，此选项仅库开发者需要设置为ON，库使用者默认应该为OFF
option(DA_AUTO_GENERATE_CONFIG_INFO
    "This option will allow DA to automatically generate DAConfig.h, This option only needs to be set to ON by library developers, and library users should default to OFF"
    OFF)
# 此选项将让da自动生成构建plugin
option(DA_BUILD_PLUGINS
    "This option will build plugin"
    ON)
if(DA_AUTO_INSTALL_PREFIX)
    message(STATUS "DA Auto Install")
    set(DA_BIN_DIR_NAME)
    dafun_set_bin_name(DA_BIN_DIR_NAME)
    set(DAWorkbench_INSTALL_DIR "${CMAKE_CURRENT_LIST_DIR}/${DA_BIN_DIR_NAME}")
    set(CMAKE_INSTALL_PREFIX ${DAWorkbench_INSTALL_DIR} CACHE PATH "Installation prefix" FORCE)
    message(STATUS "DA_BIN_DIR_NAME=${DA_BIN_DIR_NAME}")
    message(STATUS "DAWorkbench_INSTALL_DIR=${DAWorkbench_INSTALL_DIR}")
endif()



########################################################
# 一些常规设置
########################################################


set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

# c++标准，要求为17
set(CMAKE_CXX_STANDARD 17)
# 强制要求
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)
########################################################
# MSVC设置
########################################################
if(MSVC)
# msvc utf-8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    # make sure __cplusplus is defined when using msvc and enable parallel build
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")

    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)
    set(CMAKE_CXX_RESPONSE_FILE_PREFIX "@")
    set(CMAKE_CXX_RESPONSE_FILE_EXTENSION ".rsp")
endif()

if(CMAKE_GENERATOR STREQUAL "Ninja")
    # 针对Ninja生成器
    set(CMAKE_NINJA_FORCE_RESPONSE_FILE "1")
endif()
########################################################
# MSVC设置
# 如果不添加会出现
# Qt5AxContainerd.lib(qaxbase.obj) : error LNK2038  MDd_DynamicDebug MTd_StaticDebug等提示
# Qt5AxBased.lib(qaxtypefunctions.obj) : error LNK2038:
# 可以通过下面方式设置
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
########################################################
if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MD>
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MD>
    )
endif()
########################################################
# Qt
########################################################
set(DA_MIN_QT_VERSION 5.14)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)


########################################################
# 默认路径设置
########################################################
set(DA_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(DA_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(DA_INSTALL_LIB_CMAKE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake)
set(DA_INSTALL_LIB_SHARE_PATH ${CMAKE_INSTALL_PREFIX}/share/cmake)
list(APPEND CMAKE_MODULE_PATH ${DA_CMAKE_DIR} ${DA_INSTALL_LIB_CMAKE_PATH})

########################################################
# 定义第三方库路径
########################################################
set(SARibbonBar_DIR ${DA_INSTALL_LIB_CMAKE_PATH}/SARibbonBar)
message(STATUS "SARibbonBar_DIR=${SARibbonBar_DIR}")

set(DALiteCtk_DIR ${DA_INSTALL_LIB_CMAKE_PATH}/DALiteCtk)
message(STATUS "DALiteCtk_DIR=${DALiteCtk_DIR}")

set(qwt_DIR ${DA_INSTALL_LIB_CMAKE_PATH}/qwt)
message(STATUS "qwt_DIR=${qwt_DIR}")

set(QtPropertyBrowser_DIR ${DA_INSTALL_LIB_CMAKE_PATH}/QtPropertyBrowser)
message(STATUS "QtPropertyBrowser_DIR=${QtPropertyBrowser_DIR}")

set(spdlog_DIR ${DA_INSTALL_LIB_CMAKE_PATH}/spdlog)
message(STATUS "spdlog_DIR=${spdlog_DIR}")

# 注意tsl-ordered-map的安装位置在share/cmake,而不是lib/cmake
set(tsl-ordered-map_DIR ${DA_INSTALL_LIB_SHARE_PATH}/tsl-ordered-map)
message(STATUS "tsl-ordered-map_DIR=${tsl-ordered-map_DIR}")

set(qt${QT_VERSION_MAJOR}advanceddocking_DIR  ${DA_INSTALL_LIB_CMAKE_PATH}/qt${QT_VERSION_MAJOR}advanceddocking)
message(STATUS "qt${QT_VERSION_MAJOR}advanceddocking_DIR=${qt${QT_VERSION_MAJOR}advanceddocking_DIR}")

##################################
# 初步安装 install 通用内容
##################################
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "${DA_PROJECT_NAME} install dir is : ${CMAKE_INSTALL_PREFIX}")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${DA_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${DA_PROJECT_NAME}
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION lib/cmake/${DA_PROJECT_NAME}
)
########################################################
# 引入用到的通用函数和宏
########################################################
include(${CMAKE_CURRENT_LIST_DIR}/cmake/daworkbench_utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/daworkbench_3rdparty.cmake)


########################################################
# 自动化生成配置头文件
########################################################
if(DA_AUTO_UPDATE_CONFIG_INFO)
    configure_file (
      "${CMAKE_CURRENT_LIST_DIR}/src/DAConfigs.h.in"
      "${CMAKE_CURRENT_LIST_DIR}/src/DAConfigs.h"
    )
endif()
#DAGlobal.h
install(FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/DAGlobals.h
    DESTINATION include/${DA_PROJECT_NAME}
    COMPONENT headers
)
#DAConfigs.h
install(FILES
        ${CMAKE_CURRENT_LIST_DIR}/src/DAConfigs.h
        DESTINATION include/${DA_PROJECT_NAME}
        COMPONENT headers
 )
#DAShared
# install(DIRECTORY
#         ${CMAKE_CURRENT_LIST_DIR}/src/DAShared
#         DESTINATION include/${DA_PROJECT_NAME}
#         COMPONENT headers
#  )
########################################################
# 添加预定义宏NOMINMAX,windows系统会定义max和min宏，这个宏和std::max冲突,因此在高版本的c++，应该取消minmax宏
########################################################
if(WIN32)
    ADD_COMPILE_DEFINITIONS(NOMINMAX)
endif()


########################################################
# 源码路径
########################################################
add_subdirectory(src)

# 插件
if(DA_BUILD_PLUGINS)
    add_subdirectory(plugins)
endif()

##################################
# 最终安装 install
##################################
if(DA_ENABLE_PYTHON)
    # 把PyScripts文件夹复制过去
    install(DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}/src/PyScripts
        DESTINATION bin
    )
    # 为了能调试的时候运行，需要把src/PyScripts文件夹复制到${CMAKE_CURRENT_BINARY_DIR}/bin
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/src/PyScripts" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
    message(STATUS "COPY ${CMAKE_CURRENT_LIST_DIR}/src/PyScripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin")
    if(DA_ENABLE_AUTO_INSTALL_PYTHON_ENV)
        # 如果使用的是非系统目录下的 Python 可以通过指定 Python3_ROOT_DIR 改变查找路径
        find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
        if(${Python3_FOUND})
            if(WIN32)
                set(DA_PYTHON_DLL_PATH ${Python3_RUNTIME_LIBRARY_DIRS}/python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}.dll)
                # 把dll复制到bin中否则无法运行
                file(COPY ${DA_PYTHON_DLL_PATH} DESTINATION ${CMAKE_BINARY_DIR}/bin)
                install(FILES
                        ${DA_PYTHON_DLL_PATH}
                        DESTINATION
                        bin
                 )
            endif()
        endif()
    endif()
endif()


install(FILES
    "${CMAKE_CURRENT_LIST_DIR}/cmake/daworkbench_plugin_utils.cmake"
    "${CMAKE_CURRENT_LIST_DIR}/cmake/create_win32_resource_version.cmake"
    DESTINATION lib/cmake/${DA_PROJECT_NAME}
)

##################################
# 翻译
##################################
if(DA_ENABLE_AUTO_TRANSLATE)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS LinguistTools REQUIRED)
    # 设置输出目录
    set(DA_QM_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/translations)
    file(MAKE_DIRECTORY ${DA_QM_OUTPUT_DIR})
    # 定义翻译文件
    set(DA_TS_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/da_zh_CN.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/src/da_en_US.ts
    )
    if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/da_zh_CN.ts)
        # 如果翻译文件不在src目录，说明有可能被clean的时候删除了，把备份ts文件拷贝到src目录下
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_LIST_DIR}/src/da_zh_CN.ts
                   ${CMAKE_CURRENT_LIST_DIR}/src/da_en_US.ts
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_LIST_DIR}/src/ts/da_zh_CN.ts
                ${CMAKE_CURRENT_LIST_DIR}/src/
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_LIST_DIR}/src/ts/da_en_US.ts
                ${CMAKE_CURRENT_LIST_DIR}/src/
            COMMENT "Copying translation files from backup"
            VERBATIM
        )
    endif()

    # 自动生成 qm 文件
    if(QT_VERSION_MAJOR EQUAL 5)
        # Qt5需要手动处理输出路径
        foreach(TS_FILE IN LISTS DA_TS_FILES)
            get_filename_component(TS_NAME ${TS_FILE} NAME_WE)
            set(QM_FILE ${DA_QM_OUTPUT_DIR}/${TS_NAME}.qm)
            add_custom_command(
                OUTPUT ${QM_FILE}
                COMMAND ${Qt5_LRELEASE_EXECUTABLE} ${TS_FILE} -qm ${QM_FILE}
                DEPENDS ${TS_FILE}
                COMMENT "Generating ${QM_FILE}"
            )
            list(APPEND DA_QM_FILES ${QM_FILE})
        endforeach()
        # qt5_add_translation(DA_QM_FILES ${DA_TS_FILES})
    else()
        qt6_add_translation(DA_QM_FILES ${DA_TS_FILES} OUTPUT_DIRECTORY ${DA_QM_OUTPUT_DIR})
    endif()

    # 创建翻译目标
    add_custom_target(da_release_translations ALL DEPENDS ${DA_QM_FILES})

    # 安装翻译文件
    install(FILES ${DA_QM_FILES}
        DESTINATION bin/translations  # 明确目标路径
        COMPONENT translations
    )

    # 更新翻译目标
    if(DA_ENABLE_UPDATE_TRANSLATE)
        message(STATUS "update translate")
        add_custom_target(da_update_translations
            COMMAND ${Qt${QT_VERSION_MAJOR}_LUPDATE_EXECUTABLE}
                -locations relative
                -no-obsolete
                ${CMAKE_CURRENT_SOURCE_DIR}/src
                -ts ${DA_TS_FILES}
            COMMENT "Updating translations"
            VERBATIM
        )
    endif()
endif()
