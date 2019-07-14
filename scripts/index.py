#!/usr/bin/env python3

import sys
import json

import pystache

template = """
<li><a href="{{url}}">{{title}}</a></li>
"""

renderer = pystache.Renderer()

input = sys.stdin.readline()
index_entries = json.loads(input)

print("<ul class=\"nav\">")
for entry in index_entries:
    print(renderer.render(template, entry))
print("</ul>")
