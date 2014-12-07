require 'cxx'

cpp11_flags = {
  :FLAGS => ["-std=c++11","-stdlib=libc++","-Wall"]
}

cxx(Dir['**/project.rb'], 'out', "clang", './') do
  # Provider.modify_cpp_compiler("clang", cpp11_flags)
  # Provider["clang"][:LINKER][:FLAGS] = ["-std=c++11","-stdlib=libc++"]
end

