cxx_configuration do
  exe "bitstream",
    :includes => ['gtest','/usr/include'],
    :sources => FileList['lib/**/Bit*.cpp'],
    :dependencies => ['gtest_main']
end
