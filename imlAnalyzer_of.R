#目的
#本プログラムは、openFrameworksにて、iTunesの履歴データを、時間軸を含んで読み込むことにより、履歴データから音楽再生の軌跡をアニメーションによって表現するためのcsvファイルをつくるためのものである。
#仕様
#2012/01/26更新
#imlの中から再生された楽曲についての情報を、
#以下に示す通りに、列を決めてcsvファイルをつくる。
#再生時間のタイムスタンプ文字列(string)・トラック名(string)・アーティスト名(string)・ジャンル名(string)・ジャンルランキング番号(string)・年(int)・月(int)・日(int)・時(int)・分(int)・秒(int)・曜日(int)(日曜を0とする)・曜日番号(int)・時刻番号(int)（日曜の０時台を０とする）

#xmlがあるディレクトリに移動
setwd("./xml")
#人数分繰り返す
xmlOwners = list.files()
setwd("../")
for (i in 1 : length(xmlOwners)){
	setwd(paste("./xml/", xmlOwners[i] ,sep = ""))
	#書き出す。
	#ファイルを読み込む

	#DIctionaly
	#ジャンルの数---length(table(iml[,2]))

	iml = read.table('iml.txt', header = F, sep = ",", quote = "")
	#convert string to POSIXlt
	time = strptime(iml[,1],"%Y-%m-%d %H:%M:%S")
	#make data frame which consists of POXIlt and string about genre
	iml = data.frame(time, iml[,2], iml[,3], iml[,4], iml[,5], iml[6], iml[7])
	#convert UTC to JST
	iml[,1] = iml[,1]+9*60*60
	#total time
	names(iml)[1] = "playDate"
	names(iml)[2] = "genre"
	names(iml)[3] = "totalTime"
	names(iml)[4] = "playCount"
	names(iml)[5] = "disabled"
	names(iml)[6] = "trackName"
	names(iml)[7] = "albumName"

	timeFilteredIml = c()
	#ここでimlを2012年4月以降のものだけにする。
	startTime = strptime("2012-4-01 00:00:00","%Y-%m-%d %H:%M:%S")
	for(i in 1 : dim(iml)[1]){
		if(is.na(iml[i, 1]) == FALSE && startTime < iml[i, 1]){
			timeFilteredIml = rbind(timeFilteredIml, iml[i, ])
		}
	}
	#imlに上書きする。前回の再生時間が記録されているので、一度以上聞かれたもので、かつ2012年4月1日以降のもの。
	iml = timeFilteredIml

	#ヒストグラム-経時
	png(file = "~/dropbox/thesis/out_png/all.png")
	hist(iml[,1], breaks=48, main="History", col = rgb(217, 217 , 233,  maxColorValue = 255), xlab = "Time", ylab = "Frequency")
	dev.off()
	#axis(1, at= seq(0, nb, by = 1))

	#ヒストグラム-時間
	png(file = "~/dropbox/thesis/out_png/hour.png")
	hist(as.POSIXlt(iml[,1])$hour+as.POSIXlt(iml[,1])$min/60+as.POSIXlt(iml[,1])$sec/60/60, breaks=seq(0,24,1), main="24 hours", col = rgb(217, 217 , 233,  maxColorValue = 255), xlab = "Hour", ylab = "Frequency")
	dev.off()

	#曜日ごとに色を変え重畳表示
	weekMax = 0#まずは曜日に分けたときの、ヒストグラムの最大値を求める
	for(i in 1:7){
		if (weekMax < max(table(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour))){
			weekMax = max(table(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour))
		}
	}

	png(file = "~/dropbox/thesis/out_png/week_all.png")
	for(i in 1:7){
		if(i>1){
			hist(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$min/60+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$sec/60/60, breaks=seq(0,24,1),main="Week", col = hsv(1*i/7, 1 , 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", add = T, ylim =c(0,weekMax))

		}else{
			hist(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$min/60+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$sec/60/60, breaks=seq(0,24,1),main="Week", col = hsv(1*i/7, 1 , 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", add = F, ylim =c(0,weekMax))
			#axis(1, at= seq(0, 24, by = 1))
		}
	}
	dev.off()

	#曜日ごとに分けて表示
	for(i in 1:7){
		#wdayName = switch(i, "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" )
		#png(paste(file = "~/dropbox/thesis/out_png/week_",as.character(i), "_",wdayName, ".png", sep = ""))
		#hist(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$min/60+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$sec/60/60, breaks=seq(0,24,1), main=wdayName, col = hsv(1*i/7, 1 , 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", add = F,  ylim =c(0,weekMax))
		#dev.off()

		#axis(1, at= seq(0, 24, by = 1), cex.axis = 0.5)
	
		#日本語OKにして、qurartzで書き出す
		#wdayName = switch(i, "日曜日", "月曜日", "火曜日", "水曜日", "木曜日", "金曜日", "土曜日" )
		wdayName = switch(i, "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" )
		hist(as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$hour+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$min/60+as.POSIXlt(iml[,1][as.POSIXlt(iml[,1])$wday==i-1])$sec/60/60, breaks=seq(0,24,1), main=wdayName, col = hsv(1*i/7, 1 , 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", add = F,  ylim =c(0,weekMax))
		quartz.save(paste("~/dropbox/thesis/out_png/",as.character(i), "_",wdayName, ".png", sep = ""))
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
	#quartz()

	#for文で、一回全文のヒストグラムのてっぺんを出して、最大値を出す。
	histMax=-1
	for(i in 1:length(table(iml[,2]))){
		targetGenreName = names(sort(table(iml[,2]), decreasing = T))[i]
		targetGenre = iml[,1][iml[,2] == targetGenreName]

		histMax0 = max(hist(as.POSIXlt(targetGenre)$hour+as.POSIXlt(targetGenre)$min/60+as.POSIXlt(targetGenre)$sec/60/60, main=targetGenreName, breaks=seq(0,24,1), col = hsv(1*i/length(table(iml[,2])), 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", ylim = c(0, histMax))$counts)
		if(histMax0 > histMax ){
			histMax = histMax0
		}
	}
	if(histMax == -1){
		cat("something wrong!")
	}
	#laegest genre hist
	#グラフ上書き
	#hist(as.POSIXlt(largestGenre)$hour+as.POSIXlt(largestGenre)$min/60+as.POSIXlt(largestGenre)$sec/60/60, main=largestGenreName, breaks=seq(0,24,1), col = hsv(1/2, 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", ylim = c(0, histMax))
	#second largest hist
	#quartz()
	#hist(as.POSIXlt(secondLargestGenre)$hour+as.POSIXlt(secondLargestGenre)$min/60+as.POSIXlt(secondLargestGenre)$sec/60/60, main=secondLargestGenreName, breaks=seq(0,24,1), col = hsv(1, 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", ylim = c(0, histMax))


	#ggplot2を使う。
	#library(ggplot2)
	#for(i in 1:length(table(iml[,2]))){
	#	targetGenreName = names(sort(table(iml[,2]), decreasing = T))[i]
	#	targetGenre = iml[,1][iml[,2] == targetGenreName]
	#        print(targetGenreName)
	#        plot = qplot(as.POSIXlt(targetGenre)$hour+as.POSIXlt(targetGenre)$min/60+as.POSIXlt(targetGenre)$sec/60/60, geom = "histogram", binwidth = 1, main = targetGenreName, xlim = c(0,24), ylim = c(0, histMax),  xlab = "Time", ylab = "Frequency")
	#        ggsave(paste("~/dropbox/thesis/out_pdf/","genre_",i,"_",targetGenreName ,".pdf", sep=""), plot, family="Japan1GothicBBB")
	#}





	#再生時間でソート
	sortedIml = iml[sort.list(iml$playDate),]
	#ここまでrunning code
	#今回は1~1863(sortedImlで中身みて、2655までデータが存在して、2656からNANだったので、rownames(sortedIml)==2655でTRUE参照するというアナログで調べた)までデータ存在。
	playDateExistIml = sortedIml[1:1863,]


	#以下で、一回でも再生したことのあるものは、last played dateも記録されているはずという前提で、一回以上再生されたものだけのリストを取得する方法もある。
	#play countだけを変更された場合はエラーが起こる。iTunesの通常の使用ではlast playedの存在と一回以上の再生は一致するが、apple scriptなどを使って、変更した場合はそうはならないかも。

	#play countを調べて、0でなかったら、リスト（playedIml）に追加。
	playedIml = iml[iml[,4]!=0,]
	playedImlSortedByPlayCount =  playedIml[sort.list(playedIml$playCount),] #これと同じ意味の式は右のようにも書ける。 playedImlSortedByPlayCount =  playedIml[sort.list(playedIml[,4]),]
	#実験で使用する2012年の4月からの情報だけにする、filteredPlayedImlという名前で。
	#filteredPlayedIml = playedIml[ , ]


	#for文で全ジャンル書き出してみる
	#FIXME:quartz()を使う
	 for(i in 1:length(table(playedIml[,2][, drop = TRUE]))){
		targetGenreName = names(sort(table(playedIml[,2]), decreasing = T))[i]
		targetGenre = playedIml[,1][playedIml[,2] == targetGenreName]
	
	#         #FIXME:なぜか日本語で書き出されない
			#png(file=paste("~/dropbox/thesis/out_png/","genre_",i,".png", sep=""), bg="white", type="quartz")
			#hist(as.POSIXlt(targetGenre)$hour+as.POSIXlt(targetGenre)$min/60+as.POSIXlt(targetGenre)$sec/60/60, main=targetGenreName, breaks=seq(0,24,1), col = hsv(1*i/length(table(iml[,2])), 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", xlim = c(0,23), ylim = c(0, histMax))
			#dev.off()
		 
			 #日本語OKにして、qurartzで書き出す
			 hist(as.POSIXlt(targetGenre)$hour+as.POSIXlt(targetGenre)$min/60+as.POSIXlt(targetGenre)$sec/60/60, main=targetGenreName, breaks=seq(0,24,1), col = hsv(1*i/length(table(playedIml[,2][, drop=TRUE])), 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", xlim = c(0,23), ylim = c(0, histMax))
			 quartz.save(paste("~/dropbox/thesis/out_png/","genre_",i, "_", targetGenreName, ".png", sep=""))
	 }



	#一度以上再生されたジャンルをplayedImlから曜日毎に書き出す。
	#ジャズだけを取り出す　playedIml[playedIml$genre=="Jazz",]
	#日曜日だけ取り出す。  playedIml[as.POSIXlt(playedIml[,1])$wday==0,]
	#再生回数が1回で、ジャンルがJazzなものだけを取り出す。　playedIml[playedIml$playCount==1 & playedIml$genre == "Jazz",]
	 #24列で、曜日が変わったら、下の行に行く。（ジャンルが多いと、行（行はジャンルの数x7）が伸びて、列は24列固定。）
	 graphData  = c()
	 #24x7列で、曜日か変わったら、右の列に行く。（ジャンルが多いと、行（行はジャンルの数とイコール）が増えて、列は24*7列固定。）
	 graphDataMatrix = c();
	 instantGraphDataMatrix = c()
	 for(weekNum in 1 : 7){
		 for(i in 1:length(table(playedIml[,2][, drop = TRUE]))){
			targetGenreName = names(sort(table(playedIml[,2][, drop = TRUE]), decreasing = T))[i]
			imlWeekGenre = playedIml[  as.POSIXlt(playedIml[,1])$wday==weekNum-1 & playedIml$genre == targetGenreName,]
			imlWeekGenreTime = imlWeekGenre[,1]
			wdayName = switch(weekNum, "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" )
			histData = hist(as.POSIXlt(imlWeekGenreTime)$hour+as.POSIXlt(imlWeekGenreTime)$min/60+as.POSIXlt(imlWeekGenreTime)$sec/60/60, main=paste(wdayName, "_", targetGenreName, sep = "")  , breaks=seq(0,24,1), col = hsv(1*i/length(table(playedIml[,2][, drop = TRUE])), 1, 1, alpha = 0.5), xlab = "Time", ylab = "Frequency", xlim = c(0,23), ylim = c(0, histMax)) 
			graphData = rbind(graphData, histData$counts)
			instantGraphDataMatrix = rbind(instantGraphDataMatrix, histData$counts)
			quartz.save(paste("~/dropbox/thesis/out_png/", weekNum, "_", wdayName, "_", i,"_", targetGenreName, ".png", sep=""))	
		}
		graphDataMatrix = cbind(graphDataMatrix, instantGraphDataMatrix)
		instantGraphDataMatrix = c()
	}

	#音楽の長さと最後に聞いた時間をもとに、操作して選択された曲だけのリストを求める
	#iml[sort.list(iml$playDate),]
	#iml[2036,][1]-round(iml[2036,][3]/1000)#ほとんど、同じか、1秒小さいか、1秒大きい。
	#nrow(iml)
	#append
	diff = c()
	for (i in 2:nrow(playDateExistIml)){
		diff = append(diff, playDateExistIml[i-1,][1] - (playDateExistIml[i,][1]-round(playDateExistIml[i,][3]/1000)))#ここはplay dateでソートしてから行うように書き換える！
	}
	diffAbs = abs(as.numeric(diff))
	selectedItems = diffAbs == 1

	#行番号取得　rownames(sortedIml)[nrow(sortedIml)]

	#再生回数に応じて、カウントの数を動的に変化させる。例えば、一回しか再生されていないものは、プラス１、2回再生されたものはプラス2というかんじ。回数が多いほど、お気に入りの可能性が高く、ランダム再生でたまたま再生される可能性が低いという推測から。
	#play count回数分をfor文で loopでコピーして追加する。
	#
	#低速なのでコメントアウトする。
	#instantCountOfPlayedImlSortedByPlayCount = dim(playedImlSortedByPlayCount)[1]
	#imlPlusCount = playedImlSortedByPlayCount
	#
	#for(i in 1:instantCountOfPlayedImlSortedByPlayCount){
	#    if(playedImlSortedByPlayCount[i,4]>1){
	#        for (j in 1:(playedImlSortedByPlayCount[i,4]-1)){
	#            imlPlusCount  = rbind(imlPlusCount, playedImlSortedByPlayCount[i,])
	#            #imlPlusCount = rbind(imlPlusCount, rbind(rep(playedImlSortedByPlayCount[i,], playedImlSortedByPlayCount[i,4]-1)))
	#        }
	#    }
	#}

	sum(iml[,4])
	dim(imlPlusCount)

	#csvを書き出す。
	#曜日毎のジャンルの累積グラフのテーブル
	write.csv(graphData, "graphData.csv")
	#横軸
	write.csv(graphDataMatrix, "graphDataMatrix.csv");

	#ジャンル
	genreNameList = names(sort(table(playedIml[,2][, drop = TRUE]), decreasing = T))
	write.csv(genreNameList, "genreNameList.csv")

	#disableが入っていない（チェックが入っている）楽曲をジャンルごとにカウントして、csvに書き出す。
	checkedIml = iml[iml[,5]=="false",]
	write.csv(sort(table(checkedIml[,2]), decreasing = T), "checkedImlGenreTable.csv")

	#最終の時刻と最新の時刻を書き出す。
	write.csv(c(as.character(sortedIml[1,1]), as.character(sortedIml[dim(sortedIml)[1],1])), "timeStamp_oldest_newest.csv")


	#再生時間のタイムスタンプ文字列(string)・トラック名(string)・アーティスト名(string)・ジャンル名(string)・ジャンルランキング番号(string)・再生カウント（int）・年(int)・月(int)・日(int)・時(int)・分(int)・秒(int)・曜日(int)(日曜を0とする)・曜日番号(int)・時刻番号(int)（日曜の０時台を０とする）
	#まず、ジャンルランキング番号(string)以外はimlから計算して出せる。ジャンルランキングは、ジャンル名と、ランキングデータを調べて、合致するのが何行目かを調べる。for文まわしすぎかな。。。
	#sortedIml: playDate genre totalTime playCount disabled         trackName


	#ジャンルの順位を調べる
	genreRank = c()
	for (j in 1:nrow(sortedIml)){
		for (i in 1:length(genreNameList)){
			if(sortedIml[j,2] == genreNameList[i]){
				genreRank = append(genreRank, i)
			}
		}
	}
	#時間ID（日曜日の0時台が0, 土曜日の23時台が168）
	timeID =  as.POSIXlt(sortedIml[,1])$wday*24+as.POSIXlt(sortedIml[,1])$hour

	#ミニマムテスト
	#日付、トラック名、ジャンル名、ジャンルランク、時間ID（日曜日の0時台が0, 土曜日の23時台が168）
	imlOf = data.frame(sortedIml[,1], sortedIml[,6], sortedIml[,2], genreRank, timeID, sortedIml[,7])
	#write.csv(imlOf, "of.csv", qmethod = "escape")#文字列を""で囲う簡易関数
	write.table(imlOf, file = "of.csv", sep = ",", col.names = NA, quote = FALSE)

	#ディレクトリの削除
	#unlink(paste(getwd(), "/foo", sep = ""),recursive = TRUE, force = FALSE)
	#ディレクトリの作成
	#dir.create(xmlOwners[i], showWarnings = FALSE)
	setwd("../")
	setwd("../")
}

#最後まで完了
cat("done!\n")
