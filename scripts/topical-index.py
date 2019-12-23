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

topics = sorted(list(set(filter(lambda x: x, map(lambda x: x["category"], index_entries)))))

for t in topics:
    print("<h2>{0}</h2>".format(t))
    print("<ul class=\"nav\">")
    entries = filter(lambda x: x["category"] == t, index_entries)
    for entry in entries:
      print(renderer.render(template, entry))
    print("</ul>")

