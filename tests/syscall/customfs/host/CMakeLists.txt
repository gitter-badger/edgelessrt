# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

set(EDL_FILE ../test_customfs.edl)

add_custom_command(
  OUTPUT test_customfs_u.h test_customfs_u.c
  DEPENDS ${EDL_FILE} edger8r
  COMMAND
    edger8r --untrusted ${EDL_FILE} --search-path ${PROJECT_SOURCE_DIR}/include
    ${DEFINE_OE_SGX} --search-path ${CMAKE_CURRENT_SOURCE_DIR} --search-path
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../device/edl)

add_executable(customfs_host host.c test_customfs_u.c)

target_include_directories(customfs_host PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(customfs_host oehost)
