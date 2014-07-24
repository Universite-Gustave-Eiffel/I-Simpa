# -*- coding: UTF8 -*-

# This script is created in order to translate using Po files all the I-Simpa source
# from French to English. 
# The traduction works only with occurrences matching "_(" above them.
# For more details please read "LogChangeLanguage.txt" file.

import sys
import os
import re
import codecs
import fileinput
import shutil
import datetime

check = -1
tmp_msgid = ""
tmp_msgstr = ""
save_paths = []
dictionary = {}
pathDictiona = {}
filename = (("logChangeLanguage%s.txt" % (str(datetime.datetime.now())).split(".")[0]).replace(" ", "_")).replace(":", "-")
log = open(filename, 'ab+')

# In order to treat each file just one time for fastest running
# This function search for each path if exist, add in a dictionary
# a new list of occurence storage as follow:
# [msgid, msgstr, line]. I could not stock the line,
# but to prevent wrong replace occurence, i will check after find
# the occurence line the position of this line using the index
# method

def changePoFile(dictionary):
	for keys in dictionary:
		input(keys)
	# Print the new version in the Pofile
	with codecs.open("header_po", 'r', encoding='utf-8') as head:
		header = head.readlines()

	with codecs.open("messages.po", 'w', encoding='utf-8') as file:
		for line in header:
			if line.find("%s") != -1:
				time = (str(datetime.datetime.now())).split(".")[0]
				time = time.split(":")[0] + ':' + time.split(':')[1]
				line = line.replace("%s", time)
			file.write(line)
		file.write('\n\n')

		for keys in dictionary:
			for value in dictionary.get(keys):
				file.write("#: %s" % value)
			if dictionary.get(keys):
				file.write("msgid \"" + keys[1] + "\"\n")
				file.write("msgstr \"" + keys[0] + "\"\n\n")

# dictionary contain all data from the po file path as first parameter of the script
# The storage is as follow :
# {[msgid, msgstr], [first path, seconde path, etc...]}
if len(sys.argv) > 1:
	try:
		with codecs.open(sys.argv[1], 'r', encoding='utf8') as f:
			text = f.readlines()
			for line in text:
				if line.find('#: ../isimpa/') != -1 or line.find('#, c-format') != -1:
					if check == 1:
						dictionary[(tmp_msgstr, tmp_msgstr)] = save_paths
						tmp_msgid = ""
						tmp_msgstr = ""
						save_paths = list()
						check = -1
					save_paths.append(line.split(" ")[1])
				elif line.find('msgid') != -1:
					if line.find("#~ msgid") != -1:
						dictionary[(tmp_msgstr, tmp_msgstr)] = save_paths
						tmp_msgid = ""
						tmp_msgstr = ""
						save_paths = list()
					check = 0
					tmp_msgid = tmp_msgid + line.split('"')[1]
				elif line.find('msgstr') != -1:
					check = 1
					tmp_msgstr = tmp_msgstr + line.split('"')[1]
				elif check == 0:
					tmp_msgid = tmp_msgid + line.split('"')[1]
				elif check == 1:
					if line.strip() and line.split('"'):
						try:
							tmp_msgstr = tmp_msgstr + line.split('"')[1]
						except:
							pass
		input("(" + tmp_msgstr + ", " + tmp_msgstr + ") = " + save_paths)
		dictionary[(tmp_msgstr, tmp_msgstr)] = save_paths
		f.close()
		changePoFile(dictionary)
		input("Treatment done")
	except IOError:
			print("Error: Check path directory of your file %s " % sys.argv[1])
else:
	print(len(sys.argv))