convert -resize 512x512 google_play/${1}.png google_play/${1}_highres.png
convert -resize 180x120 google_play/${1}.png google_play/${1}_commercial.png
sh convert_drawable.sh ${1}
