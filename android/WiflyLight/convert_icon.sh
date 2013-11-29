convert -resize 512x512 google_play/${1}.png google_play/${1}_highres.png
convert -resize 180x120 google_play/${1}.png google_play/${1}_commercial.png
convert -resize 96x96 google_play/${1}.png res/drawable-xhdpi/${1}.png
convert -resize 72x72 google_play/${1}.png res/drawable-hdpi/${1}.png
convert -resize 48x48 google_play/${1}.png res/drawable-mdpi/${1}.png
convert -resize 36x36 google_play/${1}.png res/drawable-ldpi/${1}.png
