BUILD_DIR := build/

SOUPAULT := soupault

.PHONY: site
site:
	$(SOUPAULT)

.PHONY: assets
assets:
	cp -r assets/* $(BUILD_DIR)

.PHONY: all
all: site assets

.PHONY: clean
clean:
	rm -rf build/*
