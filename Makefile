BUILD_DIR := build
SITE_DIR := site
INDEX_FILE := index.json

ENCAPCALC_REPO := https://github.com/dmbaturin/encapcalc
IPROUTE2_REPO := https://github.com/dmbaturin/iproute2-cheatsheet

# In practice it's in my ~/.local/bin
SOUPAULT := soupault

.PHONY: encapcalc
.ONESHELL: encapcalc
encapcalc:
	echo "Cloning/updating encapcalc"
	if [ ! -d $(SITE_DIR)/tools/encapcalc ]; then git clone $(ENCAPCALC_REPO) $(SITE_DIR)/tools/encapcalc; fi
	cd $(SITE_DIR)/tools/encapcalc
	git pull

.PHONY: iproute2-manual
.ONESHELL: iproute2-manual
iproute2-manual:
	echo "Cloning/updating iproute2 manual"
	if [ ! -d iproute2-cheatsheet ]; then git clone $(IPROUTE2_REPO); fi
	cd iproute2-cheatsheet
	git pull
	$(SOUPAULT)
	mkdir -p ../site/docs/iproute2
	cp build/index.html ../site/docs/iproute2/

.PHONY: site
site: encapcalc iproute2-manual
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
