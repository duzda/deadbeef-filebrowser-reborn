dependencies = [
    dependency('gtkmm-3.0'),
    dependency('taglib')
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
    'metadata/readerfactory.cpp',
    'metadata/reader.cpp',
    'metadata/abstract/id3v2reader.cpp',
    'metadata/implementation/flacreader.cpp',
    'metadata/implementation/mp3reader.cpp',
    'metadata/implementation/oggreader.cpp',
    'metadata/implementation/wavreader.cpp',
)

includes = [
    include_directories('../include'), 
    include_directories('../include/gui'), 
    include_directories('../include/plugin'), 
    include_directories('../include/metadata'), 
    include_directories('../include/metadata/abstract'),
    include_directories('../include/metadata/implementation'),
]

lib = shared_library(
    'ddb_misc_filebrowser_reborn',
    include_directories: includes,
    dependencies: dependencies,
    install: true,
    sources: source_files,
)