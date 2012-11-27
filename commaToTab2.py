#-*- coding: utf-8 -*-
import sys
import codecs
text = codecs.open("imlTable_2.csv", "r", "utf_16_le")
out = codecs.open('out.csv', 'w', 'utf_16_le')
out.writelines(text)
			