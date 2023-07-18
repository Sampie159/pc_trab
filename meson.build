project(
  'mm',
  'cpp',
  default_options : [
    'warning_level=everything',
    'cpp_std=gnu++20',
  ]
)

benchmark_dep = dependency('benchmark')

executable(
  'mm',
  'mm.cpp',
  dependencies : [
    benchmark_dep,
  ],
)

gtest_dep = dependency('gtest_main')

executable(
  'test-mm',
  'mm_test.cpp',
  dependencies : [
    gtest_dep,
  ],
)
