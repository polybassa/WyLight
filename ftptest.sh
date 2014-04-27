SERVER=$1
FILENAME=wifly7-441.mif
HOST=localhost
PORT=60021

rm -f $FILENAME
$SERVER &
sleep 1
ftp -n -p $HOST $PORT <<SCRIPT
user roving Pass123
type binary
get $FILENAME
SCRIPT
killall $SERVER
diff $FILENAME ./rn171_fw/$FILENAME
if [ $? != 0 ] ; then
	echo "          ./library/FtpServer_ut.bin run  1 Tests ( 0 skipped |  1 errors)"
	rm -f $FILENAME
	exit 1
fi
echo "          ./library/FtpServer_ut.bin run  1 Tests ( 0 skipped |  0 errors)"
rm -f $FILENAME
