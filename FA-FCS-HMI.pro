TEMPLATE = subdirs
SUBDIRS +=  src \
            tests

tests.depends = src

OTHER_FILES += fa-hmi.sample.conf \
               .log4qt.sample.properties
