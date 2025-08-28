project(
  'my-lisp',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)

sources = [
  'src/my_lisp.c',
  'src/token.c',
  'src/lexer.c',
  'src/utils/dynamic-array.c',
  'src/utils/byte-buffer.c'
]

test_darray_srcs = [
  'src/utils/dynamic-array.c',
  'test/test-darray.c'
]

test_bbuf_srcs = [
  'src/utils/byte-buffer.c',
  'test/test-bbuf.c'
]


dependencies = [
]

exe = executable(
  'my-lisp',
  sources,
  dependencies : dependencies,
  install : true,
)

test_darray = executable(
  'test-darray',
  test_darray_srcs,
  dependencies : dependencies,
  install : true,
)

test_bbuf = executable(
  'test-bbuf',
  test_bbuf_srcs,
  dependencies : dependencies,
  install : true,
)


test('basic', exe)
test('test-darray', test_darray)
test('test-bbuf', test_bbuf)
