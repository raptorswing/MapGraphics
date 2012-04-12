TEMPLATE = subdirs

SUBDIRS += MapGraphics \
    TestApp

TestApp.depends += MapGraphics
