#-*- coding: utf-8 -*-
textFile = open("imlTable_1.csv", "r")
imlTableTab_1 = ""
for line in textFile:
	temp_list = line.strip().split(",")
	output_line = "\t".join(temp_list)
	imlTableTab_1 = imlTableTab_1 + output_line + "\n"
test_file = open('imlTable_2.csv', 'w')
test_file.writelines(imlTableTab_1)
test_file.flush()
test_file.close()