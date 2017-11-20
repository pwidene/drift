.PHONY.: all clean uml

LATEXMK      = latexmk -f
LATEXMKFLAGS = -pdf -g
LATEXMKCLEAN = -C

SOURCES      = paper.tex
PDF	     = $(SOURCES:.tex=.pdf)
BASE         = $(SOURCES:.tex=)

TEXSRC       = $(filter-out $(SOURCES), $(shell ls *.tex))

BIBSRC       = $(shell ls *.bib)
BBL          = $(notdir $(BIBSRC:.bib=.bbl))

DEPS         = $(DEP) $(TEXSRC) $(BIBSRC) $(wildcard figs/*)

all: umlfigs $(PDF)

$(PDF): %.pdf : %.tex $(DEPS)
	$(LATEXMK) $(LATEXMKFLAGS) $<
	$(LATEXMK) $(LATEXMKFLAGS) $<

$(BBL): $(BIBSRC)
	bibtex $(BASE)

umlfigs:
	$(MAKE) -C uml

view:	all
	$(LATEXMK) -pvc $(LATEXMKFLAGS) paper

clean:
	-$(LATEXMK) $(LATEXMKCLEAN)
	$(MAKE) -C uml clean
	-rm -f *~

realclean: clean
	@rm -f *.bbl *.bak

#


