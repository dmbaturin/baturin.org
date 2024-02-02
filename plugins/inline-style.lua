styles = HTML.select(page, "inlineStyle")

function add_style(s)
  css = HTML.inner_html(s)
  e = HTML.create_element("style", css)
  head = HTML.select_one(page, "head")
  HTML.append_child(head, e)
end

Table.iter_values(add_style, styles)

Table.iter_values(HTML.delete, styles)
