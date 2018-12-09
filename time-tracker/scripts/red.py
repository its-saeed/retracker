#!/usr/bin/env python

from redminelib import Redmine
import sys
import urllib3

redmine = Redmine("http://projects.mahsan.co", username=sys.argv[1], password=sys.argv[2])

issues = redmine.issue.filter(assigned_to_id="me")

f = open(".issues.txt", "w", encoding="utf-8")

for issue in issues:
	print(issue.id)
	print(",")
	print(str(issue.subject.encode('utf8')).strip())
	print(",")
	print(str(issue.status))
	print("\n")
	
	f.write(str(issue.id))
	f.write(",")
	f.write(issue.subject)
	#f.write(issue.subject.encode('utf8'))
	f.write(",")
	f.write(str(issue.status))

	f.write("\n")

f.close()
#print(json)
