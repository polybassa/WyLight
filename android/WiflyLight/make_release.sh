apk_name=WyLight.apk
key_store=/home/gpb/google_play.keystore
key_alias=bruenn.biz
sdk_dir=/home/gpb/android-sdk-linux/tools

# sign apk
cp ${apk_name} ${apk_name}-unsigned
jarsigner -verbose -sigalg MD5withRSA -digestalg SHA1 -keystore ${key_store} ${apk_name} ${key_alias}

# verify signed apk
jarsigner -verify -verbose -certs ${apk_name}

#zip
${sdk_dir}/zipalign -v 4 ${apk_name} signed-${apk_name}

#rename and cleanup
rm ${apk_name}
mv signed-${apk_name} ./google_play/${apk_name}
