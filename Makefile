BUILD_DIR := build
SITE_DIR := site
INDEX_FILE := index.json

# In practice it's in my ~/.local/bin
SOUPAULT := soupault

.PHONY: site
site:
	$(SOUPAULT) --index-only
	scripts/blog-archive.py $(SITE_DIR)/blog/tag $(INDEX_FILE)
	$(SOUPAULT)
	scripts/json2feed.py index.json > $(BUILD_DIR)/blog/atom.xml
	# OCaml posts for syndication on https://ocaml.org/community/planet/
	scripts/json2feed.py index.json ocaml > $(BUILD_DIR)/blog/atom-ocaml.xml

.PHONY: assets
assets:
	cp -r assets/* $(BUILD_DIR)/

.PHONY: all
all: site assets

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: serve
serve:
	python3 -m http.server --directory $(BUILD_DIR)

.PHONY: deploy
deploy:
	rsync -a -e "ssh" $(BUILD_DIR)/ www.baturin.org:/var/www/vhosts/baturin.org
