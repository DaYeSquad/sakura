cmake_minimum_required(VERSION 3.4.1)

set (main_dir ../..)
set (sakura_dir ../../sakura)
set (external_dir ${main_dir}/external)

file(GLOB common ${sakura_dir}/*.cc)
file(GLOB_RECURSE android ${sakura_dir}/android/*.cc)

add_library( sakura
             STATIC
             ${common}
             ${android})

include_directories(${main_dir}
                    ${external_dir}
                    ${external_dir}/curl/include/android/curl
                    ${external_dir}/openssl/include
                    ${external_dir}/websockets/include/android)

target_link_libraries( sakura )
