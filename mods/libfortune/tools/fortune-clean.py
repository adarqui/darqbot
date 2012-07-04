#!/usr/bin/python


import re
import sys

#Correctly formatted attribution: double tabs + double dashes, and remainder indented
GOOD_ATTRIBUTION_RE = re.compile(r'\n\t\t-- [^\n ][^\n]*(\n\t\t   [^\n]*)*$')

#Incorrect formatted attribution: Whitespace or incorrect numbers of dashes
BAD_ATTRIBUTION_SPACING_RE = re.compile(r'\n[\t ]*--? *([^\n]*[a-zA-Z.]{2,}[^\n]*$)')

#BAD_ATTRIBUTION_DASHES_RE = re.compile(r'\n\t\t-[^-\n][^\n]*[a-zA-Z.]{2,}[^\n]*$')
BAD_ATTRIBUTION_DASHES_RE = re.compile(r'\n\t\t(-|-{3,}) *([^-\n][^\n]*[a-zA-Z.]{2,}[^\n]*$)')

def readFortuneFile(filename):
	fortuneFile = open (filename, "r")
	current = ""
	for line in fortuneFile:
		if line == "%\n":
			old = current 
			current = ""
			yield old
		else:
			current = current + line
	if current != "":
		yield current

def checkAttribution(fortune):
	""" Checks the attribution is correct"""
	if GOOD_ATTRIBUTION_RE.search(fortune):
		print fortune + '%'
		return True
	if BAD_ATTRIBUTION_SPACING_RE.search(fortune):
		print re.sub(BAD_ATTRIBUTION_SPACING_RE, '\n\t\t-- \g<1>\n%', fortune) ,
		return False
	if BAD_ATTRIBUTION_DASHES_RE.search(fortune):
		print re.sub(BAD_ATTRIBUTION_DASHES_RE, '\n\t\t-- \g<2>\n%', fortune) ,
		return False
	print fortune + '%'
	return True
	
def testFortune(fortune):
	""" This function is used to run a series of checks on an individual fortune"""
	
	if not checkAttribution(fortune):
		pass
#		print "Error in attribution:\n\n" + fortune

if __name__ == "__main__":
	for i in readFortuneFile(sys.argv[1]):
		testFortune(i)
		


