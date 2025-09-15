project('binchecker-lib', 'c',
          version: '0.1.0',
    meson_version: '>= 1.0.0',
  default_options: [ 'warning_level=2', 'werror=false', 'c_std=gnu11', ],
)

libbindif_sources = ['src/libbindiff.c']
libbindif_inc = include_directories('src') 


libbindif = shared_library(
    'bindiff',              
    libbindif_sources,
    include_directories: libbindif_inc,
    install: true,         
    install_dir:'/usr/lib'
)

install_headers('src/libbindiff.h', subdir: 'libbindiff')
