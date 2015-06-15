.PHONY: all
all: clean
	MPP=mpp scripts/mksite.ml

.PHONY: clean
clean:
	rm -rf build/*
