Plugin.require_version("3.0")

sound_extension = "mp3"

score_extensions = {"mscz", "pdf", "mid", "mxl", "mp3"}

-- The mapping of file extensions to human-readable descriptions
mapping = {}
mapping["mscz"] = "MuseScore"
mapping["pdf"] = "PDF"
mapping["mid"] = "MIDI"
mapping["mxl"] = "MusicXML"
mapping["mp3"] = "MP3"

container_selector = config["selector"]
container = HTML.select_one(page, container_selector)

function add_extension(filename, extension)
  return(filename .. "." .. extension)
end

function generate_piece(entry)
  div = HTML.create_element("div")
  HTML.set_attribute(div, "class", "composition")

  Log.debug(JSON.to_string(entry))

  -- Look for files in the dir with the page
  dir = Sys.dirname(page_file)

  url_path = page_url

  if entry["file"] then
    local file = entry["file"]

    -- Insert the header
    HTML.append_child(div, HTML.parse(format([[<h3 id="%s" class="composition-title">%s</h3>]],
      String.slugify_ascii(entry["title"]), entry["title"])))
    HTML.set_attribute(div, "id", "_" .. String.slugify_ascii(entry["title"]))

    -- Insert the playable audio
    sound_file = add_extension(file, sound_extension)
    Log.debug(Sys.join_path(dir, sound_file))
    if Sys.file_exists(Sys.join_path(dir, sound_file)) then
      audio_elem = HTML.parse(format([[<audio src="%s" controls=""></audio>]], Sys.join_path(url_path, sound_file)))
      HTML.append_child(div, audio_elem)
    end

    HTML.append_child(div, HTML.parse(entry["description"]))

    -- Insert date
    HTML.append_child(div, HTML.parse(format([[<p>Date: %s.</p>]], entry["date"])))

    -- Insert download links
    links_p = HTML.create_element("p")
    HTML.append_child(links_p, HTML.create_text("Download: "))
    local k = 1
    local n = 1
    local links = {}
    while score_extensions[k] do
      local ext = score_extensions[k]
      Log.debug(ext)
      if Sys.file_exists(Sys.join_path(dir, add_extension(file, ext))) then
        Log.debug("making link")
        link = format([[<a href="%s">%s</a>]], Sys.join_path(url_path, add_extension(file, ext)), mapping[ext])
        Log.debug("inserring link")
        links[n] = link
        n = n + 1
      end
      k = k + 1
    end
    Log.debug(JSON.pretty_print(links))
    links_src = String.join(", ", links)
    Log.debug("aboiut to parse")
    Log.debug(links_src)
    HTML.append_child(links_p, HTML.parse(links_src))
    HTML.append_child(div, links_p)
  else
    Plugin.fail("Missing file base name")
  end

  HTML.append_child(container, div)
end

Log.debug("reading data")
data = TOML.from_string(Sys.read_file(config["data_file"]))
data = data["compositions"]
--Log.debug(JSON.pretty_print(data))

Log.debug("generating pieces")

-- Table.iter doesn't make any guarantees about the order,
-- so we can't use it and have to enforce original ordering
-- with hand-cranked iteration.
n = 1
while data[n] do
  generate_piece(data[n])
  n = n + 1
end

--Plugin.fail("expected")
