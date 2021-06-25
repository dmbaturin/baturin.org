Plugin.require_version("2.0")

function process_entry(container, template, entry)
  git_date = Sys.get_program_output(format("git log -n 1 --pretty=format:%%ad --date=format:%%Y-%%m-%%d %s", entry["page_file"]))
  commit_msg = ""
  -- For blog posts, display the excerpt rather than a git commit description
  if entry["nav_path"][1] == "blog" then
    entry["description"] = entry["excerpt"]
  elseif (git_date == entry["date"]) then
    commit_msg = Sys.get_program_output(format("git log -1 --pretty=%%B %s", entry["page_file"]))
    entry["description"] = format("<p>%s</p>", commit_msg)
  else
    entry["description"] = ""
  end

  news_entry = HTML.parse(String.render_template(template, entry))

  HTML.append_child(container, news_entry)
end

max_entries = config["max_entries"]
selector = config["selector"]
template = config["entry_template"]

if not max_entries then
  max_entries = 10
end

if not selector then
  Plugin.fail("selector option is not specified, don't know where to insert news entries")
end

if not template then
  Plugin.fail("entry_template option is not specified")
end

news = HTML.select_one(page, selector)

local n = 1
while (n <= max_entries) do
  entry = site_index[n]

  if entry then
    process_entry(news, template, entry)
  end
  n = n + 1
end
