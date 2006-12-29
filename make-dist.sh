#!/bin/sh

MAKE=make

UIM_REPOSITORY="http://anonsvn.freedesktop.org/svn/uim"
TAGS_REPOSITORY="${UIM_REPOSITORY}/tags"
#SSCM_REPOSITORY="${UIM_REPOSITORY}/sigscheme-trunk"
SSCM_REPOSITORY="${TAGS_REPOSITORY}/sigscheme-0.7.2"
#LIBGCROOTS_URL="${UIM_REPOSITORY}/libgcroots-trunk"
LIBGCROOTS_URL="${TAGS_REPOSITORY}/libgcroots-0.1.3"

svn export $LIBGCROOTS_URL libgcroots
(cd libgcroots && ./autogen.sh) && ./autogen.sh
./configure --enable-maintainer-mode --enable-conf=full
$MAKE distcheck sum