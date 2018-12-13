#!/usr/bin/env python

import cgi
import cgitb
from datetime import datetime
cgitb.enable()

print ("Content-type: text/plain\n")

print( datetime.now().strftime('*%A *%B *%m *%d *%Y *%H *%M *%S'))
