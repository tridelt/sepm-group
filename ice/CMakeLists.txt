# add any ice files here
set(ICE_SOURCE_FILES "SecureDistributedChat")

# don't edit below
set(ICE_CPP_FILES)

foreach(ice_file ${ICE_SOURCE_FILES})

  set(out_dir "${CMAKE_CURRENT_BINARY_DIR}")
  set(out_file "${out_dir}/${ice_file}.cpp")

  add_custom_command(OUTPUT
  "${out_file}"
  COMMAND "${ZeroCIce_slice2cpp_BIN}"
    --add-header=cstddef
    -I /usr/share/Ice/slice/
    -I /usr/share/Ice-3.4.2/slice/
    -I /usr/share/slice/
    --stream
    --output-dir "${out_dir}"
    "${CMAKE_CURRENT_SOURCE_DIR}/${ice_file}.ice"
  )

  set(ICE_CPP_FILES ${ICE_CPP_FILES} ${out_file})
endforeach()


include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library (ice SHARED ${ICE_CPP_FILES})
target_link_libraries(ice ${LIBS})
