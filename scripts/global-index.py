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
<dt>{{date}} <a href="{{url}}">{{title}}</a></dt>
"""

renderer = pystache.Renderer()

with open('index.json') as f:
    entries = json.load(f)

entries = entries[:10]

print("""<dl>""")
for e in entries:
    print(renderer.render(template, e))

    git_date = get_output('git log -n 1 --pretty=format:%ad --date=format:%Y-%m-%d', e['page_file'])
    if git_date == e['date']:
        commit_msg = get_output('git log -1 --pretty=%B', e['page_file'])
        commit_msg = re.sub(r'\[(.*)\]', r'<a href="\1">\1</a>',  commit_msg)
        print("""<dd>{0}</dd>""".format(commit_msg))

print("</dl>")
