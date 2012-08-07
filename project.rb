cxx_configuration do
  exe "bitstream",
    :includes => ['gtest','/usr/include','lib'],
    :sources => FileList['test/**/*.cpp'],
    :dependencies => ['gtest_main']
end
