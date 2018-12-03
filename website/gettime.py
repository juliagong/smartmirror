#!/usr/bin/env python

import cgi
import cgitb
from datetime import datetime
cgitb.enable()

print "Content-type: text/html"
# Have to print <html> tags or else page shows server error
print """
<html><header><title>Date&Time</title></header></body>
"""
print('\t'+'\t' + datetime.now().strftime('%A %B %d %Y \n\t\t%H:%M:%S'))
print """
</body></html>
"""
