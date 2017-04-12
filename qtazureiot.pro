TEMPLATE = subdirs

SUBDIRS += \
    libqtazureiot \
    qtazureiotconsole \
    plugin \
    qmlazureiot

lib.subdir = libqtazureiot
console.subdir = qtazureiotconsole
console.depends = lib
