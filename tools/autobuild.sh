#!/bin/bash
SENDMAIL=/usr/sbin/sendmail

BUILD_TYPE=$1
BUILD_BRANCH=${BUILD_TYPE}-$(date +%Y%m%d%H%M%S)
PULL_BRANCH=Master
BINDIR=exe
MAIL_HEADER="to: admin@wylight.de\nfrom: build01.wylight@gmail.com\nsubject: ${BUILD_BRANCH}"
LOGFILE=~/buildlog.txt
LOGFILERESULTS=~/buildresults.txt
TARGETS=(test cc_fw firmware_pic cli cc_bl sim library)


function make_targets() {
	# run tests
	local retvalue=1
	for target in "${TARGETS[@]}"
	do
		echo -e "\n\n Start to build $target \n---------------------------------------\n" >>$LOGFILE
		make $target >> $LOGFILE 2>> $LOGFILE

		if [ $? -ne 0 ]; then
			echo -e "\nBUILD of $target FAILED!!! \n---------------------------------------" >>$LOGFILE
			retvalue=0
		else
			echo -e "$target	BUILD SUCCESSFUL!!! \n" >> $LOGFILERESULTS
		fi
	done
	return $(retvalue)
}

function report_result() {
	echo $1 >> $LOGFILE
	echo -e "${MAIL_HEADER} $1\n\n" > buildmsg.txt
	cat $LOGFILERESULTS >> buildmsg.txt
	cat $LOGFILE >> buildmsg.txt
	cat buildmsg.txt | ${SENDMAIL} -t
	rm buildmsg.txt
	cat $LOGFILE
}

# check arguments
if [ $# -eq 0 ] || ( [ "$1" != "nightly" ] && [ "$1" != "build" ] ); then
	echo -e "Usage:\n\t$0 nightly -> preserve build results"
	echo -e "or\n\t$0 build   -> delete build results\n"
    exit 1
fi

rm $LOGFILE
rm $LOGFILERESULTS

# prepare a fresh branch for build
cd WyLight >> $LOGFILE 2>> $LOGFILE && \
git checkout ${PULL_BRANCH} >> $LOGFILE 2>> $LOGFILE && \
git pull >> $LOGFILE 2>> $LOGFILE && \
git checkout -b ${BUILD_BRANCH} >> $LOGFILE 2>> $LOGFILE && \
echo -e "\n\nSTART CONFIGURATION\n\n--------------------\n\n" >> $LOGFILE &&\
./configure >> $LOGFILE 2>> $LOGFILE

if [ $? -ne 0 ]; then
	report_result "prepare repository and branch for build failed"
	exit 1
fi

if make_targets; then
	report_result "build failed!"
	exit 1
fi

report_result "build was successful!"

# save nightly build results only
if [ "${BUILD_TYPE}" == "nightly" ]; then
    git add -f ${BINDIR}/*.bin >> $LOGFILE 2>&1
    git commit -m "${BUILD_BRANCH} was successful" >> $LOGFILE 2>&1
else
    git checkout . >> $LOGFILE 2>&1
    git checkout ${PULL_BRANCH} >> $LOGFILE 2>&1
    git branch -D ${BUILD_BRANCH} >> $LOGFILE 2>&1
fi
