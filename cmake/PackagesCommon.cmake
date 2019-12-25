#
# Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
#

SET_DEPS(NAME "ASIO"  REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC  SHARED  INCLUDE asio.hpp)

SET_DEPS(NAME "ATOMIC"  REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libatomic.a SHARED atomic INCLUDE )

SET_DEPS(NAME "ZLIB"  REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libz.a SHARED z INCLUDE zlib.h)
#INSTALL_LIBS(lib ${ZLIB_LIBRARIES_SHARED})

SET_DEPS(NAME "BZIP2" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libbz2.a SHARED bz2 INCLUDE bzlib.h)
#INSTALL_LIBS(lib ${BZIP2_LIBRARIES_SHARED})

SET_DEPS(NAME "LZMA"  REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC liblzma.a SHARED lzma INCLUDE lzma.h)
#INSTALL_LIBS(lib ${LZMA_LIBRARIES_SHARED})

SET_DEPS(NAME "ZSTD"  REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libzstd.a SHARED zstd INCLUDE zstd.h)
#INSTALL_LIBS(lib ${ZSTD_LIBRARIES_SHARED})

SET_DEPS(NAME "SNAPPY" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libsnappy.a SHARED snappy INCLUDE snappy.h)
#INSTALL_LIBS(lib ${SNAPPY_LIBRARIES_SHARED})

SET_DEPS(NAME "LZ4" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC liblz4.a SHARED lz4 INCLUDE lz4.h)
#INSTALL_LIBS(lib ${LZ4_LIBRARIES_SHARED})

SET_DEPS(NAME "RE2" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libre2.a SHARED re2 INCLUDE re2/re2.h)
#INSTALL_LIBS(lib ${RE2_LIBRARIES_SHARED})

SET_DEPS(
	NAME "SSL" 
	REQUIRED TRUE 
	LIB_PATHS /usr/local/ssl/lib
	INC_PATHS /usr/local/ssl/include
	STATIC libssl.a libcrypto.a 
	SHARED ssl crypto
	INCLUDE openssl/ssl.h openssl/crypto.h
)
#INSTALL_LIBS(lib ${SSL_LIBRARIES_SHARED})


find_package(FileSystems REQUIRED)

SET_DEPS(NAME "EDITLINE" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libeditline.a SHARED editline INCLUDE editline.h)
#SET_DEPS(NAME "NCURSESW" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libncursesw.a SHARED ncursesw INCLUDE ncurses.h)




if(FALSE)
SET_DEPS(NAME "EXPAT" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libexpat.a SHARED expat INCLUDE expat.h)
# INSTALL_LIBS(lib ${EXPAT_LIBRARIES_SHARED})

SET_DEPS(NAME "EVENT" REQUIRED TRUE LIB_PATHS "" INC_PATHS "" STATIC libevent.a SHARED event INCLUDE event.h)
#INSTALL_LIBS(lib ${EVENT_LIBRARIES_SHARED})
SET_DEPS(NAME "SSP" REQUIRED TRUE LIB_PATHS "" INC_PATHS "/usr/local/lib/gcc/x86_64-ubuntu-linux-gnu/8.3.0/include" STATIC libssp.a SHARED ssp INCLUDE ssp/ssp.h)

endif()


# find_package(Doxygen)

# find_package(Thrift REQUIRED)

# find_package(Libssh REQUIRED)
