cxx_configuration do
  exe "bitstream",
    :includes => ['lib'],
    :sources => FileList['test/**/*.cpp','lib/**/*.cpp'],
    :dependencies => []
end
