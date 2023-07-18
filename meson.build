project(
  'mm',
  'cpp',
  default_options : [
    'warning_level=everything',
    'cpp_std=gnu++20',
  ]
)

benchmark_proj = subproject('google-benchmark')
benchmark_dep = benchmark_proj.get_variable('google_benchmark_dep')

executable(
  'mm',
  'mm.cpp',
  dependencies : [
    benchmark_dep,
  ],
)

gtest_proj = subproject('gtest')
gtest_dep = benchmark_proj.get_variable('gtest_main_dep')

executable(
  'test-mm',
  'mm_test.cpp',
  dependencies : [
    gtest_dep,
  ],
)
