#!/usr/bin/env python

import cgi
import cgitb
from datetime import datetime
cgitb.enable()

print "Content-type: text/html"
print """
<html>
        <head><title>Time & Date</title></head>
        <body>
"""
print('\t'+'\t' + datetime.now().strftime('%A %B %d %Y \n\t\t%H:%M:%S'))
print """
        </body>
</html>
"""
