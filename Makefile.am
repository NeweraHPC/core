SUBDIRS = src

bin_PROGRAMS             = neweraHPC_server
neweraHPC_server_SOURCES = neweraHPC_server.cpp
neweraHPC_server_LDADD   = src/libneweraHPC_core.la -ldl -lpthread

library_includedir      = $(includedir)/neweraHPC
library_include_HEADERS = include/*.h

dist_doc_DATA = README
