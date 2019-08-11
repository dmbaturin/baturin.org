-- Inserts a div to be used by the git-timestamp widget
-- iff the page doesn't have a handwritten revision
--
-- In most cases timestamp generator from git log is perfect,
-- but sometimes minor or markup-only change warrants preserving
-- the date of last significant modification
--
-- The revision selector option defines the tag where a handwritten revision is found
-- The selector option defines where the timestamp container is inserted

revision_selector = config["revision_selector"]
selector = config["selector"]

revision = HTML.select_one(page, revision_selector)

if not revision then
  container = HTML.select_one(page, selector)
  git_timestamp_div = HTML.parse("<div id=\"last-modified\">This page was last modified:\
    <time id=\"git-timestamp\"> </time> </div>")
  HTML.append_child(container, git_timestamp_div)
end
