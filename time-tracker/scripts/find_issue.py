#!/usr/bin/env python

from redminelib import Redmine
import sys

redmine = Redmine("http://projects.mahsan.co", username=sys.argv[1], password=sys.argv[2])

issue = redmine.issue.get(sys.argv[3])

f = open(".issues.txt", "w")

f.write(str(issue.id))
f.write(",")
f.write(issue.subject.encode('utf8'))
f.write(",")
f.write(str(issue.status))
f.write("\n")

f.close()

