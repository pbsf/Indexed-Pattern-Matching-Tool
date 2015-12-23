NAME:=relatorio

# To update, run: latex-process-inputs -makefilelist main.tex
TEX_FILES = \
relatorio.tex \
description.tex \
experiments.tex \
implementation.tex \
intro.tex \
macros.tex \
requirements.tex

pdf:
	-rm -f $(NAME).aux
	pdflatex $(NAME)
	bibtex $(NAME)
	pdflatex $(NAME)
	@echo '****************************************************************'
	pdflatex $(NAME)

final: pdf
	@echo '******** Did you spell-check the paper? ********'
	pdffonts $(NAME).pdf; fi

warnings: bib-update
	make pdf > /tmp/removeme
	@echo '****************************************************************'
	pdflatex $(NAME) | grep "Warning"

export BIBINPUTS ?= .:bib

clean:
	-rm -f *aux *bbl *blg *log *.dvi main.pdf *.ps *.toc

tags: TAGS
TAGS: ${TEX_FILES}
	etags ${TEX_FILES}

