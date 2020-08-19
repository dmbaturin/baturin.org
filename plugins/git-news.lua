Plugin.require_version("2.0")

function process_entry(container, entry)
  git_date = Sys.get_program_output(format("git log -n 1 --pretty=format:%%ad --date=format:%%Y-%%m-%%d %s", entry["page_file"]))
  commit_msg = ""
  if (git_date == entry["date"]) then
    commit_msg = Sys.get_program_output(format("git log -1 --pretty=%%B %s", entry["page_file"]))
    entry["description"] = format("<p>%s</p>", commit_msg)
  else
    entry["description"] = ""
  end

  news_content = HTML.create_element("div")
  HTML.add_class(news_content, "site-news-content")
  HTML.append_child(news_content,
    HTML.parse(format("<h3><a href=\"%s\">%s</a></h3> %s",
      entry["url"], entry["title"], entry["description"])))
  news_date = HTML.parse(format("<div class=\"site-news-date\"><time>%s</time></div>", entry["date"]))

  news_entry = HTML.create_element("div")
  HTML.add_class(news_entry, "site-news-entry")
  HTML.append_child(news_entry, news_date)
  HTML.append_child(news_entry,	news_content)

  HTML.append_child(container, news_entry)
end

max_entries = config["max_entries"]
selector = config["selector"]

if not max_entries then
  max_entries = 10
end

if not selector then
  Plugin.fail("selector option is not specified, don't know where to insert news entries")
end

news = HTML.select_one(page, selector)

local n = 1
while (n <= max_entries) do
  entry = site_index[n]

  if entry then
    process_entry(news, entry)
  end
  n = n + 1
end
