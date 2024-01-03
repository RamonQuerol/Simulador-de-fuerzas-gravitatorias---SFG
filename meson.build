project('sfg', 'c')

gnome = import('gnome')

sfg_resources = gnome.compile_resources('sfg_resources',
  'sfg.gresource.xml',
  source_dir: meson.current_source_dir())

executable('sfg',
  'sfgApp.c', 'sfgMainWindow.c', 'main.c', sfg_resources,
  dependencies: [
    dependency('gtk4')
  ],
  )