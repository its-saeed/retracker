#!/usr/bin/env python

from redminelib import Redmine
import sys

redmine = Redmine("http://projects.mahsan.co", username=sys.argv[1], password=sys.argv[2])

issues = redmine.issue.filter(assigned_to_id="me")

f = open(".issues.txt", "w")

for issue in issues:
	f.write(str(issue.id))
	f.write(",")
	f.write(issue.subject.encode('utf8'))
	f.write(",")
	f.write(str(issue.status))
	f.write("\n")

f.close()
#print(json)
