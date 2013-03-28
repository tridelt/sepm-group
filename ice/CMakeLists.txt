#begin snippet
#next lines are for creating .h & .cpp files out of .ice Files
set(out_dir "${CMAKE_CURRENT_BINARY_DIR}")
set(out_file "${out_dir}/SecureDistributedChat.cpp")

add_custom_command(OUTPUT
"${out_file}"
COMMAND "${ZeroCIce_slice2cpp_BIN}"
  --add-header=cstddef
  -I /usr/share/Ice/slice/
  -I /usr/share/Ice-3.4.2/slice/
  -I /usr/share/slice/
  --stream
  --output-dir "${out_dir}"
  "${CMAKE_CURRENT_SOURCE_DIR}/SecureDistributedChat.ice"
)

#end snippet


include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library (ice ${out_file})
