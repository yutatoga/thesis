#DIctionaly
#ジャンルの数---length(table(iml[,2]))

iml = read.table('iml.txt', header = F, sep = ",", quote = "")
#convert string to POSIXlt
time = strptime(iml[,1],"%Y-%m-%d %H:%M:%S")
#make data frame which consists of POXIlt and string about genre
iml = data.frame(time, iml[,2], iml[,3], iml[,4], iml[,5], iml[6])
#convert UTC to JST
iml[,1] = iml[,1]+9*60*60
#total time
names(iml)[1] = "playDate"
names(iml)[2] = "genre"
names(iml)[3] = "totalTime"
names(iml)[4] = "playCount"
names(iml)[5] = "disabled"
names(iml)[6] = "trackName"

#disableが入っていない（チェックが入っている）楽曲をジャンルごとにカウントして、csvに書き出す。
checkedIml = iml[iml[,5]=="false",]
write.csv(sort(table(checkedIml[,2]), decreasing = T), "checkedImlGenreTable.csv")

#最後まで完了
cat("done!\n")
