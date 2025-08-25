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
  'src/utils/dynamic-array.c'
]

dependencies = [
]

exe = executable(
  'my-lisp',
  sources,
  dependencies : dependencies,
  install : true,
)

test('basic', exe)
