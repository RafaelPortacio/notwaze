HTMLS = $(wildcard *.html)
CSSS = $(wildcard *.css)
JSS = $(wildcard *.js)

DISTDIR = dist

PROCESSED_HTMLS = $(addprefix $(DISTDIR)/, $(HTMLS))
PROCESSED_CSSS = $(addprefix $(DISTDIR)/, $(CSSS))
PROCESSED_JSS = $(addprefix $(DISTDIR)/, $(JSS))

.PHONY: all
all: $(PROCESSED_HTMLS) $(PROCESSED_CSSS) $(PROCESSED_JSS)

.PHONY: serve
serve: all
	cd $(DISTDIR) && python3 -m http.server -b 127.0.0.1

$(DISTDIR)/%.html: %.html | $(DISTDIR)
	cp $< $@

$(DISTDIR)/%.css: %.css | $(DISTDIR)
	cp $< $@

$(DISTDIR)/%.js: %.js | $(DISTDIR)
	cp $< $@

$(DISTDIR):
	mkdir -p $@
