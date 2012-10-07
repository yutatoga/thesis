# -*- coding: utf-8 -*-

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



iml=''
for i in tracks_keys:
    #FIXME: encode problem
    try:
        instantTextDate = tracks[i]["Play Date UTC"]
        instantTextGenre = tracks[i]["Genre"]
        iml = iml + str(instantTextDate) + ',' + str(instantTextGenre) + '\n'
    except:
        pass

print(iml)

#とりあえずこの時点のテキストファイルを書き出す。
#pythonだけでもできるかもしれないけど、とりあえず、以降はRで1.時間とジャンルを統計で使いやすいようにする。2.t検定、多重比較を行うこととした。2012_06_09
test_file = open('iml.txt', 'w')
test_file.writelines(iml)
test_file.flush()
test_file.close()


#1.改行でsplitする
#2.スペースで分割して、偶数目の要素のみだけの行列を生成する
#3.コロンで分割して行列にする