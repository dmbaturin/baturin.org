BUILD_DIR := build

# In practice it's in my ~/.local/bin
SOUPAULT := soupault

.PHONY: site
site:
	$(SOUPAULT)

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
