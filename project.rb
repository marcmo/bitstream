cxx_configuration do
  exe "bitstream",
    :includes => ['gtest','/usr/include'],
    :sources => FileList['**/Bit*.cpp'],
    :dependencies => ['gtest_main']
end
