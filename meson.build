project('main', 'c', version: '0.1', default_options: ['warning_level=3'])

dep_gtk = dependency('gtk+-3.0')

subdir('src')

executable('main', 
    files('main.c'), 
    include_directories: includes,
    link_with: libs,
    dependencies: dep_gtk,
    install: true)