#!/usr/bin/env python3

import sys
import json
import datetime

import dateutil.tz
import dateutil.parser

from datetime import datetime
from feedgen.feed import FeedGenerator


base_url = 'https://baturin.org/'

default_date = datetime(1970, 1, 1, tzinfo=dateutil.tz.gettz('Etc/UTC'))

feed_id = "https://baturin.org/blog/atom.xml"
feed_title = "Daniil Baturin\'s blog"
feed_subtitle = "On programming and other subjects"
feed_logo = "https://baturin.org/images/favicon.png"
feed_language = "en"
feed_author = "Daniil Baturin"
feed_author_email = "daniil+website@baturin.org"

def get_date(ds):
    try:
        return dateutil.parser.parse(ds, default=default_date).isoformat()
    except:
        return default_date


fg = FeedGenerator()
fg.id(feed_id)
fg.title(feed_title)
fg.subtitle(feed_subtitle)
fg.author( {'name': feed_author, 'email': feed_author_email} )
#fg.link( href=, rel='alternate' )
fg.logo(feed_logo)
fg.language(feed_language)

index_file = sys.argv[1]

with open(index_file, 'r') as f:
    entries = json.load(f)

for entry in entries:
    if not entry["nav_path"] or (entry["nav_path"][0] != "blog"):
        continue

    fe = fg.add_entry()

    url = base_url + entry["url"]

    fe.id(url)
    fe.link(href=url, rel="alternate")
    fe.title(entry["title"])
    fe.content(entry["excerpt"], type='html')
    fe.updated(get_date(entry["date"]))


atomfeed = fg.atom_str(pretty=True).decode()

print(atomfeed)
