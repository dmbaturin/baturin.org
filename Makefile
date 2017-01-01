.PHONY: all
all: clean
	MPP=mpp scripts/mksite.ml
	scripts/fetch-external-files

.PHONY: clean
clean:
	rm -rf build/*
