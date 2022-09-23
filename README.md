# Deadbeef Filebrowser Reborn

This plugin adds a very simple filebrowser widget into deadbeef, it's based on the original [filebrowser plugin](https://gitlab.com/zykure/deadbeef-fb) and would never be created if it wasn't for the amazing people creating the original plugin! If you've been using the original plugin, you'll feel at home with this one!

## Motivation

The original project is dead for quite some time and there's no alternative. It's hard to pick up the original project as the source code is not documented well enough, contains many backwards compatible patches and is basically just one massive file containing all the logic. This project aims to bring a solid starting base for a new filebrowser plugin. The aims of this project are to provide less bloated user interface, better performance and maintain decent level of code readability.

## Key differences

- Code readability > Performance > Features
- Better performance than the original plugin
- Less settings and overall less bloated UI
- Improved stability
- Support only for GTK3 and Linux

Beware of loops in your filesystem, plugin doesn't like them! :)

## Instalation

Feel free to [download the plugin](https://github.com/duzda/deadbeef-filebrowser-reborn/releases) and put inside of your 
``` ~/.local/lib/deadbeef/ ```
then you should be able to create a new widget called 'File Browser' via design menu. If you would instead prefer to compile the plugin youself, download the source code and run

``` meson build --buildtype release && cd build && ninja && ../quick-install.sh ```

To uninstall the plugin, you can remove the .so file from ``` ~/.local/lib/deadbeef/ ``` or run ``` quick-remove.sh ```.

## Contribution

Before making any feature contributions, please try to ask, as this project aims to be minimalistic.