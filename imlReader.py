#!/usr/bin/env python
#-*- coding: utf-8 -*-
#iTunes Uというジャンルをstrしようとしたら、UnicodeEncodeError: 'ascii' codec can't encode character u'\xa0' in position 6: ordinal not in range(128)というエラーがでたので、http://d.hatena.ne.jp/x-ite/20111130/1322618815を参照し、以下を追加した。
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

#2012_06_09 by Yuta Toga
#reference : http://born1981.g.hatena.ne.jp/gamella/20081019/1224398922

from xml.etree.cElementTree import iterparse
import base64, datetime, re, urllib

unmarshallers = {
    # collections
    "array": lambda x: [v.text for v in x],
    "dict": lambda x:
        dict((x[i].text, x[i+1].text) for i in range(0, len(x), 2)),
    "key": lambda x: x.text or "",
    # simple types
    "string": lambda x: x.text or "",
    "data": lambda x: base64.decodestring(x.text or ""),
    "date": lambda x: datetime.datetime(*map(int, re.findall("\d+", x.text))),
    "true": lambda x: True,
    "false": lambda x: False,
    "real": lambda x: float(x.text),
    "integer": lambda x: int(x.text),
}

def load(file):
    parser = iterparse(file)
    for action, elem in parser:
        unmarshal = unmarshallers.get(elem.tag)
        if unmarshal:
            data = unmarshal(elem)
            elem.clear()
            elem.text = data
        elif elem.tag != "plist":
            raise IOError("unknown plist type: %r" % elem.tag)
    return parser.root[0].text

# usage

itunes = load("iTunes Music Library.xml")

tracks = itunes["Tracks"]
tracks_keys = tracks.keys()

#print(tracks)

iml=''
countSuccess = 0
countFail = 0
"""
for i in tracks_keys:
    #FIXME: encode problem
    try:
		print('sucess')
		countSuccess+=1
		instantTextDate = tracks[i]["Play Date UTC"]
   		instantTextGenre = tracks[i]["Genre"]
   		instantTextTime = tracks[i]["Total Time"]
   		instantTextName = tracks[i]["Name"]
   		iml = iml + str(instantTextDate) + ',' + str(instantTextGenre) + ',' + str(instantTextTime) + ',' + str(instantTextName) + '\n'

    except:
		print("パスしました。")
		countFail+=1
    	#pass
		#print(iml)
"""
countSuccess_totalTime = 0
countSuccess_playDateUTC = 0
countSuccess_genre = 0
countFail_totalTime = 0
countFail_playDateUTC = 0
countFail_genre = 0
countSuccess_playCount = 0
countFail_playCount = 0
countSuccess_trackName = 0
countFail_trackName = 0
countSuccess_disabled = 0
countFail_disabled = 0
for i in tracks_keys:
		#Total Time
		try:
			instantTextTime = tracks[i]["Total Time"]
			countSuccess_totalTime+=1
		except:
			instantTextTime = 0
			countFail_totalTime += 1
		#Play Date UTC
		try:
			instantTextDate = tracks[i]["Play Date UTC"]
			countSuccess_playDateUTC+=1
		except:
			instantTextDate = "never"
			countFail_playDateUTC += 1
		#Genre
		try:
			instantTextGenre = tracks[i]["Genre"]
			#ジャンル名にコロンがあると、コロン区切りで読めなくなって困るので、_ (アンダーバー）で置き換える。
			instantTextGenre = instantTextGenre.replace(",", "_")
			#スラッシュもディレクトリの記号と読まれてエラーが出たので、アンダーバーに置き換える。
			instantTextGenre = instantTextGenre.replace("/", "_")
			#%もエラーが出たので、アンダーバーに置き換える。
			instantTextGenre = instantTextGenre.replace("%", "_")
			#スラッシュでもディレクトリ記号と間違えたので、and に変更
			countSuccess_genre += 1
		except:
			instantTextGenre = "none" 
			countFail_genre += 1
		#Play Count
		try:
			instantTextPlayCount = tracks[i]["Play Count"]
			countSuccess_playCount += 1
		except:
			instantTextPlayCount = 0
			countFail_playCount += 1
		#Name
		try:
			instantTrackName = tracks[i]["Name"]
			instantTrackName =  instantTrackName.replace(",", '_') 
			instantTrackName =  instantTrackName.replace("/", '_') 
			instantTextGenre = instantTextGenre.replace("%", "_")
			countSuccess_trackName += 1
		except:
			instantTrackName = "untitled"
			countFail_trackName += 1
		try:
			instantDisabled = tracks[i]["Disabled"]
			instantDisabled = str(instantDisabled)
			#Trueとなってしまうので、Tを小文字のtにしておく。
			instantDisabled =  instantDisabled.replace("T", 't') 
			countSuccess_disabled  += 1
		except:		
			instantDisabled = "false"
			countFail_disabled  += 1
			
		#区切り文字をコロンにして行列にしていく。
		iml = iml + str(instantTextDate) + ',' + str(instantTextGenre) + ',' + str(instantTextTime) + ',' + str(instantTextPlayCount) + ',' + str(instantDisabled) + ',' + str(instantTrackName) + '\n'
print "\nsuccess-totalTime:%d"% (countSuccess_totalTime)
print "fail-totalTime:%d"%countFail_totalTime
print "total-totalTIme:%d"%(countSuccess_totalTime+countFail_totalTime)

print "\nsuccess-playDate:%d"% (countSuccess_playDateUTC)
print "fail-playDate%d"%countFail_playDateUTC
print "total-playDate:%d"%(countSuccess_playDateUTC+countFail_playDateUTC)

print "\nsuccess-genre:%d"% (countSuccess_genre)
print "fail-genre:%d"%countFail_genre
print "total-genre%d"%(countSuccess_genre+countFail_genre)

print "\nsuccess-playCount:%d"%countSuccess_playCount
print "fail-playCount:%d"%countFail_playCount
print "total-playCount:%d"%(countSuccess_playCount+countFail_playCount)

print "\nsuccess-disabled:%d"%countSuccess_disabled
print "fail-disabled:%d"%countFail_disabled
print "total-disabled:%d"%(countSuccess_disabled+countFail_disabled)

print "\nsuccess-playName:%d"%countSuccess_trackName
print "fail-name:%d"%countFail_trackName
print "total-name:%d"%(countSuccess_trackName+countFail_trackName)



print "\ntotal:%d"%(len(tracks_keys))
#print "iml\n:%s"%iml

#とりあえずこの時点のテキストファイルを書き出す。
#pythonだけでもできるかもしれないけど、とりあえず、以降はRで1.時間とジャンルを統計で使いやすいようにする。2.t検定、多重比較を行うこととした。2012_06_09
test_file = open('iml.txt', 'w')
test_file.writelines(iml)
test_file.flush()
test_file.close()

#1.改行でsplitする
#2.スペースで分割して、偶数目の要素のみだけの行列を生成する
#3.コロンで分割して行列にする

#print(tracks_keys)
print("done!")
