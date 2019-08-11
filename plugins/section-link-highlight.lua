-- Highlights the link to the current section in the menu
-- Quite hacky, if you ask me

active_link_class = config["active_link_class"]
nav_menu_selector = config["selector"]

menu = HTML.select_one(page, nav_menu_selector)
links = HTML.select(menu, "a")

-- For those who want to try it on Windows
page_file = Regex.replace_all(page_file, "\\\\", "/")

index, link = next(links)
while index do
  href = strlower(HTML.get_attribute(link, "href"))
  -- Don't highlight the main page
  -- That would also highlight all paths since they all have / in them
  if href ~= "/" then
    if Regex.match(page_file, href) then
      HTML.add_class(link, active_link_class)
    end
  end
  index, link = next(links, index)
end
