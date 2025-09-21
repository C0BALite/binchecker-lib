project('libbindiff', 'c',
          version: '0.1.0',
    meson_version: '>= 1.0.0',
  default_options: [ 'warning_level=2', 'werror=false', 'c_std=gnu11', ],
)
package_api_name = meson.project_name()
libbindiff_header_subdir =  package_api_name
libbindiff_sources = ['src/libbindiff.c']
libbindiff_inc = include_directories('src') 
libdir     = get_option('libdir')

libbindiff = library(
    'bindiff',              
    libbindiff_sources,
    include_directories: libbindiff_inc,
    install: true,         
    install_dir: libdir,
    soversion: 0
)

install_headers('src/libbindiff.h', subdir: libbindiff_header_subdir)

pkgg = import('pkgconfig')

pkgg.generate(
    libraries: [libbindiff],
      subdirs: libbindiff_header_subdir,
      version: meson.project_version(),
         name: 'BinDiff',
     filebase: package_api_name,
  description: 'Binary checker shared library',
  install_dir: libdir / 'pkgconfig',
)
