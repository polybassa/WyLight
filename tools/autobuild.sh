#!/bin/bash
# check arguments
if [ $# -eq 0 ] || ( [ "$1" != "nightly" ] && [ "$1" != "build" ] ); then
	echo -e "Usage:\n\t$0 nightly -> preserve build results"
	echo -e "or\n\t$0 build   -> delete build results\n"
    exit 1
fi

SENDMAIL=/usr/sbin/sendmail

BUILD_TYPE=$1
BUILD_BRANCH=${BUILD_TYPE}-$(date +%Y%m%d%H%M%S)
PULL_BRANCH=Master
BINDIR=exe
MAIL_HEADER="to: admin@wylight.de\nfrom: build01.wylight@gmail.com\nsubject: ${BUILD_BRANCH}"

# prepare a fresh branch for build
cd WyLight && \
git checkout ${PULL_BRANCH} > buildlog.txt && \
git pull >> buildlog.txt && \
git checkout -b ${BUILD_BRANCH} >> buildlog.txt && \
./configure >> buildlog.txt
if [ $? -ne 0 ]; then
	echo "prepare repository and branch for build failed" >> buildlog.txt
	echo -e "${MAIL_HEADER} prepare repository failed\n" > buildmsg.txt
	cat buildlog.txt >> buildmsg.txt
	cat buildmsg.txt | ${SENDMAIL} -t
	exit 1
fi

# run tests
# TODO replace this with 'make all' when supported on raspi
make firmware_test >> buildlog.txt && \
make cli >>  buildlog.txt && \
make firmware_pic >> buildlog.txt

if [ $? -ne 0 ]; then
	echo "build failed!" >> buildlog.txt
	echo -e "${MAIL_HEADER} failed\n" > buildmsg.txt
	cat buildlog.txt >> buildmsg.txt
	cat buildmsg.txt | ${SENDMAIL} -t
	exit $?
fi

echo "build was successful!" >> buildlog.txt
echo -e "${MAIL_HEADER} was successful\n" > buildmsg.txt
cat buildlog.txt >> buildmsg.txt
cat buildmsg.txt | ${SENDMAIL} -t


# save nightly build results only
if [ "${BUILD_TYPE}" == "nightly" ]; then
    git add -f ${BINDIR}/*.bin && \
    git commit -m "${BUILD_BRANCH} was successful"
else
    git checkout .
    git checkout ${PULL_BRANCH}
    git branch -D ${BUILD_BRANCH}
fi
