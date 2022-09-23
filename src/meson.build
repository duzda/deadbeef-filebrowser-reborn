dependencies = [
    dependency('gtkmm-3.0'),
]

# Apparently deadbeef can't be found with pkg-conf
add_project_arguments('-I\usr\include\deadbeef', language: 'c')

source_files = files(
    'plugin.cpp',
    'filebrowser.cpp',
    'utils.cpp',
    'gui/modelcolumns.cpp',
    'gui/container.cpp',
    'gui/treefilebrowser.cpp',
    'gui/searchbar.cpp',
    'gui/filebrowserfilter.cpp',
    'gui/addressbox.cpp',
    'gui/treepopup.cpp',
    'plugin/controller.cpp',
)

lib = shared_library(
    'ddb_misc_filebrowser_reborn',
    include_directories: [include_directories('../include'), include_directories('../include/gui'), include_directories('../include/plugin')],
    dependencies: dependencies,
    install: true,
    sources: source_files,
)