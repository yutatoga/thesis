#DIctionaly
#ジャンルの数---length(table(iml[,2]))

iml = read.table('iml.txt', header = F, sep = ",")
#convert string to POSIXlt
time = strptime(iml[,1],"%Y-%m-%d %H:%M:%S")
#make data frame which consists of POXIlt and string about genre
iml = data.frame(time, iml[,2])
#convert UTC to JST
iml[,1] = iml[,1]+9*60*60
#ヒストグラム-経時
quartz()
hist(iml[,1], breaks=48, main="History", col = rgb(217, 217 , 233,  maxColorValue = 255), xlab = "Time", ylab = "Frequency")

#axis(1, at= seq(0, nb, by = 1))

#ヒストグラム-時間
quartz()
hist(as.POSIXlt(iml[,1])$hour, breaks=seq(0,23,1), main="24 hours", col = rgb(217, 217 , 233,  maxColorValue = 255), xlab = "Hour", ylab = "Frequency")

#曜日ごとに色を変え重畳表示
weekMax = 0#まずは曜日に分けたときの、ヒストグラムの最大値を求める
for(i in 1:7){
	if (weekMax < max(table(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour))){
		weekMax = max(table(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour))
	}
}
quartz()
for(i in 1:7){ 
	if(i>1){
		hist(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$min/60, breaks=seq(0,24,1),main="Week", col = hsv(1*i/7, 1 , 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", add = T, ylim =c(0,weekMax))
	}else{
		hist(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$min/60, breaks=seq(0,24,1),main="Week", col = hsv(1*i/7, 1 , 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", add = F, ylim =c(0,weekMax))
		#axis(1, at= seq(0, 24, by = 1))
	}
}
#曜日ごとに分けて表示
for(i in 1:7){ 
		quartz()
		wdayName = switch(i, "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" )
		hist(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$min/60, breaks=seq(0,24,1), main=wdayName, col = hsv(1*i/7, 1 , 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", add = F,  ylim =c(0,weekMax))
		#axis(1, at= seq(0, 24, by = 1), cex.axis = 0.5)	
}
#ジャンルを分けて分析
#とりあえず、最大の度数と次点のジャンルを比較分析する。
#first
largestGenreName = names(table(iml[,2])[table(iml[,2]) == max(table(iml[,2]))])#最大度数のジャンルを取り出す（imamuraのサンプルではJazz)
largestGenre = iml[,1][iml[,2] == largestGenreName]
#second
secondLargestGenreName = names(sort(table(iml[,2]), decreasing = T))[2]#2番目に度数が大きいジャンル名を取得(imamuraのサンプルではRock)
secondLargestGenre = iml[,1][iml[,2] == secondLargestGenreName]
#縦軸をそろえたいので最大値を求める
quartz()

#for文で、一回全文のヒストグラムのてっぺんを出して、最大値を出す。
histMax=-1
for(i in 1:length(table(iml[,2]))){
	targetGenreName = names(sort(table(iml[,2]), decreasing = T))[i]
	targetGenre = iml[,1][iml[,2] == targetGenreName]
	
	histMax0 = max(hist(as.POSIXlt(targetGenre)$hour+as.POSIXlt(targetGenre)$min/60, main=targetGenreName, breaks=seq(0,24,1), col = hsv(1*i/length(table(iml[,2])), 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", ylim = c(0, histMax))$counts)
	if(histMax0 > histMax ){
		histMax = histMax0
	}
}
if(histMax == -1){
	cat("something wrong!")
}
#laegest genre hist
#グラフ上書き
hist(as.POSIXlt(largestGenre)$hour+as.POSIXlt(largestGenre)$min/60, main=largestGenreName, breaks=seq(0,24,1), col = hsv(1/2, 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", ylim = c(0, histMax))
#second largest hist
quartz()
hist(as.POSIXlt(secondLargestGenre)$hour+as.POSIXlt(secondLargestGenre)$min/60, main=secondLargestGenreName, breaks=seq(0,24,1), col = hsv(1, 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", ylim = c(0, histMax))
#for文で全ジャンル書き出してみる

for(i in 1:length(table(iml[,2]))){
	targetGenreName = names(sort(table(iml[,2]), decreasing = T))[i]
	targetGenre = iml[,1][iml[,2] == targetGenreName]
	#quartz()
	png(file=paste("genre_",i,".png", sep=""), bg="white")
	hist(as.POSIXlt(targetGenre)$hour+as.POSIXlt(targetGenre)$min/60, main=targetGenreName, breaks=seq(0,24,1), col = hsv(1*i/length(table(iml[,2])), 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", xlim = c(0,23), ylim = c(0, histMax))
	dev.off()
}

#最後まで完了
cat("done!")
