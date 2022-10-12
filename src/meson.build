dependencies = [
    dependency('gtkmm-3.0'),
    dependency('taglib'),
    dependency('threads'),
]

# Apparently deadbeef can't be found with pkg-conf
add_project_arguments('-I\usr\include\deadbeef', language: 'c')

source_files = files(
    'plugin.cpp',
    'filebrowser.cpp',
    'utils.cpp',
    'cache/covers/cover.cpp',
    'cache/covers/coverfactory.cpp',
    'cache/covers/implementation/coverfirst.cpp',
    'cache/covers/implementation/metadatafirst.cpp',
    'cache/covers/implementation/metadataonly.cpp',
    'gui/dispatcherbridge.cpp',
    'gui/modelcolumns.cpp',
    'gui/container.cpp',
    'gui/searchbar.cpp',
    'gui/addressbox.cpp',
    'gui/fbtreemodel.cpp',
    'gui/fbtreefilter.cpp',
    'gui/fbtreepopup.cpp',
    'gui/views/fbtreeview.cpp',
    'plugin/controller.cpp',
    'plugin/settings.cpp',
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
    include_directories('../include/cache'),
    include_directories('../include/cache/covers'),
    include_directories('../include/cache/covers/implementation'),
    include_directories('../include/gui'), 
    include_directories('../include/gui/views'), 
    include_directories('../include/plugin'), 
    include_directories('../include/metadata'), 
    include_directories('../include/metadata/abstract'),
    include_directories('../include/metadata/implementation'),
]

version = meson.project_version()
version = version.split('.')

lib = shared_library(
    'ddb_misc_filebrowser_reborn',
    include_directories: includes,
    dependencies: dependencies,
    install: true,
    sources: source_files,
    cpp_args: [
        '-DVMAJOR=@0@'.format(version[0]), 
        '-DVMINOR=@0@'.format(version[1]),
    ],
)