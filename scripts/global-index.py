#!/usr/bin/env python3

import re
import sys
import json
import subprocess

import pystache

def get_output(cmd, file):
    cmd = '{0} -- {1}'.format(cmd, file)
    return subprocess.check_output(cmd, shell=True).decode()


template = """
<div class="site-news-entry">
  <div class="site-news-date"><time>{{date}}</time></div>
  <div class="site-news-content">
    <h3><a href="{{url}}">{{title}}</a></h3>
    {{{description}}}
  </div>
</div>
"""

renderer = pystache.Renderer()

with open('index.json') as f:
    entries = json.load(f)

entries = entries[:10]

print("""<div id="site-news">""")
for e in entries:
    commit_msg = ""
    git_date = get_output('git log -n 1 --pretty=format:%ad --date=format:%Y-%m-%d', e['page_file'])
    if git_date == e['date']:
        commit_msg = get_output('git log -1 --pretty=%B', e['page_file'])
    e["description"] = "<p>{0}</p>".format(commit_msg)

    print(renderer.render(template, e))

print("</div>")
