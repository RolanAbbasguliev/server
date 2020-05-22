#!/usr/bin/env python2.7

import sys
import os

files_path = (str)(os.environ['F_PATH'])

body = ""

body += "<html>"
body += "<head>"
body += "<link rel=\"icon\" href=\"" + files_path + "/favicon.ico\">"
body += "<title>Send form</title>"
body += "<style>"
body += "body {background-image: url(" + files_path + "/backgr.png);}" 
body += "</style>"
body += "</head>"
body += "<body>"
body += "<form action=\"send\" method=\"POST\">"
body += "<div>"
body += "<label for=\"say\">What greeting do you want to say?</label>"
body += "<input name=\"say\" id=\"say\" value=\"Hi\">"
body += "</div>"
body += "<div>"
body += "<label for=\"to\">Who do you want to say it to?</label>"
body += "<input name=\"to\" id=\"to\" value=\"Mom\">"
body += "</div>"
body += "<div>"
body += "<button>Send my greetings</button>"
body += "</div>"
body += "</form>"
body += "</body>"
body += "</html>"

headers = ""

headers += "HTTP/1.1 200 OK\r\n"
headers += "Content-Type: text/html; charset=utf-8\r\n"
headers += "Connection: keep-alive\r\n"
headers += "Content-Length: "
headers += (str)(len(body))
headers += "\r\n\r\n"

response = ""
response += headers
response += body

sys.stdout.write( response )
sys.stdout.flush()
