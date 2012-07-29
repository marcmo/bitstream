cxx_configuration do
  source_lib "gtest",
    :sources => FileList['gtest/gtest-all.cc'],
    :includes => ['.','/usr/include'],
    :dependencies => [BinaryLibrary.new("pthread")]
  source_lib "gtest_main",
    :sources => FileList['gtest/gtest_main.cc'],
    :includes => ['.','/usr/include'],
    :dependencies => ["gtest"]
end
