#!/usr/bin/env python
# coding: utf-8

import argparse
import sys
import requests

from HTMLParser import HTMLParser

import base64

class MyHtmlParser(HTMLParser):
	def handle_starttag(self, tag, attrs):
		if tag == "meta" and len(attrs) == 2 and attrs[0][0] == 'name' and attrs[1][0] == 'content':
			if attrs[0][1] == 'csrf-token':
				self.authenticity_token = attrs[1][1]

def login(username, password):

	base64_of_username_password = base64.b64encode(username + ":" + password)
	session = requests.Session()
	
	response = session.get("http://projects.mahsan.co/login")
	
	cookies = ""
	
	for key in session.cookies.get_dict():
		if cookies != "":
			cookies += "; "
		cookies += key + "=" + session.cookies.get_dict()[key]
	
	parser = MyHtmlParser()
	
	parser.feed(response.text)
	
	
	authenticity_token =  parser.authenticity_token
	authenticity_token_u =  parser.authenticity_token
	
	authenticity_token_u = authenticity_token_u.replace("=", "%3D")
	authenticity_token_u = authenticity_token_u.replace("+", "%2B")
	authenticity_token_u = authenticity_token_u.replace("/", "%2F")
	
	URL = "http://projects.mahsan.co/login"
	
	pattern = '%s=%s'
	
	data = pattern % ("utf8","%E2%9C%93")
	data += "&"
	data += pattern % ("authenticity_token", authenticity_token_u)
	data += "&"
	data += pattern % ("back_url","http%3A%2F%2Fprojects.mahsan.co%2F")
	data += "&"
	data += pattern % ("username", username)
	data += "&"
	data += pattern % ("password", password)
	data += "&"
	data += pattern % ("autologin","1")
	data += "&"
	data += pattern % ("login","%D9%88%D8%B1%D9%88%D8%AF")
	
	headers = {
		"Connection" : "keep-alive",
		"Cache-Control" : "max-age=0",
		"Authorization" : "Basic " + base64_of_username_password,
		"Upgrade-Insecure-Requests" : "1",
		"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36",
		"Content-Type" : "application/x-www-form-urlencoded",
		"Accept" : "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8",
		"Accept-Encoding" : "gzip, deflate",
		"Accept-Language" : "en-US,en;q=0.9",
		"Cookie" : cookies
	}
	
	response = session.post(url = URL, data = data, headers = headers, allow_redirects=False)
	
	for key in session.cookies.get_dict():
		if cookies != "":
			cookies += "; "
		cookies += key + "=" + session.cookies.get_dict()[key]

	return (authenticity_token, cookies)

def create_time_entry(username, password, authenticity_token, cookies, issue_id, date, hours, ideal_hours, comments, activity_id):

	base64_of_username_password = base64.b64encode(username + ":" + password)
	
	URL = "http://projects.mahsan.co/time_entries"
	
	pattern = '------WebKitFormBoundaryzc5QRnTh51RG02dB\r\nContent-Disposition: form-data; name="%s"\r\n\r\n%s\r\n'
	
	data = pattern % ("utf8","e29c93")
	data += pattern % ("authenticity_token", authenticity_token)
	data += pattern % ("issue_id", issue_id)
	data += pattern % ("time_entry[issue_id]", issue_id)
	data += pattern % ("time_entry[spent_on]", date)
	data += pattern % ("time_entry[hours]", hours)
	data += pattern % ("time_entry[comments]", comments)
	data += pattern % ("time_entry[activity_id]", activity_id)
	data += pattern % ("time_entry[custom_field_values][11]", ideal_hours)
	data += pattern % ("time_entry[custom_field_values][12]","")
	data += pattern % ("time_entry[custom_field_values][151]","")
	data += pattern % ("commit","")
	
	
	data += '------WebKitFormBoundaryzc5QRnTh51RG02dB--\r\n'
	
	headers = {
		"Connection" : "keep-alive",
		"Cache-Control" : "max-age=0",
		"Authorization" : "Basic " + base64_of_username_password,
		"Origin" : "http://projects.mahsan.co",
		"Upgrade-Insecure-Requests" : "1",
		"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36",
		"Content-Type" : "multipart/form-data; boundary=----WebKitFormBoundaryzc5QRnTh51RG02dB",
		"Accept" : "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8",
		"Accept-Encoding" : "gzip, deflate",
		"Accept-Language" : "en-US,en;q=0.9",
		"Cookie" : cookies
	}
	
	response = requests.post(url = URL, data = data, headers = headers, allow_redirects=False)

def main():
    """
        Prepare test results as a message and send it to redmine.
    """

    parser = argparse.ArgumentParser(description='Create Time Entity In Redmine.')
    parser.add_argument('-u', '--username', action="store", help='Redmine Username', required=True)
    parser.add_argument('-p', '--password', action="store", help='Redmine Passworn', required=True)
    parser.add_argument('-i', '--issue', action="store", help='issue number', required=True)
    parser.add_argument('-d', '--date', action="store", help='', required=True)
    parser.add_argument('-sh', '--hours', action="store", help='Sepent Hourse. for example: 1.25', required=True)
    parser.add_argument('-ih', '--ideal_hours', action="store", help='', required=True)
    parser.add_argument('-c', '--comments', action="store", help='', required=True)
    parser.add_argument('-a', '--activity_id', action="store", help='', required=True)
    args = parser.parse_args()

    (authenticity_token, cookies) = login(args.username, args.password)
    create_time_entry(args.username, args.password, authenticity_token, cookies, args.issue, args.date, args.hours, args.ideal_hours, args.comments, args.activity_id)

if __name__ == "__main__":
    main()
