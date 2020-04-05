#!/usr/bin/env python3

import os
import sys
import json

import pystache

template = """
<h2><a href="{{url}}">{{{title}}}</a></h2>
<p><strong>Last update:</strong> {{date}}.</p>
<tags>{{tags}}</tags>
<p>{{{excerpt}}}</p>
<a href="{{url}}">Read more</a>
<hr>
"""

base_path = sys.argv[1]

try:
    index_file = sys.argv[2]
    with open(index_file) as f:
        index_entries = json.load(f)
except:
    print("Could not read index file!")
    sys.exit(1)

renderer = pystache.Renderer()

# Create a dict of entries grouped by tag
entries = {}
for e in index_entries:
    tags_string = e["tags"]
    if tags_string:
        tag_strings = filter(lambda s: s != "",  map(lambda s: s.strip(), tags_string.split(",")))
    else:
        continue

    for t in tag_strings:
        if not (t in entries):
            entries[t] = []
        entries[t].append(e)

# Now for each tag, create an HTML page with all entries that have that tag
for t in entries:
    page_file = os.path.join(base_path, t) + '.html'
    with open(page_file, 'w') as f:
        print("""<h1>Posts tagged &ldquo;{}&rdquo;</h1>""".format(t), file=f)
        for e in entries[t]:
            print(renderer.render(template, e), file=f)
