Plugin.require_version("1.11")

base_path = config["base_path"]
if not base_path then
  base_path = "/tag"
end

function make_tag_links(tags_elem)
  tags_string = String.trim(HTML.strip_tags(tags_elem))
  if not tags_string then
    return nil
  end

  -- Split the tags string like "foo, bar" into tags
  tags = Regex.split(tags_string, ",")

  -- Generate <a href="/tag/$tag">$tag</a> elements for each tag
  local count = size(tags)
  local index = 1
  tag_links = {}
  while (index <= count) do
    tag = String.trim(tags[index])

    tag_link = HTML.create_element("a", tag)
    HTML.set_attribute(tag_link, "href", format("%s/%s", base_path, tag))

    tag_links[index] = tostring(tag_link)

    index = index + 1
  end

  links_html = String.join(", ", tag_links)

  tags_html = [[
    <p>
      <strong>Tags:</strong> <span id="tags">%s</span>
    </p>
  ]]

  links_html = format(tags_html, links_html)

  HTML.insert_after(tags_elem, HTML.parse(links_html))
  HTML.delete_element(tags_elem)
end

tags_elems = HTML.select(page, "tags")
index = 1
count = size(tags_elems)
while (index <= count) do
  make_tag_links(tags_elems[index])
  index = index + 1
end
