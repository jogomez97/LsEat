SUBDIRS=./Picard ./Enterprise ./Data

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)
